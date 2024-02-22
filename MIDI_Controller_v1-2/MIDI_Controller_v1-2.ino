#include <MIDI.h>
#include "Controller.h"
#include <LiquidCrystal_PCF8574.h>
LiquidCrystal_PCF8574 lcd(0x27);
#include <LedControl.h> 

int DIN1 = 26;
int CS1 =  24;
int CLK1 = 22;
int DIN2 = 32;
int CS2 =  30;
int CLK2 = 28;
int DIN3 = 38;
int CS3 =  36;
int CLK3 = 34;
int DIN4 = 44;
int CS4 =  42;
int CLK4 = 40;
int start_bright_time=0;
int current_bright_time=0;
int start_dark_time=0;
int current_dark_time=0;
int light_state=1;
int check_bright=0;
int sequence_state=0;
int play_state=0;
byte rec1[8]= {0x1C,0x3C,0x7E,0xFF,0xFF,0x7E,0x3C,0x1C};//上
byte line1[8]= {0x04,0x08,0x10,0x10,0x10,0x10,0x08,0x04};//上
byte rec2[8]={0x18,0x3C,0x7E,0xFF,0xFF,0xFF,0x3C,0x18};//右
byte line2[8]={0x00,0x00,0x00,0x3C,0x42,0x81,0x00,0x00};//右
byte rec3[8]={0x38,0x3C,0x7E,0xFF,0xFF,0x7E,0x3C,0x38};//下
byte line3[8]={0x20,0x10,0x08,0x08,0x08,0x08,0x10,0x20};//下
byte rec4[8]={0x18,0x3C,0xFF,0xFF,0xFF,0x7E,0x3C,0x18};//左
byte line4[8]={0x00,0x00,0x81,0x42,0x3C,0x00,0x00,0x00};//左

LedControl lc1=LedControl(DIN1,CLK1,CS1,0);
LedControl lc2=LedControl(DIN2,CLK2,CS2,0);
LedControl lc3=LedControl(DIN3,CLK3,CS3,0);
LedControl lc4=LedControl(DIN4,CLK4,CS4,0);
/*************************************************************
  MIDI CONTROLLER

  參考來源：Notes and Volts
*************************************************************/
MIDI_CREATE_DEFAULT_INSTANCE();



//************************************************************
//***SET THE NUMBER OF CONTROLS USED**************************
//************************************************************
//---How many buttons are connected directly to pins?---------
byte NUMBER_BUTTONS = 17;
//---How many potentiometers are connected directly to pins?--
byte NUMBER_POTS = 8;
//---How many buttons are connected to a multiplexer?---------
byte NUMBER_MUX_BUTTONS = 0;
//---How many potentiometers are connected to a multiplexer?--
byte NUMBER_MUX_POTS = 0;
//************************************************************

//***ANY MULTIPLEXERS? (74HC4067)************************************
//MUX address pins must be connected to Arduino UNO pins 2,3,4,5
//A0 = PIN2, A1 = PIN3, A2 = PIN4, A3 = PIN5
//*******************************************************************
//Mux NAME (OUTPUT PIN, , How Many Mux Pins?(8 or 16) , Is It Analog?);

//cc list
#define volume 7
#define single_patch_RL_balance 10
#define modulation 1
#define sostenuto 66
#define _1st_ctrl 74
#define _2nd_ctrl 71
#define _3rd_ctrl 73
#define _4th_ctrl 72
#define _5th_ctrl 75
#define _6th_ctrl 76
#define _7th_ctrl 77
#define _8th_ctrl 78
#define note_plus 47
#define note_minus 45
#define control_toggle 43
#define harmonic 41 
#define unknown A7
//Mux M1(10, 16, false); //Digital multiplexer on Arduino pin 10
//Mux M2(A5, 8, true); //Analog multiplexer on Arduino analog pin A0
//*******************************************************************


//***DEFINE DIRECTLY CONNECTED POTENTIOMETERS************************
//Pot (Pin Number, Command, CC Control, Channel Number)
//**Command parameter is for future use**

