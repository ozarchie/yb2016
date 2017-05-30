#include <dht.h>   // Latest DHTnn library
#include <Time.h>
#include <EEPROM.h>
#include "E2P_Map.h"
#include <Wire.h>
#include <DS1307RTC.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include "SensorEC.h"
#include <avr/pgmspace.h>
#include "Arduino.h"

/*
//VARIABLES
*/
#define MSG_INTERVAL 10
#define LCD_INTERVAL 100

boolean stringComplete = false;
int serialcounter = 0;
int serialincounter = 0;

char SerialBuffer[80] = { 1,1,1,1,1,1,1,1,1,1,              // Testing, until the problem with PString() was found
                          2,2,2,2,2,2,2,2,2,2,
                          3,3,3,3,3,3,3,3,3,3,
                          4,4,4,4,4,4,4,4,4,4,
                          5,5,5,5,5,5,5,5,5,5,
                          6,6,6,6,6,6,6,6,6,6,
                          7,7,7,7,7,7,7,7,7,7,
                          8,8,8,8,8,8,8,8,8,8
                          };
int  pSerialBuffer = 0;
char* message[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};      // Maximum 16 elements in array

// RTC Interface
// -------------
tmElements_t tm;

char longdate[25] = "Jan  1, 2017 12:00:00 AM";
char pH1_char[7] = "00.00";
char pH2_char[7] = "00.00";
char Temp_char[8]= "";
char RH_char[7] = "00.00%";
char TDS1_char[11] = "0000.0ppm";
char TDS2_char[11] = "0000.0ppm";
char CO2_char[8] = "00.00%";
char Light_char[8] = "00.00%";

//Committed Light Settings
int Light_ON_hour = 18;
int Light_ON_min = 0;
int Light_OFF_hour = 6;
int Light_OFF_min = 0;

// Committed Pump Schedule Settings
// -------------------------------
int Pump_start_hour = 7;
int Pump_start_minute = 15;
int Pump_start_isAM = true;
int Pump_every_hours = 1;
int Pump_every_mins = 0;
int Pump_for = 15; //In Minutes
int Pump_repeat = 12; //Number of times per day

// Allow for pump schedules (Set in E2P_Map.h)
int Pump_hour_array[MAXREPEAT];
int Pump_minute_array[MAXREPEAT];
int Pump_isAM_array[MAXREPEAT];

/*
  SENSOR VARIABLES
 */
//Values the arduino sees
int pH1RawValue = 0;
int pH2RawValue = 0;
int TempRawValue = 0;
int RHRawValue = 0;
int TDS1RawValue = 0;
int TDS2RawValue = 0;
int CO2RawValue = 0;
int LightRawValue = 0;

//Values the humans see (after it has been calculated using raw values above ^^^ and their respected formulas)
float pH1Value = 0;
float pH2Value = 0;
float TempValue = 0;
float RHValue = 0;
float TDS1Value = 0;
float TDS2Value = 0;
float CO2Value = 0;
float LightValue = 0;

//PH
//Status Indicator Values
char  pH1_Status[4] = "OK";
float pH1Value_Low = 5.60;
float pH1Value_High = 6.10;
char  pH2_Status[4] = "OK";
float pH2Value_Low = 5.60;
float pH2Value_High = 6.10;

//Temp
char  Temp_Status[4] = "OK";
float TempValue_Low = 5.00;
float TempValue_High = 21.00;
//Accessory ON/OFF Values
float Heater_ON = 5.00;
float Heater_OFF = 21.00;
float AC_ON = 29.00;
float AC_OFF = 21.00;

//Humidty(RH)
char  RH_Status[4] = "OK";
float RHValue_Low = 30.00;
float RHValue_High = 80.00;
//Accessory ON/OFF Values
float Humidifier_ON = 40.00;
float Humidifier_OFF = 60.00;
float Dehumidifier_ON = 80.00;
float Dehumidifier_OFF = 60.00;

//TDS1
char  TDS1_Status[4] = "OK";
float TDS1Value_Low = 500.0;
float TDS1Value_High = 800.0;
//Accessory ON/OFF Values
float NutePump1_ON = 500.0;
float NutePump1_OFF = 800.0;
boolean MixPump1_Enabled = true;

