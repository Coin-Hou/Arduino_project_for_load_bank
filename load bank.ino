// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// For TFT shield
#include <stdint.h>
#include "TouchScreen.h"
#include <UTFT.h>


// These are the pins for the TFT shield!
#define YP A0  // must be an analog pin, use "An" notation!
#define XM A1  // must be an analog pin, use "An" notation!
#define YM A2   // can be a digital pin
#define XP A3   // can be a digital pin

// MIN&MAX pressure
#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define DICLENGTH 29
int end_order = 9;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

UTFT myGLCD(ILI9481,38,39,40,41);

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
int x, y;
boolean state;
const int bits = 8;    // number of bits

int bitPin[] = { 
2, 3, 4, 5, 6,7,8,9};   // Pin Ccorresponding to bit


//the amp_value vs. sequence table
int amp[DICLENGTH] = {10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,500,600,700,800,900,1000};
int sequence[DICLENGTH][8] = {{0,0,0,0,0,0,0,1},{0,0,0,0,0,0,1,0},{0,0,0,0,0,0,1,1},{0,0,0,0,0,1,0,0},{0,0,0,0,0,1,0,1},{0,0,0,0,0,1,1,0},
                              {0,0,0,0,0,1,1,1},{0,0,0,0,1,0,0,0},{0,0,0,0,1,0,0,1},{0,0,0,0,1,0,1,0},{0,0,0,0,1,0,1,1},{0,0,0,0,1,1,0,0},
                              {0,0,0,0,1,1,0,1},{0,0,0,0,1,1,1,0},{0,0,0,0,1,1,1,1},{0,0,0,1,1,0,0,0},{0,0,0,1,1,0,0,1},{0,0,0,1,1,0,1,0},
                              {0,0,0,1,1,0,1,1},{0,0,0,1,1,1,0,0},{0,0,0,1,1,1,0,1},{0,0,0,1,1,1,1,0},{0,0,0,1,1,1,1,1},{0,1,0,1,1,0,1,0},
                              {0,1,1,1,0,1,0,0},{0,1,1,1,1,1,1,0},{1,1,1,0,0,0,0,0},{1,1,1,1,0,0,1,0},{1,1,1,1,1,1,0,0}};
int zero_state[8] = {0,0,0,0,0,0,0,0};


int index_find(int value) //look up the amp table
{
  int index = 0;
  int flag = 0;
  for (int num=0; num<end_order+1; num++){
    if (amp[num]==value){
      index = num;
      flag = 1;
      break;
    }
  }
  if(flag==0){
    Serial.println("The sequence is not found!");
  }
  return index;
}

// set the state of pin according to the look-up sequence
void set_amp(int sequence_state[]){
        for (int num = 0; num < 8; num++) {
          Serial.println(sequence_state[7-num]);
          if (sequence_state[7-num]==0){
            state=LOW;
          }
          else {
            state=HIGH;
          }
          digitalWrite(bitPin[num], state);
        }     
}


// execute the amp profile
void  button_trigger(int label)
{
      myGLCD.setColor(0, 0, 0);
      myGLCD.fillRoundRect (190, 150, 400, 180);
      myGLCD.setBackColor(0, 0, 0);
      myGLCD.setColor(255, 0, 0);
      myGLCD.setFont(BigFont);
      myGLCD.print("Sequence", 195,155, 0);
      myGLCD.printNumI(label, 340,155);
      int index = 0;
        
      if (label==1){
        index = index_find(50);
        delay(1000);
        set_amp(sequence[index]);
        delay(10800000);
        set_amp(zero_state);
        delay(1000);
        set_amp(zero_state);
      }
    else if (label==2){
        index = index_find(100);
        delay(1000);
        set_amp(sequence[index]);
        for (int num=0; num<900; num++)
        {
          delay(10000);
          set_amp(zero_state);
          delay(10000);
          set_amp(sequence[index]);          
        }
        delay(1000);
        set_amp(zero_state);
        delay(1000);
        set_amp(zero_state);          
    }
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRoundRect (190, 150, 400, 180);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(255, 0, 0);
  myGLCD.setFont(BigFont);
  myGLCD.print("None", 195,155, 0); 
}


// detect the botton pressure each 10ms by timer
void detect(){
TSPoint p = ts.getPoint();
if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
  x=480-(p.y-75)/1.8;
  y=(p.x-145)/2.4;
        if ((y>=10) && (y<=60))  // Upper row
      {
        if ((x>=10) && (x<=60))  // Button: 1
        {
          Serial.println("Print bottom 1.");
          button_trigger(1);
        }
        if ((x>=70) && (x<=120))  // Button: 2
        {
          Serial.println("Print bottom 2.");
          button_trigger(2);
        }
        if ((x>=130) && (x<=180))  // Button: 3
        {
          Serial.println("Print bottom 3.");
          button_trigger(3);
        }
        if ((x>=190) && (x<=240))  // Button: 4
        {
          Serial.println("Print bottom 4.");
          button_trigger(4);
        }
        if ((x>=250) && (x<=300))  // Button: 5
        {
          Serial.println("Print bottom 5.");
          button_trigger(5);
        }
      }

//      if ((y>=70) && (y<=120))  // Center row
//      {
//        if ((x>=10) && (x<=60))  // Button: 6
//        {
//          waitForIt(10, 70, 60, 120);
//          updateStr('6');
//        }
//        if ((x>=70) && (x<=120))  // Button: 7
//        {
//          waitForIt(70, 70, 120, 120);
//          updateStr('7');
//        }
//        if ((x>=130) && (x<=180))  // Button: 8
//        {
//          waitForIt(130, 70, 180, 120);
//          updateStr('8');
//        }
//        if ((x>=190) && (x<=240))  // Button: 9
//        {
//          waitForIt(190, 70, 240, 120);
//          updateStr('9');
//        }
//        if ((x>=250) && (x<=300))  // Button: 0
//        {
//          waitForIt(250, 70, 300, 120);
//          updateStr('0');
//        }
//      }
}  
}

// draw the button GUI
void drawButtons()
{
// Draw the upper row of buttons
  myGLCD.setBackColor(0, 0, 255);
  for (x=0; x<5; x++)
  {
    myGLCD.setColor(0, 0, 255);
    myGLCD.fillRoundRect (10+(x*60), 10, 60+(x*60), 60);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (10+(x*60), 10, 60+(x*60), 60);
    myGLCD.printNumI(x+1, 27+(x*60), 27);
  }
// Draw the center row of buttons
  for (x=0; x<5; x++)
  {
    myGLCD.setColor(0, 0, 255);
    myGLCD.fillRoundRect (10+(x*60), 70, 60+(x*60), 120);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (10+(x*60), 70, 60+(x*60), 120);
    if (x<4)
      myGLCD.printNumI(x+6, 27+(x*60), 87);
  }
  myGLCD.print("10", 267, 87);
}


void setup() {
  Serial.begin(9600);
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  myGLCD.clrScr();
  myGLCD.setBackColor(0, 0, 0);
  drawButtons(); 
  for (int i = 0; i < bits; i++) {
    pinMode(bitPin[i], OUTPUT); 
  }
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print("State:", 100,155, 0);
  myGLCD.print("None", 195,155, 0);
}


void loop() { 
  detect();
  delay(10);
}