Pot ROTPO1(A0, 0, _1st_ctrl, 1);
Pot ROTPO2(A1, 0, _2nd_ctrl, 1);//第五個旋鈕
Pot ROTPO3(A2, 0, _3rd_ctrl, 1);//第六個旋鈕
Pot ROTPO4(A3, 0, _4th_ctrl , 1);
Pot SLPO1(A4, 0, 16, 1);//第一個旋鈕
Pot SLPO2(A5, 0, 17, 1);//第三個旋鈕
Pot SLPO3(A6, 0, 18 , 1);//第四個旋鈕
Pot SLPO4(A7, 0, 19, 1);
//Pot PO5(A4, 0, 30, 1);
//Pot PO6(A5, 0, 31, 1);
//*******************************************************************
//Add pots used to array below like this-3, & ROTPOT4};
Pot *POTS[] {& ROTPO1, & ROTPO2, & ROTPO3, & ROTPO4, &SLPO1, &SLPO2, &SLPO3,&SLPO4  };
//*******************************************************************
int ROTPO[4] = {};


//***DEFINE DIRECTLY CONNECTED BUTTONS*******************************
//Button (Pin Number, Command, Note Number, Channel, Debounce Time)
//** Command parameter 0=NOTE  1=CC  2=Toggle CC **

Button TouchBU1(13, 0, 60, 1, 5 );
Button TouchBU2(12, 0, 61, 1, 5 );
Button TouchBU3(11, 0, 62, 1, 5 );
Button TouchBU4(10, 0, 63, 1, 5 );
Button TouchBU5(9, 0, 64, 1, 5 );
Button TouchBU6(8, 0, 65, 1, 5 );
Button TouchBU7(7, 0, 66, 1, 5 );
Button TouchBU8(6, 0, 67, 1, 5 );
Button TouchBU9(5, 0, 68, 1, 5 );
Button TouchBU10(4, 0, 69, 1, 5 );
Button TouchBU11(3, 0, 70, 1, 5 );
Button TouchBU12(2, 0, 71, 1, 5 );


Button NormalBU1(39, 1, sostenuto, 1, 5);
Button NormalBU2(31, 1, 91, 1, 5);
Button NormalBU3(33, 1, 92, 1, 5);
Button NormalBU4(35, 1, 93, 1 ,5);
Button NormalBU5(37, 1, 94, 1, 5);
//Button BU8(9, 2, 64, 1, 5 );
//*******************************************************************
//Add buttons used to array below like this->  Button *BUTTONS[] {&BU1, &BU2, &BU3, &BU4, &BU5, &BU6, &BU7, &BU8};
Button *BUTTONS[] {& TouchBU1, & TouchBU2, & TouchBU3, & TouchBU4, & TouchBU5, & TouchBU6, & TouchBU7, & TouchBU8, & TouchBU9, & TouchBU10, & TouchBU11, & TouchBU12, & NormalBU1, & NormalBU2, & NormalBU3, & NormalBU4, & NormalBU5};
//*******************************************************************


//***DEFINE BUTTONS CONNECTED TO MULTIPLEXER*************************
//Button::Button(Mux mux, byte muxpin, byte command, byte value, byte channel, byte debounce)
//** Command parameter 0=NOTE  1=CC  2=Toggle CC **

//Button MBU1(M1, 0, 0, 70, 1, 5);
//Button MBU2(M1, 1, 1, 71, 1, 5);
//Button MBU3(M1, 2, 2, 72, 1, 5);
//Button MBU4(M1, 3, 0, 73, 1, 5);
//Button MBU5(M1, 4, 0, 74, 1, 5);
//Button MBU6(M1, 5, 0, 75, 1, 5);
//Button MBU7(M1, 6, 0, 76, 1, 5);
//Button MBU8(M1, 7, 0, 77, 1, 5);
//Button MBU9(M1, 8, 0, 78, 1, 5);
//Button MBU10(M1, 9, 0, 79, 1, 5);
//Button MBU11(M1, 10, 0, 80, 1, 5);
//Button MBU12(M1, 11, 0, 81, 1, 5);
//Button MBU13(M1, 12, 0, 82, 1, 5);
//Button MBU14(M1, 13, 0, 83, 1, 5);
//Button MBU15(M1, 14, 0, 84, 1, 5);
//Button MBU16(M1, 15, 0, 85, 1, 5);
//*******************************************************************
////Add multiplexed buttons used to array below like this->  Button *MUXBUTTONS[] {&MBU1, &MBU2, &MBU3, &MBU4, &MBU5, &MBU6.....};
Button *MUXBUTTONS[] {};