//TDS2
char  TDS2_Status[4] = "OK";
float TDS2Value_Low = 500.0;
float TDS2Value_High = 800.0;
//Accessory ON/OFF Values
float NutePump2_ON = 500.0;
float NutePump2_OFF = 800.0;
boolean MixPump2_Enabled = true;

//CO2
char  CO2_Status[4] = "OK";
float CO2Value_Low = 60.00;
float CO2Value_High= 75.00;
//Accessory ON/OFF Values
float CO2_ON = 40.00;
float CO2_OFF = 60.00 ;
boolean CO2_Enabled = true;

//Light
char  Light_Status[4] = "OK";
float LightValue_Low = 20.00;
float LightValue_High = 95.00;


/*
 PIN DEFINITIONS
 */
 
 // Analog pins
 // ===========
int CO2Pin = A0;
int LightPin = A1;
int pH1Pin = A2;
int pH2Pin = A2;
int TDS1Pin = A3;
int TDS2Pin = A3;
#define MAXINPUTS 5
int Input_Pin[MAXINPUTS] = { A0, A1, A2, A3, A4 };

// RH - DHT22 device for temperature and humidity
#define DHTPIN 7
dht DHT;

// EC Probe Related
int ECAin=A6;                   // Probe A Sense Pin
int ECBin=A7;                   // Probe B Sense Pin 
int ECA=8;                      // Probe A Drive Pin
int ECB=9;                      // Probe B Drive Pin

// EC Temp Probe Related
#define ONE_WIRE_BUS 10         // Data wire For Temp Probe is plugged into pin 10 on the Arduino
OneWire oneWire(ONE_WIRE_BUS);  // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature

// Realtime Clock I2C - SDA/SCL
DS1307RTC rtc;

// LCD Display Related 
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// Digital pins
// ============
// Relays, SSR
// -----------
// Currently 6 relays
// MAXRELAYS set in E2P_Map.h
// Relays 5, 6 are SSR polarity

// Relay1_Pin = 2;  // Water Pump
// Relay2_Pin = 3;  // Nutrient
// Relay3_Pin = 4;  // Humidity
// Relay4_Pin = 5;  // CO2
// Relay5_Pin = 6;  //SSR (Light/Ballast)
// Relay6_Pin = 12; //SSR (Heater)

#define ON 1
#define OFF 0

#define WATERPUMP1 0
// #define WP2 1
#define NUTRIENT 1
#define HUMIDITY 2
#define pHADJUST 3
#define SSRLIGHT 4
#define SSRHEAT 5

int Relay_Pin[MAXRELAYS] = { 2, 3, 4, 5, 6, 11, 12, A15 };
uint8_t Relay_State[MAXRELAYS];
uint8_t Relay_isAuto[MAXRELAYS];

#define STAT 13      // Status LED on expansion board

// Utility functions
// =================
boolean UIToStr(char *str, unsigned int val, uint8_t len, uint8_t dp)
{
  if (len > 5) return false;
  if (dp > len) return false;
  str += len;          // Point to end of string
  if (dp) str += 1;    //  add '.' if required
  *str = '\0';         // NULL terminate

  while (len--)
  {
    *--str = (val % 10) + '0';
    val /= 10;
    if ((--dp) == 0) *--str = '.'; 
  }
  return true;
}
boolean uint8ToStr(char *str, uint8_t val, uint8_t len, uint8_t dp)
{
  if (len > 3) return false;
  if (dp > len) return false;
  str += len;          // Point to end of string
  if (dp) str += 1;    //  add '.' if required
  *str = '\0';         // NULL terminate

  while (len--)
  {
    *--str = (val % 10) + '0';
    val /= 10;
    if ((--dp) == 0) *--str = '.'; 
  }
  return true;
}
/*
 SETUP
 */
