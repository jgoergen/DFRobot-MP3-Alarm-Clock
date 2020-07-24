long offsetSeconds = 0;
int currentSeconds = 0;
int currentMinutes = 0;
int currentHours = 0;
int setMinutes = 0;
int setHours = 0;
int alarmMinutes = 0;
int alarmHours = 0;
long lastClockUpdate = 0;
long milliseconds = 0;
bool refreshNeeded = false;
bool forcedRefreshNeeded = false;
byte clockMode = 0;
byte entryIndex = 0;
bool alarmSet = false;

void Clock_Init() {

  startRegularClockMode();
}

void Clock_Update() {

  milliseconds = millis();

  if (milliseconds - lastClockUpdate > 1000) {

    lastClockUpdate = milliseconds;
    updateTime();
    
    switch(clockMode) {

      case 0:

        // regular clock mode
        updateRegularClockMode();
        break;

      case 1:

        // set clock time
        updateSetClockMode();
        break;

      case 2:

        // set alarm time
        updateSetAlarmMode();
        break;

      case 3:

        // alarm going off
        Audio_PlayRandomSound();
        delay(1000);
        break;
    }
  }
}

void updateTime() {

  currentSeconds ++;

  if (currentSeconds > 59) {

    currentSeconds = 0;
    currentMinutes ++;
    forcedRefreshNeeded = true;
  }

  if (currentMinutes > 59) {

    currentMinutes = 0;
    currentHours ++;
    forcedRefreshNeeded = true;
  }

  if (currentHours > 24) {

    currentHours = 0;
    forcedRefreshNeeded = true;
  }
}

void startRegularClockMode() {

  mx.clear();
  clockMode = 0;
  forcedRefreshNeeded = true;
}

void updateRegularClockMode() {

  if (alarmSet == true && currentHours == alarmHours && currentMinutes == alarmMinutes && currentSeconds < 2) {

    clockMode = 3;
  }

  Display_ShowTime(currentSeconds, currentMinutes, currentHours, forcedRefreshNeeded);

  if (forcedRefreshNeeded)
    forcedRefreshNeeded = false;
}

void startSetClockMode() {

  mx.clear();
  Display_PrintText("Time?");
  delay(400);
  clockMode = 1;
  entryIndex = 0;
  setMinutes = 0;
  setHours = 0;
  forcedRefreshNeeded = true;
}

void updateSetClockMode() {
  
  Display_ShowTime(0, setMinutes, setHours, forcedRefreshNeeded);  

  
  if (forcedRefreshNeeded)
    forcedRefreshNeeded = false;
}

void startSetAlarmMode() {

  mx.clear();
  Display_PrintText("Alarm?");
  delay(400);
  clockMode = 2;
  entryIndex = 0;
  setMinutes = 0;
  setHours = 0;
  forcedRefreshNeeded = true;
}

void updateSetAlarmMode() {
  
  Display_ShowTime(0, setMinutes, setHours, forcedRefreshNeeded);  
  
  if (forcedRefreshNeeded)
    forcedRefreshNeeded = false;
}

void Clock_ForceFullRefresh() {

  forcedRefreshNeeded = true;
}

void Clock_HandleKey(char key) {

  int keyVal = getIntFromKey(key);

  switch(clockMode) {

    case 0:
    
      // regular clock mode
      if (key == 'A')
        startSetClockMode();

      if (key == 'B')
        startSetAlarmMode();

      if (key == 'D')
        Audio_PlayRandomSound();
        
      break;

    case 1:

      if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
        
        startRegularClockMode();
        return;
      }

      if (key == '*') {

        if (entryIndex > 0) {

          switch(entryIndex) {

            case 1:
              
              // remove 
              setMinutes = 0;
              setHours = 0;
              break;

            case 2:
              setMinutes = 0;
              setHours = int(float(setHours) / 10.0) * 10;
              break;

            case 3:
              setMinutes = 0;
              break;
          }
          
          forcedRefreshNeeded = true;
          entryIndex --;
        }
      }

      refreshNeeded = false;

      if (keyVal < 10) {
      
        // set clock mode
          switch(entryIndex) {
  
            case 0:
  
              if (keyVal > 2)
                return;
  
              forcedRefreshNeeded = true;
              setHours = keyVal * 10;
              entryIndex ++;
              break;
  
            case 1:
  
              if (keyVal > 4)
                return;
                
              forcedRefreshNeeded = true;
              setHours += keyVal;
              entryIndex ++;
              break;
  
            case 2:
  
              if (keyVal > 5)
                return;
  
              forcedRefreshNeeded = true;
              setMinutes = keyVal * 10;
              entryIndex ++;
              break;
  
            case 3:
  
              forcedRefreshNeeded = true;
              setMinutes += keyVal;
              currentSeconds = 0;
              currentMinutes = setMinutes;
              currentHours = setHours;
              startRegularClockMode();
              break;
          }
      }
      break;

    case 2:

      if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
        
        startRegularClockMode();
        return;
      }
    
      // set alarm mode
      if (key == '*') {

        if (entryIndex > 0) {

          switch(entryIndex) {

            case 1:
              
              // remove 
              setMinutes = 0;
              setHours = 0;
              break;

            case 2:
              setMinutes = 0;
              setHours = int(float(setHours) / 10.0) * 10;
              break;

            case 3:
              setMinutes = 0;
              break;
          }
          
          forcedRefreshNeeded = true;
          entryIndex --;
        }
      }

      refreshNeeded = false;

      if (keyVal < 10) {
      
        // set clock mode
          switch(entryIndex) {
  
            case 0:
  
              if (keyVal > 2)
                return;
  
              forcedRefreshNeeded = true;
              setHours = keyVal * 10;
              entryIndex ++;
              break;
  
            case 1:
  
              if (keyVal > 4)
                return;
                
              forcedRefreshNeeded = true;
              setHours += keyVal;
              entryIndex ++;
              break;
  
            case 2:
  
              if (keyVal > 5)
                return;
  
              forcedRefreshNeeded = true;
              setMinutes = keyVal * 10;
              entryIndex ++;
              break;
  
            case 3:
  
              forcedRefreshNeeded = true;
              setMinutes += keyVal;
              alarmMinutes = setMinutes;
              alarmHours = setHours;
              alarmSet = true;
              startRegularClockMode();
              break;
          }
      }
      break;

      case 3:

        // alarm ringing mode
        clockMode = 0;
        break;
  }
}

int getIntFromKey(char key) {

  switch(key) {

    case '0':
      return 0;
      break;

    case '1':
      return 1;
      break;

    case '2':
      return 2;
      break;

    case '3':
      return 3;
      break;

    case '4':
      return 4;
      break;

    case '5':
      return 5;
      break;

    case '6':
      return 6;
      break;

    case '7':
      return 7;
      break;

    case '8':
      return 8;
      break;

    case '9':
      return 9;
      break;

    default:
      return 99;
      break;
  }
}
