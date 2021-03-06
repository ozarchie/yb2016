/*
/!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 /!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!
 /!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

//READ ALL SENSOR VALUES AND CONVERT FOR LCD DISPLAY
void updateSensorValues() {
 
  analogReference(DEFAULT);  //Seems more accurate.
  
 /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !!PH SENSORS!!PH SENSORS!!PH SENSORS!!PH SENSORS!!PH SENSORS!!PH SENSORS!!PH SENSORS!!PH SENSORS!!PH SENSORS!!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  /*PH1------------------------------------------------*/
  float pH1Sum = 0;
  int j = 0;
  analogRead(pH1Pin);  //Get ADC to switch to correct pin
  delay(20); //Wait for Pin to Change
  
  while(j<30) {
    pH1Sum = pH1Sum + analogRead(pH1Pin);
    j++;
  }
  pH1RawValue = pH1Sum/30;

  pH1Value = (0.0178 * pH1RawValue - 1.889);
  if(isnan(pH1Value)){
    pH1Value = 0;        
  }

  UIToStr(pH1_char, pH1Value, 5, 1);
  strcat(pH1_char,"ppm");

  /*PH2------------------------------------------------*/

  float pH2Sum = 0;
  j = 0;
  analogRead(pH2Pin);  //Get ADC to switch to correct pin
  delay(15); //Wait for Pin to Change

  while(j<30) {
    pH2Sum = pH2Sum + analogRead(pH2Pin);
    j++;
  }
  pH2RawValue = pH2Sum/30;

  pH2Value = (0.0178 * pH2RawValue - 1.889);
  if(isnan(pH2Value)){
    pH2Value = 0;        
  }

  UIToStr(pH2_char, pH2Value, 5, 1); 
  strcat(pH1_char,"ppm");

  
 /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !!TEMPERATURE SENSOR!!TEMPERATURE SENSOR!!TEMPERATURE SENSOR!!TEMPERATURE SENSOR!!TEMPERATURE SENSOR!!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

/* jma Use DHT for temperature

  float TempSum = 0;
  j = 0;
  analogRead(TempPin);  //Get ADC to switch to correct pin
  delay(15); //Wait for Pin to Change

  while(j<10) {
    TempSum = TempSum + analogRead(TempPin);
    j++;
  }
  TempRawValue = TempSum/((j-1) * 2);
  TempValue = ((5.00 * TempRawValue * 100.0)/1024.0) + 8.50;
*/
  
  DHT.read22(DHTPIN);  //Use updated DHT library for hum, temp
