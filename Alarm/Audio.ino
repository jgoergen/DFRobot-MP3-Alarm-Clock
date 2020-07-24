bool playing = false;

void Audio_Init() {
  
  pinMode(MP3_RX_PIN, INPUT);
  pinMode(MP3_TX_PIN, OUTPUT);
  mySoftwareSerial.begin(9600);
  
  if (!myDFPlayer.begin(mySoftwareSerial, true, false))
    Display_PrintText("MP3 =(");
    
  Audio_SetVolume(ALARM_VOLUME);
}

void Audio_SetVolume(int val) {

  myDFPlayer.volume(val);
}

void Audio_Update() {

  if (myDFPlayer.available())
    printDetail(myDFPlayer.readType(), myDFPlayer.read());
}

void Audio_PlayRandomSound() {
      
  playing = true;
  myDFPlayer.play(int(random(1, MAX_SOUND_INDEX)));

  delay(250);
  
  while(playing) {

    //runRandomVisualizer();
    for (int i = 0; i < 200; i ++)
      Display_RandomLights();
      
    Audio_Update();
  }

  Clock_ForceFullRefresh();
}

void printDetail(uint8_t type, int value){

  switch (type) {
    case TimeOut:
      //Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      //Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      //Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      //Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      //Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      //Serial.print(F("Number:"));
      //Serial.print(value);
      //Serial.println(F(" Play Finished!"));
      playing = false;
      break;
    case DFPlayerError:
      //Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          //Serial.println(F("Card not found"));
          break;
        case Sleeping:
          //Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          //Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          //Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          //Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          //Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          //Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