//*******************************************************************


//***DEFINE POTENTIOMETERS CONNECTED TO MULTIPLEXER*******************
//Pot::Pot(Mux mux, byte muxpin, byte command, byte control, byte channel)
//**Command parameter is for future use**

//Pot MPO1(M2, 0, 0, 1, 1);
//Pot MPO2(M2, 1, 0, 7, 1);
//Pot MPO3(M2, 2, 0, 50, 1);
//Pot MPO4(M2, 3, 0, 55, 2);
//Pot MPO5(M2, 4, 0, 50, 1);
//Pot MPO6(M2, 5, 0, 55, 2);
//Pot MPO7(M2, 6, 0, 50, 1);
//Pot MPO8(M2, 7, 0, 55, 2);
//Pot MPO9(M2, 8, 0, 50, 1);
//Pot MPO10(M2, 9, 0, 55, 2);
//Pot MPO11(M2, 10, 0, 50, 1);
//Pot MPO12(M2, 11, 0, 55, 2);
//Pot MPO13(M2, 12, 0, 50, 1);
//Pot MPO14(M2, 13, 0, 55, 2);
//Pot MPO15(M2, 14, 0, 50, 1);
//Pot MPO16(M2, 15, 0, 55, 2);
//*******************************************************************
//Add multiplexed pots used to array below like this->  Pot *MUXPOTS[] {&MPO1, &MPO2, &MPO3, &MPO4, &MPO5, &MPO6.....};
Pot *MUXPOTS[] {};
//*******************************************************************
int buttons_state[12];
int mimic_buttons_state[12];
int notebase=60;
void setup() {
  //Serial.begin(9600);
  lcd.begin(16, 2);
  MIDI.begin(MIDI_CHANNEL_OFF);
  pinMode(note_plus, INPUT);
  pinMode(note_minus,INPUT);
  pinMode(control_toggle, INPUT);
  pinMode(harmonic, INPUT);
  //pinMode(unknown, INPUT);
  for(int t=0;t<13;t++){
buttons_state[t]=0;
mimic_buttons_state[t]=0;  
}
  //test_setup();

  lcd.setBacklight(255);
  lcd.clear();
  lcd.setCursor(0, 0);  //設定游標位置 (字,行)
  lcd.print("Control Mode 1");
  lcd.setCursor(0, 1);  //設定游標位置 (字,行)
  lcd.print("Chord : Off");
  lc1.shutdown(0,false);       
 lc1.setIntensity(0,15);      
 lc1.clearDisplay(0);         
  lc2.shutdown(0,false);       
 lc2.setIntensity(0,15);      
 lc2.clearDisplay(0);         
  lc3.shutdown(0,false);       
 lc3.setIntensity(0,15);      
 lc3.clearDisplay(0);         
  lc4.shutdown(0,false);       
 lc4.setIntensity(0,15);      
 lc4.clearDisplay(0);         
 start_bright_time=millis();
   for (int i=0; i<=7;i++){
  ROTPO[i]=0;
}
}
/*
void piano_setup(){
  for (int i=0; i<=3;i++){
  ROTPO[i]=0;  
}
for  (int i=0;i<=6;i++){
  BUTTONS[i]->newValue(0, 72+i*2, 1);
} 

BUTTONS[7]->newValue(1, 66, 1);
BUTTONS[8]->newValue(1, 66, 1);
  POTS[0]->newValue(0, volume, 1);//音量
  POTS[1]->newValue(0, single_patch_RL_balance, 1);//左右單聲道平衡
  POTS[2]->newValue(0, modulation, 1);//顫音     
  POTS[3]->newValue(0, sostenuto, 1);//
  //71控制第二個旋鈕           
}

void test_setup(){
    for (int i=0; i<=3;i++){
  ROTPO[i]=0;  
}

BUTTONS[7]->newValue(1, 66, 1);
BUTTONS[8]->newValue(1, 66, 1);  
}
*/
void note_reset(){
    BUTTONS[0]->newValue(0, notebase+0, 1);
    BUTTONS[1]->newValue(0, notebase+1, 1);
    BUTTONS[2]->newValue(0, notebase+2, 1);
    BUTTONS[3]->newValue(0, notebase+3, 1);
    BUTTONS[4]->newValue(0, notebase+4, 1);
    BUTTONS[5]->newValue(0, notebase+5, 1);
    BUTTONS[6]->newValue(0, notebase+6, 1);
    BUTTONS[7]->newValue(0, notebase+7, 1);
    BUTTONS[8]->newValue(0, notebase+8, 1);
    BUTTONS[9]->newValue(0, notebase+9, 1);
    BUTTONS[10]->newValue(0, notebase+10, 1);
    BUTTONS[11]->newValue(0, notebase+11, 1);
}