//  delay(15); //Wait for Pin to Change
  
  TempValue=DHT.temperature; //
  if(isnan(TempValue)){
    TempValue = 0;        
  }
  
  UIToStr(Temp_char, TempValue, 5, 2);
  strcat(Temp_char," C"); 
  
  
 /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !!HUMIDITY DHT11 SENSOR!!HUMIDITY DHT11 SENSOR!!HUMIDITY DHT11 SENSOR!!HUMIDITY DHT11 SENSOR!!HUMIDITY DHT11!!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  //       float RHSum = 0;
  //       j = 0;
  //       
  //       while(j<30) {
  //       RHSum = RHSum + ;
  //       j++;
  //       delay(10);
  //       }
  //       RHRawValue = RHSum/30;
  
  RHValue = DHT.humidity;
  if (isnan(RHValue)) {
    RHValue = 0;
  } 
  
  UIToStr(RH_char, RHValue, 5, 2);
  if (RHValue < 10){
    strcat(RH_char,"% ");
  } 
  else {
    strcat(RH_char,"%");
  }
  
 /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !!TDS1 SENSOR!!TDS1 SENSOR!!TDS1 SENSOR!!TDS1 SENSOR!!TDS1 SENSOR!!TDS1 SENSOR!!TDS1 SENSOR!!TDS1 SENSOR!!TDS1 SENSOR!!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  float TDS1Sum = 0;
  j = 0;
  analogRead(TDS1Pin);  //Get ADC to switch to correct pin
  delay(15); //Wait for Pin to Change

  while(j<10) {
    TDS1Sum = TDS1Sum + analogRead(TDS1Pin);
    j++;
  }

  TDS1RawValue = TDS1Sum/((j-1) * 2);

  TDS1Value = ((TDS1RawValue * 1000.0)/1024.0);
  if(isnan(TDS1Value)){
    TDS1Value = 0;        
  }
  UIToStr(TDS1_char, TDS1Value, 5, 2);
  strcat(TDS1_char," ppm");    
  
  
 /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !!TDS2 SENSOR!!TDS2 SENSOR!!TDS2 SENSOR!!TDS2 SENSOR!!TDS2 SENSOR!!TDS2 SENSOR!!TDS2 SENSOR!!TDS2 SENSOR!!TDS2 SENSOR!!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

  float TDS2Sum = 0;
  j = 0;
  analogRead(TDS2Pin);  //Get ADC to switch to correct pin
  delay(15); //Wait for Pin to Change

  while(j<10) {
    TDS2Sum = TDS2Sum + analogRead(TDS2Pin);
    j++;
  }
  TDS2RawValue = TDS2Sum/((j-1) * 2);

  TDS2Value = ((TDS2RawValue * 1000.0)/1024.0);
  if(isnan(TDS2Value)){
    TDS2Value = 0;        
  }
  UIToStr(TDS2_char, TDS2Value, 5, 2);
  strcat(TDS2_char," ppm");   
  
    
 /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !!CO2 SENSOR!!CO2 SENSOR!!CO2 SENSOR!!CO2 SENSOR!!CO2 SENSOR!!CO2 SENSOR!!CO2 SENSOR!!CO2 SENSOR!!CO2 SENSOR!!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

  float CO2Sum = 0;
  j = 0;
  analogRead(CO2Pin);  //Get ADC to switch to correct pin
  delay(15); //Wait for Pin to Change

  while(j<10) {
    CO2Sum = CO2Sum + analogRead(CO2Pin);
    j++;
  }
  CO2RawValue = CO2Sum/((j-1) * 2);
  CO2Value = ((CO2RawValue * 100.0)/1024.0);
  
  if(isnan(CO2Value)){
    CO2Value = 0;        
  }
  UIToStr(CO2_char, CO2Value, 5, 2);
  strcat(CO2_char," ppm");   
 
  
 /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !!LIGHT SENSOR!!LIGHT SENSOR!!LIGHT SENSOR!!LIGHT SENSOR!!LIGHT SENSOR!!LIGHT SENSOR!!LIGHT SENSOR!!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

  float LightSum = 0;
  j = 0;
  analogRead(LightPin);  //Get ADC to switch to correct pin
  delay(10); //Wait for Pin to Change
  
  while(j<10) {
    LightSum = LightSum + analogRead(LightPin);
    j++;
  }

  LightRawValue = LightSum/10;

  LightValue = ((LightRawValue * 100.0) / 1024.0);
  
  if (isnan(LightValue)) {
    LightValue = 0;
  } 
  UIToStr(Light_char, LightValue, 5, 2);
  if (LightValue < 10){
    strcat(Light_char,"% ");
  } 
  else {
    strcat(Light_char,"%");
  }
  
  //pH
  if (pH1Value < pH1Value_Low) {
    strcpy(pH1_Status,"LOW");
  } 
  else if (pH1Value > pH1Value_Low && pH1Value < pH1Value_High) {
    strcpy(pH1_Status,"OK");
  } 
  else if (pH1Value > pH1Value_High) {
    strcpy(pH1_Status,"HIGH");
  }


  if (pH2Value < pH2Value_Low) {
    strcpy(pH2_Status,"LOW");
  } 
  else if (pH2Value > pH2Value_Low && pH2Value < pH2Value_High) {
    strcpy(pH2_Status,"OK");
  } 
  else if (pH2Value > pH2Value_High) {
    strcpy(pH2_Status,"HIGH");
  }

  //Temp
  if (TempValue < TempValue_Low) {
    strcpy(Temp_Status,"LOW");
  } 
  else if (TempValue > TempValue_Low && TempValue < TempValue_High) {
    strcpy(Temp_Status,"OK");
  } 
  else if (TempValue > TempValue_High) {
    strcpy(Temp_Status,"HIGH");
  }
  //RH
  if (RHValue < RHValue_Low) {
    strcpy(RH_Status,"LOW");
  } 
  else if (RHValue > RHValue_Low && RHValue < RHValue_High) {
    strcpy(RH_Status,"OK");
  }
  else if (RHValue < RHValue_High) {
    strcpy(RH_Status,"HIGH");
  }
  //TDS1
  if (TDS1Value < TDS1Value_Low) {
    strcpy(TDS1_Status,"LOW");
  } 
  else if (TDS1Value > TDS1Value_Low && TDS1Value < TDS1Value_High) {
    strcpy(TDS1_Status,"OK");
  } 
  else if (TDS1Value < TDS1Value_High) {
    strcpy(TDS1_Status,"HIGH");
  }
  
  //TDS2
  if (TDS2Value < TDS2Value_Low) {
    strcpy(TDS2_Status,"LOW");
  } 
  else if (TDS2Value > TDS2Value_Low && TDS2Value < TDS2Value_High) {
    strcpy(TDS2_Status,"OK");
  } 
  else if (TDS2Value < TDS2Value_High) {
    strcpy(TDS2_Status,"HIGH");
  }
  
  //CO2
  if (CO2Value < CO2Value_Low) {
    strcpy(CO2_Status,"LOW");
  } 
  else if (CO2Value > CO2Value_Low && CO2Value < CO2Value_High) {
    strcpy(CO2_Status,"OK");
  } 
  else if (CO2Value > CO2Value_High){
    strcpy(CO2_Status,"HIGH");
  }
  //Light
  if (LightValue < LightValue_Low) {
    strcpy(Light_Status,"LOW");
  } 
  else if (LightValue > LightValue_Low && LightValue < LightValue_High) {
    strcpy(Light_Status,"OK");
  } 
  else if (LightValue < LightValue_High) {
    strcpy(Light_Status,"HIGH");
  }
}
