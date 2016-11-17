/*
/!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 /!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!turnRelay!!
 /!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
void turnRelay(int Relay_num, int TurnOnOff){

  if (Relay_num < 6) {
    //Relay Board works opposite (Low/High) (Relays 1 - 5)
    if (TurnOnOff == 1) digitalWrite(Relay_Pin[Relay_num - 1], LOW);
    else if (TurnOnOff == 0) digitalWrite(Relay_Pin[Relay_num - 1], HIGH);      
  } 
  else if (Relay_num == 6) {
    //SSR relay works as expected (High/Low) (Relay 6)
    if (TurnOnOff == 1) digitalWrite(Relay_Pin[Relay_num - 1], HIGH);
    else if (TurnOnOff == 0) digitalWrite(Relay_Pin[Relay_num - 1], LOW);      
  }
//  sendRelayMessages(); // (through serial)
}

void FillPumpTimesArrays(int Pump_start_hour, int Pump_start_min, int Pump_every_hours, int Pump_every_mins, int Pump_for, int Pump_repeat) {
  Pump_hour_array[0] = Pump_start_hour;
  Pump_minute_array[0] = Pump_start_minute;
  Pump_isAM_array[0] = Pump_start_isAM;

  if (Pump_start_hour > 12) {
    Pump_isAM_array[0] = false;
  }

  int i = 0;
  for(i = 0; i < Pump_repeat; i++){
    Pump_hour_array[i] = Pump_start_hour + ((i) * Pump_every_hours);
    Pump_minute_array[i] = Pump_start_minute + ((i) * Pump_every_mins);
    while (Pump_minute_array[i] > 59) {
      Pump_minute_array[i] = Pump_minute_array[i] - 60;
      Pump_hour_array[i]++;
    }
    int AMPM_int = Pump_hour_array[i] / 12;
    while (Pump_hour_array[i] > 12) {
      Pump_hour_array[i] = Pump_hour_array[i] - 12;
    }
/*
    Serial.println();
    Serial.print("Pump_start_isAM:");       
    Serial.print(Pump_start_isAM);       
    Serial.println();
    Serial.print(Pump_hour_array[i]);
    Serial.print(") ");
    Serial.print(AMPM_int);
    Serial.println();
*/
    boolean case_zero = false;
    switch (AMPM_int) {
    case 0:
      case_zero = true;
      if (Pump_start_isAM == true) {
        Pump_isAM_array[i] = true;
      } 
      else {
        Pump_isAM_array[i] = false;
      }
      break;
    case 1:
      if (case_zero == true){
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = false;
        } 
        else {
          Pump_isAM_array[i] = true;
        }
      } 
      else {
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = false;
        } 
        else {
          Pump_isAM_array[i] = true;
        }
      }
      break;
    case 2:
      if (case_zero == true){
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = false;
        } 
        else {
          Pump_isAM_array[i] = true;
        }
      } 
      else {
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = true;
        } 
        else {
          Pump_isAM_array[i] = false;
        }
      }
      break;
    case 3:
      if (case_zero == true){
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = true;
        } 
        else {
          Pump_isAM_array[i] = false;
        }
      } 
      else {
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = false;
        } 
        else {
          Pump_isAM_array[i] = true;
        }
      }
      break;
    case 4:
      if (case_zero == true){
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = false;
        } 
        else {
          Pump_isAM_array[i] = true;
        }
      } 
      else {
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = true;
        } 
        else {
          Pump_isAM_array[i] = false;
        }
      }
      break;
    }
  }
}

