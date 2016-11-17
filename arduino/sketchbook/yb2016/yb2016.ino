#include <dht.h>   // Latest DHTnn library
#include <Time.h>
#include <EEPROM.h>
#include "E2P_Map.h"
#include <Wire.h>
#include <DS1307RTC.h>
#include <avr/pgmspace.h>
#include "Arduino.h"

/*
/!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 /!!VARIABLES!!VARIABLES!!VARIABLES!!VARIABLES!!VARIABLES!!VARIABLES!!VARIABLES!!VARIABLES!!VARIABLES!!
 /!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

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

char longdate[25] = "Nov  1, 2016 12:00:00 AM";
char pH1_char[7] = "00.00";
char pH2_char[7] = "00.00";
char Temp_char[8]= "";
char RH_char[7] = "00.00%";
char TDS1_char[11] = "0000.0ppm";
char TDS2_char[11] = "0000.0ppm";
char CO2_char[8] = "00.00%";
char Light_char[8] = "00.00%";

//Committed Light Settings
uint8_t Light_ON_hour = 18;
uint8_t Light_ON_min = 0;
uint8_t Light_OFF_hour = 6;
uint8_t Light_OFF_min = 0;

// Committed Pump Schedule Settings
// -------------------------------
uint8_t Pump_start_hour = 7;
uint8_t Pump_start_minute = 15;
boolean Pump_start_isAM = true;
uint8_t Pump_every_hours = 2;
uint8_t Pump_every_mins = 0;
uint8_t Pump_for = 15; //In Minutes
uint8_t Pump_repeat = 1; //Number of times per day

// Allow for pump schedules (Set in E2P_Map.h)
uint8_t Pump_hour_array[MAXREPEAT];
uint8_t Pump_minute_array[MAXREPEAT];
boolean Pump_isAM_array[MAXREPEAT];

/*
/!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 /!!SENSOR VARIABLES!!SENSOR VARIABLES!!SENSOR VARIABLES!!SENSOR VARIABLES!!SENSOR VARIABLES!!SENSOR VARIABLES!!
 /!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
float TempValue_Low = 21.00;
float TempValue_High = 29.00;
//Accessory ON/OFF Values
float Heater_ON = 21.00;
float Heater_OFF = 25.00;
float AC_ON = 29.00;
float AC_OFF = 25.00;

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
/!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 /!!PIN DEFINITIONS!!PIN DEFINITIONS!!PIN DEFINITIONS!!PIN DEFINITIONS!!PIN DEFINITIONS!!PIN DEFINITIONS!!
 /!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
 
 // Analog pins
 // ===========
int CO2Pin = A0;
int LightPin = A1;
int pH1Pin = A2;
int pH2Pin = A2;
int TDS1Pin = A3;
int TDS2Pin = A3;
int DHTSDA = A4;
int DHTSCL = A5;
//RH - DHT device for temperature and humidity
#define DHTPIN 2
dht DHT;

// Realtime Clock
DS1307RTC rtc;

// Digital pins
// ============
// Relays, SSR
// -----------
// Currently 6 relays
// MAXRELAYS set in E2P_Map.h
// Only relay 6 is SSR

// Relay1_Pin = 3;  //Water Pump 1
// Relay2_Pin = 4;  //Water Pump 2
// Relay3_Pin = 5;  //Free
// Relay4_Pin = 6;  //Free
// Relay5_Pin = 7;  //Free
// Relay6_Pin = 8;  //SSR (Light/Ballast)
#define SSR 5
#define WP1 0
#define WP2 1

uint8_t Relay_Pin[MAXRELAYS] = { 3, 4, 5, 6, 7, 8 };
uint8_t Relay_State[MAXRELAYS];
uint8_t Relay_isAuto[MAXRELAYS];

#define STAT 13      // Status LED on expansion

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
/!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 /!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!SETUP!!
 /!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
void setup()
{
  int i;
  // Setup serial for boot reporting
  Serial.begin(9600);
  Serial.println("YB Firmware V2.3");
  delay(500);    // Let user read, and wait for EEPROM  

  //Set Pin Modes for Relays
  for (i = 0; i < 6; i++) {
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
  i = loadE2PValues();
  if (i != 0) {
    Serial.print(i, DEC);
    Serial.println(" Errors in EEPROM stored Values - enable print for details");
    Serial.println("Defaults restored for values with errors");
    saveE2PValues();
    Serial.println("New values saved");
  }
  else {
//    Serial.println("EEPROM values loaded");
  }
//  printE2PValues();

}


/*
/!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 /!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!LOOP!!
 /!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
void loop()
{
    int  loopcount = 0;
//  Serial.println("loop"); 
  while (1) {
    if (loopcount > 50) {
      digitalWrite(STAT, HIGH);
      updateSensorValues();
      sendserialmessages();
      CheckTimers();
      CheckSetPoints();
      digitalWrite(STAT, LOW);
      loopcount = 0;
    }
    serialEvent();
    loopcount +=1;
    delay(50);
  }
}

void printtime(void) {
  Serial.print(longdate);
  Serial.println();
}