void setup()
{
  int i;
  // Setup serial for boot reporting
  Serial.begin(9600);
  // SetuLCD for display
  //lcd.setBacklightPin ( BACKLIGHT_PIN, POSITIVE );
  //lcd.setBacklight ( HIGH );
  lcd.backlight();
  lcd.begin(20,4);               // initialize the lcd 
  lcd.home ();                   // go home
  
  Serial.println("HH Vers V17.0202");
  lcd.setCursor ( 0, 2 );        // go to the third line
  lcd.print("HH Vers V17.0202"); 
//  delay(50);    // Let user read, and wait for EEPROM  

  //Set Pin Modes for Inputs
  for (i = 0; i < MAXINPUTS; i++) {
    pinMode(Relay_Pin[i], INPUT);
  }

  // Setup EC Probe fir Input/Output
  EC_setup();
  
  //Set Pin Modes for Relays
  for (i = 0; i < MAXRELAYS; i++) {
    pinMode(Relay_Pin[i], OUTPUT);
  }
  pinMode(STAT, OUTPUT);
  
  // Read RTC
  // --------
  RestoreRTC();
  updatelongdate();
//  printtime();

//  Serial.println("Reading EEPROM into memory");
  // Load (and crc check) the E2P region values
  // If the region crc is wrong, load defaults

//  dumpcrc();
//  printE2PValues();
  lcd.setCursor ( 0, 3 );        // go to the bottom line  
  i = loadE2PValues();
  if (i != 0) {
    Serial.print("crc error count - ");
    Serial.println(i, DEC);
    lcd.print("EEPROM crc error");
    Serial.println(" Errors in EEPROM stored Values - enable print for details");
    dumpcrc();
    Serial.println("Defaults restored for values with errors");
    saveE2PValues();
    Serial.println("New values saved");
    dumpcrc();
    }
    else {
      lcd.print("EEPROM values loaded");
    }
    delay(1000);
}


/*
 LOOP
 */
 
void loop()
{
  char Month_array[12][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
  int msgcount = 1;  // For sensor/relay messages
  int lcdcount = 1;  // For LCD Update
  
//  sendAllMessages();
  while (1) {
    if (msgcount > MSG_INTERVAL) {
      digitalWrite(STAT, HIGH);
      updateSensorValues();
      sendserialmessages();
      CheckTimers();
      CheckSetPoints();
      digitalWrite(STAT, LOW);
      msgcount = 0;
    }
    // Show on display
    if (lcdcount == 1) {
//      lcd.clear();
      lcd.setCursor ( 0, 0 );        // go to the top line
      if (hour() < 10) lcd.print(" ");
      lcd.print (hour());
      lcd.print(":");
      if (minute() < 10) lcd.print("0");
      lcd.print (minute());
      lcd.print(".");
      if (second() < 10) lcd.print("0");
      lcd.print (second());

//      lcd.setCursor ( 0, 1 );        // go to the bottom line
//      lcd.print("/");
//      lcd.print (year() % 2000);
//      if (month() < 10) lcd.print(" ");
      lcd.print("  ");
      lcd.print (Month_array[month()-1]);
      lcd.print(" ");
//      if (day() < 10) lcd.print(" ");
      lcd.print (day());
      
      lcd.setCursor ( 0, 1 );        // go to the bottom line
      lcd.print("pH ");
      lcd.print (pH1Value);
      lcd.setCursor ( 8, 1 );
      lcd.print("Lux ");
      lcd.print(LightValue);

    }
    if (lcdcount == LCD_INTERVAL) {
//      lcd.clear();
      lcd.setCursor ( 0, 0 );        // go to the top line
      lcd.print("RH ");
      lcd.print(RHValue);
      lcd.setCursor ( 8, 0 );
      lcd.print(" T ");
      lcd.print (TempValue);
      lcd.setCursor ( 0, 1 );        // go to the bottom line
      lcd.print("pp ");
      lcd.print (TDS1Value);
      lcd.setCursor ( 8, 1 );
      lcd.print(" T ");
      lcd.print(ECTemperatureFinish);
    }
    serialEvent();
    msgcount += 1;
    lcdcount += 1;
    if (lcdcount > LCD_INTERVAL*2) lcdcount = 0;
    delay(50);
  }
}

void printtime(void) {
  Serial.print(longdate);
  Serial.println();
}









