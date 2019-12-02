#include <Arduino.h>
#include <MIDI.h>
#include <LiquidCrystal.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, midiA)
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiB)
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, midiC)
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, midiD)

midi::MidiType midiType;
byte midiData1;
byte midiData2;
int midiChannel;

const int rs = 12, en = 10, d0 = 9, d1 = 8, d2 = 7, d3 = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int backlightPin = 21;
LiquidCrystal lcd(rs, en, d0, d1, d2, d3, d4, d5, d6, d7);

int dipPins[] = {26, 28, 30, 32, 38, 40, 42, 44, 27, 29, 31, 33, 39, 41, 43, 45};
int transAChannel;
int transBChannel;
int transCChannel;
int transDChannel;

void setup() {
  midiA.begin(MIDI_CHANNEL_OMNI);
  midiB.begin(MIDI_CHANNEL_OMNI);
  midiC.begin(MIDI_CHANNEL_OMNI);
  midiD.begin(MIDI_CHANNEL_OMNI);
  midiA.turnThruOff();
  midiB.turnThruOff();
  midiC.turnThruOff();
  midiD.turnThruOff();

  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  pinMode(backlightPin, OUTPUT);
  digitalWrite(backlightPin, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Welcome");
  lcd.setCursor(0, 2);
  lcd.print("MIDI merger");

  int i;
  for(i = 0; i<=15; i++){
    pinMode(dipPins[i], INPUT_PULLUP);      // set the digital pins (defined above) as input
  }
  delay(2000);

  lcd.clear();
  for (int i = 0; i<=3; i++){
    lcd.setCursor(0,i);
    lcd.print(i+1);
    lcd.print(":");
  }
}

//Read state from DIP Switch (4 positions used) using int x as multiplier for pins to
byte address(int x){
 int i,j=0;
 
 //Get the switches state
 for(i=0; i<=3; i++){
 j = (j << 1) | digitalRead(dipPins[i+(4*x)]);   // read each input pin
 }
 return j; //return address
}
void displayInput(int v, midi::MidiType w, byte x, byte y, int z) {
  lcd.setCursor(3,v);
  lcd.print(w);
  lcd.print(" ");
  lcd.print(x);
  lcd.print(" ");
  lcd.print(y);
  lcd.print(" ");
  lcd.print(z);
}

void displayOutput(int w, int x, int y, int z) {
  lcd.setCursor(18,0);
  lcd.print(w);
  lcd.setCursor(18,1);
  lcd.print(x);
  lcd.setCursor(18,2);
  lcd.print(y);
  lcd.setCursor(18,3);
  lcd.print(z);
}

void loop() {

        midiA.sendNoteOn(42, 127, 1);    // Send a Note (pitch 42, velo 127 on channel 1)
        delay(1000);		            // Wait for a second
        midiA.sendNoteOff(42, 0, 1);     // Stop the note
        delay(1000);

  // transAChannel = address(0);
  // transBChannel = address(1);
  // transCChannel = address(2);
  // transDChannel = address(3);
  // displayOutput(transAChannel, transBChannel, transCChannel, transDChannel);

  // if (midiA.read()) {
  //   if (transAChannel == 0) {
  //     transAChannel = midiA.getChannel();
  //   }
  //   midiType = midiA.getType();
  //   midiData1 = midiA.getData1();
  //   midiData2 = midiA.getData2();
  //   if (transAChannel == 0) {midiChannel = midiA.getChannel();} else {midiChannel = transAChannel;}
  //   displayInput(0, midiType, midiData1, midiData2, midiChannel);
  //   midiA.send(midiType, midiData1, midiData2, midiChannel);
  // }
  // if (midiB.read()) {
  //   midiA.send(midiB.getType(),
  //              midiB.getData1(),
  //              midiB.getData2(),
  //              transBChannel == 0 ? midiB.getChannel() : transBChannel);
  // }
  // if (midiC.read()) {
  //   midiA.send(midiC.getType(),
  //              midiC.getData1(),
  //              midiC.getData2(),
  //              transCChannel == 0 ? midiC.getChannel() : transCChannel);
  // }
  // if (midiD.read()) {
  //   midiA.send(midiD.getType(),
  //              midiD.getData1(),
  //              midiD.getData2(),
  //              transDChannel == 0 ? midiD.getChannel() : transDChannel);
  // }  
}