void control_reset1(){
    POTS[0]->newValue(0, _1st_ctrl, 1);
    POTS[1]->newValue(0, _2nd_ctrl, 1);
    POTS[2]->newValue(0, _3rd_ctrl, 1);      
    POTS[3]->newValue(0, _4th_ctrl, 1);
}

void control_reset2(){
    POTS[0]->newValue(0, _5th_ctrl, 1);
    POTS[1]->newValue(0, _6th_ctrl, 1);
    POTS[2]->newValue(0, _7th_ctrl, 1);      
    POTS[3]->newValue(0, _8th_ctrl, 1);
}

void control_reset3(){
    POTS[0]->newValue(0, volume, 1);
    POTS[1]->newValue(0, single_patch_RL_balance, 1);
    POTS[2]->newValue(0, modulation, 1);      
    POTS[3]->newValue(0, 18, 1);
}

int switch_note_plus=0;
int switch_note_minus=0;
int switch_control=0;
int switch_harmonic=0;
int last_note_plus=0;
int last_note_minus=0;
int last_control_toggle=0;
int last_switch_harmonic=0;
int control_state=0;
int harmonic_state=0;
bool sustain = false;
//bool in_harmonic = false;
//int slp=0;
//int last_slp=0;
int bu1=0;
int bright_delay=50;
int sum_bu=0;

