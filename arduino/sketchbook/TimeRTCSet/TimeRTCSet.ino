/*
 * TimeRTCSet.pde
 * example code illustrating Time library with Real Time Clock.
 *
 * RTC clock is set in response to serial port time message 
 * A Processing example sketch to set the time is included in the download
 * On Linux, you can use "date +T%s > /dev/ttyACM0" (UTC time zone)
 */

#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#define TZ_adjust 10
#define TIME_MSG_LEN 11

void setup()  {
  Serial.begin(9600);
  while (!Serial) ; // Needed for Leonardo only
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if (timeStatus() != timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");      
}

void loop()
{
  unsigned long t1 = 0L;
  if (Serial.available()) {
   digitalClockDisplay();
   Serial.println("Trying to sync with the RTC");
   t1 = processSyncMessage();
   Serial.println(" ");
   Serial.print("From SR : ");   
   Serial.print(t1);
   Serial.println(" ");
   time_t t = t1; 
   if (t != 0) {
      Serial.println("Setting the RTC");
      RTC.set(t);   // set the RTC and the system time to the received value
      digitalClockDisplay();
      Serial.println("Getting the RTC");
      setSyncProvider(RTC.get);   // the function to get the time from the RTC
      digitalClockDisplay();
      Serial.println("Setting the Arduino time");
      setTime(t);          
      digitalClockDisplay();
    }
  }
  digitalClockDisplay();  
  delay(1000);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

/*  code to process time sync messages from the serial port   */
#define TIME_HEADER  "T"   // Header tag for serial time sync message

unsigned long processSyncMessage() {
  unsigned long pctime = 0L;
  unsigned long pcadjust = 60L * 60l * TZ_adjust;
  char c;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 

  if(Serial.find(TIME_HEADER)) {
    
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        c = Serial.read();          
        Serial.write(c);          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
 
 //     pctime = Serial.parseInt();
     Serial.println(" ");
     Serial.print("From PC : ");   
     Serial.print(pctime);
     Serial.println(" ");
     Serial.print(" adjust : ");   
     Serial.print(pcadjust);
     Serial.println(" ");
     pctime += pcadjust;      // Adjust for local time
     Serial.print("Adjusted: ");     
     Serial.println(pctime);          
     Serial.println(" ");
//     if( pctime < DEFAULT_TIME) { // check the value is a valid time (greater than Jan 1 2013)
//       pctime = 0L; // return 0 to indicate that the time is not valid
//     }
    return pctime;
  }
  return pctime;
}





