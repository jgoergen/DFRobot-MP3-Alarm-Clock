char outputBuffer[20]; 

void Display_Init() {

  pinMode(MAX7219_CLK_PIN,OUTPUT);
  pinMode(MAX7219_CS_PIN,OUTPUT);
  pinMode(MAX7219_DIN_PIN,OUTPUT);  
  mx.begin();
  mx.clear();
}

void Display_ShowTime(int sec, int min, int hour, bool fullRefresh) {

  if (fullRefresh) {
    
    memset(outputBuffer, 0, sizeof outputBuffer);
    sprintf(outputBuffer, "%02i:%02i", hour, min);
    mx.clear();
    Display_PrintText(outputBuffer);
  }

  if (sec % 2 == 0) {

    mx.setPoint(1, 1, 1);
    mx.setPoint(6, 1, 0);
    
  } else {

    mx.setPoint(1, 1, 0);
    mx.setPoint(6, 1, 1);
  }
}

void Display_RandomLights() {

  mx.setPoint(
    int(random(ROW_SIZE)), 
    int(random(mx.getColumnCount())), 
    random(2) > 0.5);
}

void Display_RandomVisualizer() {

  int y = int(random(ROW_SIZE));
  int width = int(random(mx.getColumnCount() / 2));
  int startX = int(((mx.getColumnCount() / 2) - width));
  int endX = int(startX + (width * 2));
  
  for (int x = 0; x < mx.getColumnCount(); x++) {

    if (x < startX || x > endX) { 
      
      mx.setPoint(
        y, 
        x, 
        false);
        
    } else {
      
      mx.setPoint(
        y, 
        x, 
        true);
    }
  }    
}

void Display_PrintText(char *pMsg) {

  uint8_t modStart = 0;
  uint8_t modEnd = 3;
  uint8_t   state = 0;
  uint8_t   curLen;
  uint16_t  showLen;
  uint8_t   cBuf[8];
  int16_t   col = ((modEnd + 1) * COL_SIZE) - 1;

  mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

  do     // finite state machine to print the characters in the space available
  {
    switch(state)
    {
      case 0: // Load the next character from the font table
        // if we reached end of message, reset the message pointer
        if (*pMsg == '\0')
        {
          showLen = col - (modEnd * COL_SIZE);  // padding characters
          state = 2;
          break;
        }

        // retrieve the next character form the font file
        showLen = mx.getChar(*pMsg++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
        curLen = 0;
        state++;
        // !! deliberately fall through to next state to start displaying

      case 1: // display the next part of the character
        mx.setColumn(col--, cBuf[curLen++]);

        // done with font character, now display the space between chars
        if (curLen == showLen)
        {
          showLen = CHAR_SPACING;
          state = 2;
        }
        break;

      case 2: // initialize state for displaying empty columns
        curLen = 0;
        state++;
        // fall through

      case 3:  // display inter-character spacing or end of message padding (blank columns)
        mx.setColumn(col--, 0);
        curLen++;
        if (curLen == showLen)
          state = 0;
        break;

      default:
        col = -1;   // this definitely ends the do loop
    }
  } while (col >= (modStart * COL_SIZE));

  mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}