void loop() {
  sum_bu=0;
  if (NUMBER_BUTTONS != 0) updateButtons();
  if (NUMBER_POTS != 0) updatePots();
  if (NUMBER_MUX_BUTTONS != 0) updateMuxButtons();
  if (NUMBER_MUX_POTS != 0) updateMuxPots();
  switch_note_plus=digitalRead(note_plus);
  switch_note_minus=digitalRead(note_minus);
  switch_control=digitalRead(control_toggle);
  switch_harmonic=digitalRead(harmonic);
  //slp=analogRead(unknown);
  for(int j=2;j<=13;j++){
      sum_bu+=digitalRead(j);    
  }
      if(sum_bu>0){
      play_state=1;
    }
  if (switch_note_plus==HIGH && switch_note_plus!=last_note_plus){

    
    if (notebase<=104){
      //Serial.println("plus");
      notebase+=12;
      note_reset();
    }
        
  }
  if (switch_note_minus==HIGH && switch_note_minus!=last_note_minus){
      if (notebase>=12){
      //Serial.println("Minus");
      notebase-=12;
      note_reset();
    }
        
  }
if(switch_control==HIGH && switch_control!=last_control_toggle){
  if(control_state==0){
    //Serial.println("Control up");    
    control_state=1;
    control_reset2();
    if(harmonic_state==0){
        lcd.clear();
        lcd.setCursor(0, 0);  //設定游標位置 (字,行)
        lcd.print("Control Mode 2");
        lcd.setCursor(0, 1);  //設定游標位置 (字,行)
        lcd.print("Chord : Off");
    }else{
        lcd.clear();
        lcd.setCursor(0, 0);  //設定游標位置 (字,行)
        lcd.print("Control Mode 2");
        lcd.setCursor(0, 1);  //設定游標位置 (字,行)
        lcd.print("Chord : On");      
    }    
  }else if(control_state==1){
    //Serial.println("Control down"); 
    control_state=2;
    control_reset3();
    if(harmonic_state==0){
        lcd.clear();
        lcd.setCursor(0, 0);  //設定游標位置 (字,行)
        lcd.print("Control Mode 3");
        lcd.setCursor(0, 1);  //設定游標位置 (字,行)
        lcd.print("Chord : Off");
    }else{
        lcd.clear();
        lcd.setCursor(0, 0);  //設定游標位置 (字,行)
        lcd.print("Control Mode 3");
        lcd.setCursor(0, 1);  //設定游標位置 (字,行)
        lcd.print("Chord : On");      
    }      
    } else if(control_state==2){
      control_state=0;
      control_reset1();
      if(harmonic_state==0){
        lcd.clear();
        lcd.setCursor(0, 0);  //設定游標位置 (字,行)
        lcd.print("Control Mode 1");
        lcd.setCursor(0, 1);  //設定游標位置 (字,行)
        lcd.print("Chord : Off");
    }else{
        lcd.clear();
        lcd.setCursor(0, 0);  //設定游標位置 (字,行)
        lcd.print("Control Mode 1");
        lcd.setCursor(0, 1);  //設定游標位置 (字,行)
        lcd.print("Chord : On");      
    }        
    }  
}   
if(switch_harmonic==HIGH && switch_harmonic!=last_switch_harmonic){
  if(harmonic_state==0){
    //Serial.println("Control up");    
    harmonic_state=1;
    if(control_state==0){
        lcd.clear();
        lcd.setCursor(0, 0);  //設定游標位置 (字,行)
        lcd.print("Control Mode 1");
        lcd.setCursor(0, 1);  //設定游標位置 (字,行)
        lcd.print("Chord : On");
    }else if(control_state==1){
        lcd.clear();
        lcd.setCursor(0, 0);  //設定游標位置 (字,行)
        lcd.print("Control Mode 2");
        lcd.setCursor(0, 1);  //設定游標位置 (字,行)
        lcd.print("Chord : On");
    }else{
        lcd.clear();
        lcd.setCursor(0, 0);  //設定游標位置 (字,行)
        lcd.print("Control Mode 3");
        lcd.setCursor(0, 1);  //設定游標位置 (字,行)
        lcd.print("Chord : On");      
    }   
  }else if(harmonic_state==1){
    //Serial.println("Control down"); 
    harmonic_state=0; 
        if(control_state==0){
        lcd.clear();
        lcd.setCursor(0, 0);  //設定游標位置 (字,行)
        lcd.print("Control Mode 1");
        lcd.setCursor(0, 1);  //設定游標位置 (字,行)
        lcd.print("Chord : Off");
    }else if(control_state==1){
        lcd.clear();
        lcd.setCursor(0, 0);  //設定游標位置 (字,行)
        lcd.print("Control Mode 2");
        lcd.setCursor(0, 1);  //設定游標位置 (字,行)
        lcd.print("Chord : Off");
    }else{
        lcd.clear();
        lcd.setCursor(0, 0);  //設定游標位置 (字,行)
        lcd.print("Control Mode 3");
        lcd.setCursor(0, 1);  //設定游標位置 (字,行)
        lcd.print("Chord : Off");      
    }   
    }   
}     
  last_note_plus=switch_note_plus;
  last_note_minus=switch_note_minus;
  last_control_toggle=switch_control;
  last_switch_harmonic=switch_harmonic;

    if(play_state){
      
        
    if (sequence_state==0){
      if(light_state){
      
        printByte1(rec2);
                
      
      
      current_bright_time=millis();
          if(current_bright_time-start_bright_time>=bright_delay){
            lc1.clearDisplay(0);
          
          light_state=0;
          start_dark_time=current_bright_time;      
          }
    }else{
      current_dark_time=millis();
      printByte1(line2);
          if(current_dark_time-start_dark_time>=0){
          light_state=1;
          start_bright_time=current_dark_time;
          sequence_state=1;      
          }
    }
    }else if(sequence_state==1){
        if(light_state){
      
        printByte2(rec1);
                
      
      
      current_bright_time=millis();
          if(current_bright_time-start_bright_time>=bright_delay){
          lc2.clearDisplay(0);
          light_state=0;
          start_dark_time=current_bright_time;      
          }
    }else{
      current_dark_time=millis();
      printByte2(line1);
          if(current_dark_time-start_dark_time>=0){
          light_state=1;
          start_bright_time=current_dark_time;
          sequence_state=2;      
          }
    }      
    }else if(sequence_state==2){
        if(light_state){
      
        printByte3(rec4);
                
      
      
      current_bright_time=millis();
          if(current_bright_time-start_bright_time>=bright_delay){
          lc3.clearDisplay(0);
          light_state=0;
          start_dark_time=current_bright_time;      
          }
    }else{
      current_dark_time=millis();
      printByte3(line4);
          if(current_dark_time-start_dark_time>=0){
          light_state=1;
          start_bright_time=current_dark_time;
          sequence_state=3;      
          }
    }      
    }else if(sequence_state==3){
        if(light_state){
      
        printByte4(rec3);
                
      
      
      current_bright_time=millis();
          if(current_bright_time-start_bright_time>=bright_delay){
          lc4.clearDisplay(0);
          light_state=0;
          start_dark_time=current_bright_time;      
          }
    }else{
      current_dark_time=millis();
      printByte4(line3);
          if(current_dark_time-start_dark_time>=0){
          light_state=1;
          start_bright_time=current_dark_time;
          sequence_state=0; 
          play_state=0;     
          }
    }      
    }
    
    }else{
      //lc1.clearDisplay(0);
      //lc2.clearDisplay(0);
      //lc3.clearDisplay(0);
      //lc4.clearDisplay(0);
      printByte1(line2);
      printByte2(line1);
      printByte3(line4);
      printByte4(line3);
      
    }

    
  }  


bool status_sus = false;

 
//*****************************************************************
void updateButtons() {    
  /*
  sum_bu=0;
  for(int j=0;j<12;j++){
    sum_bu+=buttons_state[j];    
  }
  */
  // Cycle through Button array
  for (int i = 0; i < NUMBER_BUTTONS; i = i + 1) {
    byte message = BUTTONS[i]->getValue();
    
    //  Button is pressed
    if (message == 1) {
      switch (BUTTONS[i]->Bcommand) {
        case 0: //Note
          buttons_state[i]=1;
          
          if (harmonic_state==0){
            MIDI.sendNoteOn(BUTTONS[i]->Bvalue, 127, BUTTONS[i]->Bchannel);            
            }else if(harmonic_state==1){
               MIDI.sendNoteOn(BUTTONS[i]->Bvalue, 127, BUTTONS[i]->Bchannel);
               delay(10);
              MIDI.sendNoteOn((BUTTONS[i]->Bvalue)+4, 127, BUTTONS[i]->Bchannel);
              if (i<=7) mimic_buttons_state[i+4]=1;
                delay(10);                                       
              MIDI.sendNoteOn((BUTTONS[i]->Bvalue)+7, 127, BUTTONS[i]->Bchannel);
              if (i<=4) mimic_buttons_state[i+7]=1;              
}
            
          
          break;
        case 1: //CC
            if(i==12) sustain=true;            
            MIDI.sendControlChange(BUTTONS[i]->Bvalue, 127, BUTTONS[i]->Bchannel);                  
            break;          
/*
          if(i==7){            
                  MIDI.sendControlChange(BUTTONS[i]->Bvalue, 127, BUTTONS[i]->Bchannel);                  
                  break;                 
          }else if (i==8){
              MIDI.sendControlChange(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);                  
                  break;            
          }
*/          
        case 2: //Toggle
          if (BUTTONS[i]->Btoggle == 0) {
            MIDI.sendControlChange(BUTTONS[i]->Bvalue, 127, BUTTONS[i]->Bchannel);
            BUTTONS[i]->Btoggle = 1;
          }
          else if (BUTTONS[i]->Btoggle == 1) {
            MIDI.sendControlChange(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);
            BUTTONS[i]->Btoggle = 0;
          }
          break;
      }
    }

    //  Button is not pressed
    if (message == 0) {
      switch (BUTTONS[i]->Bcommand) {
        case 0:
        buttons_state[i]=0;
        
          if (!sustain){
            
          if(!harmonic_state){
              MIDI.sendNoteOff(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);  
          }
          
          
          if (harmonic_state){
          if(i>=7){
            if (buttons_state[i]==0 && buttons_state[i-7]==0 && buttons_state[i-4]==0){
              MIDI.sendNoteOff(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);
              delay(10);            
              } 
                  
          }else if(i>=4){
              if (buttons_state[i]==0 && buttons_state[i-4]==0){
              MIDI.sendNoteOff(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);
              delay(10);
              }            
          }else{
              if (buttons_state[i]==0 ){
              MIDI.sendNoteOff(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);
              delay(10);
              }            
          }
          
          if(i<=7){
            if (buttons_state[i+4]==0 && buttons_state[i-3]==0){
              MIDI.sendNoteOff((BUTTONS[i]->Bvalue)+4, 0, BUTTONS[i]->Bchannel);
              mimic_buttons_state[i+4]=0;
            }                         
          }else{
              MIDI.sendNoteOff((BUTTONS[i]->Bvalue)+4, 0, BUTTONS[i]->Bchannel);
              //mimic_buttons_state[i+4]=0;            
          } 
          delay(10);
          if (i<=4){
            if (buttons_state[i+7]==0 && buttons_state[i+3]==0){
                MIDI.sendNoteOff((BUTTONS[i]->Bvalue)+7, 0, BUTTONS[i]->Bchannel);
                mimic_buttons_state[i+7]=0;              
            }            
          }else{
            MIDI.sendNoteOff((BUTTONS[i]->Bvalue)+7, 0, BUTTONS[i]->Bchannel);
            //mimic_buttons_state[i+7]=0;  
          }
          }      
          break;
          }else{
            if (harmonic_state){
            
          
          if(i<=7){
            if (buttons_state[i+4]==0 && buttons_state[i-3]==0){
              //MIDI.sendNoteOff((BUTTONS[i]->Bvalue)+4, 0, BUTTONS[i]->Bchannel);
              mimic_buttons_state[i+4]=0;
            }                         
          } 
          delay(10);
          if (i<=4){
            if (buttons_state[i+7]==0 && buttons_state[i+3]==0){
                //MIDI.sendNoteOff((BUTTONS[i]->Bvalue)+7, 0, BUTTONS[i]->Bchannel);
                mimic_buttons_state[i+7]=0;              
            }            
          }
          }        
          }
        case 1:
        if(i==12){
         sustain=false;
          for(int j=0;j<12;j++){
            if (harmonic_state==0){
              if(!buttons_state[j]) MIDI.sendNoteOff(BUTTONS[j]->Bvalue, 0, BUTTONS[i]->Bchannel);
              }else if(harmonic_state==1){
                   if(j<=4){
                     if (j<=3){
                        if(!buttons_state[j]){
                          MIDI.sendNoteOff(BUTTONS[j]->Bvalue, 0, BUTTONS[i]->Bchannel);
                          if (!buttons_state[j+7] && !mimic_buttons_state[j+7]) MIDI.sendNoteOff((BUTTONS[j]->Bvalue)+7, 0, BUTTONS[i]->Bchannel);                      
                    }
                     }else{
                        if(!buttons_state[j] && !buttons_state[j-4]){
                          MIDI.sendNoteOff(BUTTONS[j]->Bvalue, 0, BUTTONS[i]->Bchannel);
                          if (!buttons_state[j+7] && !mimic_buttons_state[j+7]) MIDI.sendNoteOff((BUTTONS[j]->Bvalue)+7, 0, BUTTONS[i]->Bchannel);                      
                    }                       
                     }                                         
                                          

                }else if(j<=7){
                  if(j<=6){
                    if (!buttons_state[j] && !mimic_buttons_state[j] && !buttons_state[j-4]){
                     MIDI.sendNoteOff(BUTTONS[j]->Bvalue, 0, BUTTONS[i]->Bchannel);
                  }          
                  }else{
                      if (!buttons_state[j] && !mimic_buttons_state[j] && !buttons_state[j-4] && !buttons_state[j-7]){
                     MIDI.sendNoteOff(BUTTONS[j]->Bvalue, 0, BUTTONS[i]->Bchannel);
                  }                       
                  }
                                                                 
                  MIDI.sendNoteOff((BUTTONS[j]->Bvalue)+7, 0, BUTTONS[i]->Bchannel);
                }else{
                    if (!buttons_state[j] && !mimic_buttons_state[j] && !buttons_state[j-4] && !buttons_state[j-7]){
                     MIDI.sendNoteOff(BUTTONS[j]->Bvalue, 0, BUTTONS[i]->Bchannel);
                    }
                    MIDI.sendNoteOff((BUTTONS[j]->Bvalue)+4, 0, BUTTONS[i]->Bchannel);
                    MIDI.sendNoteOff((BUTTONS[j]->Bvalue)+7, 0, BUTTONS[i]->Bchannel);                
                    }             
              }
            
          }
        }        
          MIDI.sendControlChange(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);
          break;
      }
    }

  }

}
//*******************************************************************
void updateMuxButtons() {

  // Cycle through Mux Button array
  for (int i = 0; i < NUMBER_MUX_BUTTONS; i = i + 1) {

    MUXBUTTONS[i]->muxUpdate();
    byte message = MUXBUTTONS[i]->getValue();

    //  Button is pressed
    if (message == 0) {
      switch (MUXBUTTONS[i]->Bcommand) {
        case 0: //Note
          MIDI.sendNoteOn(MUXBUTTONS[i]->Bvalue, 127, MUXBUTTONS[i]->Bchannel);
          break;
        case 1: //CC
          MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue, 127, MUXBUTTONS[i]->Bchannel);
          break;
        case 2: //Toggle
          if (MUXBUTTONS[i]->Btoggle == 0) {
            MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue, 127, MUXBUTTONS[i]->Bchannel);
            MUXBUTTONS[i]->Btoggle = 1;
          }
          else if (MUXBUTTONS[i]->Btoggle == 1) {
            MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue, 0, MUXBUTTONS[i]->Bchannel);
            MUXBUTTONS[i]->Btoggle = 0;
          }
          break;
      }
    }
    //  Button is not pressed
    if (message == 1) {
      switch (MUXBUTTONS[i]->Bcommand) {
        case 0:
          MIDI.sendNoteOff(MUXBUTTONS[i]->Bvalue, 0, MUXBUTTONS[i]->Bchannel);
          break;
        case 1:
          MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue, 0, MUXBUTTONS[i]->Bchannel);
          break;
      }
    }
  }
}
//***********************************************************************
void updatePots() {
  for (int i = 0; i < NUMBER_POTS; i = i + 1) {
    byte potmessage = POTS[i]->getValue();
    int mul_fac=2;
    //if (i==2) mul_fac=1;
    bool alter=abs(potmessage-ROTPO[i])>=1;
    int new_potmsg=potmessage*mul_fac-100;
    if(new_potmsg<0) new_potmsg=0;
    if(new_potmsg>127) new_potmsg=127;
    //if (alter) Serial.println(new_potmsg);         
    if (potmessage != 255 && alter) MIDI.sendControlChange(POTS[i]->Pcontrol, new_potmsg, POTS[i]->Pchannel);
    ROTPO[i]=potmessage; 
  }
}
//***********************************************************************
void updateMuxPots() {
  for (int i = 0; i < NUMBER_MUX_POTS; i = i + 1) {
    MUXPOTS[i]->muxUpdate();
    byte potmessage = MUXPOTS[i]->getValue();
    if (potmessage != 255) MIDI.sendControlChange(MUXPOTS[i]->Pcontrol, potmessage, MUXPOTS[i]->Pchannel);
  }
}

void printByte1(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc1.setRow(0,i,character[i]);
  }
}

void printByte2(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc2.setRow(0,i,character[i]);
  }
}

void printByte3(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc3.setRow(0,i,character[i]);
  }
}

void printByte4(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc4.setRow(0,i,character[i]);
  }
}