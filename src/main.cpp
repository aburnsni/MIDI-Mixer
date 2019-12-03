#include <Arduino.h>
#include <MIDI.h>
#include <LiquidCrystal.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, midi1)
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midi4)
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, midi3)
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, midi2)

midi::MidiType midiType;
byte midiData1;
byte midiData2;
int midiChannel;

// LCD setup
const int rs = 12, en = 10, d0 = 9, d1 = 8, d2 = 7, d3 = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int backlightPin = 21;
LiquidCrystal lcd(rs, en, d0, d1, d2, d3, d4, d5, d6, d7);

// DIP switch setup
int dipPins[16] = {26, 28, 30, 32, 38, 40, 42, 44, 27, 29, 31, 33, 39, 41, 43, 45};
int transmitChannel[4] = {0, 0, 0, 0};

void setup() {

  // MIDI declerations
  midi1.begin(MIDI_CHANNEL_OMNI);
  midi2.begin(MIDI_CHANNEL_OMNI);
  midi3.begin(MIDI_CHANNEL_OMNI);
  midi4.begin(MIDI_CHANNEL_OMNI);
  midi1.turnThruOff();
  midi2.turnThruOff();
  midi3.turnThruOff();
  midi4.turnThruOff();


  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  pinMode(backlightPin, OUTPUT);
  digitalWrite(backlightPin, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Welcome");
  lcd.setCursor(0, 2);
  lcd.print("MIDI merger");

  for(uint8_t i = 0; i<=15; i++){
    pinMode(dipPins[i], INPUT_PULLUP);      // set the digital pins (defined above) as input
  }
  delay(2000);

  lcd.clear();
  for (uint8_t i = 0; i<=3; i++){
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
 j = (j << 1) | !digitalRead(dipPins[i+(4*x)]);   // read each input pin
 }
 return j; //return address
}
void displayInput(int v, midi::MidiType w, byte x, byte y, int z) {
  lcd.setCursor(2,v);
  // if (w < 100) { lcd.print(" ");}
  // if (w < 10) { lcd.print(" ");}
  // lcd.print(w);
  if (w == 144) {
    lcd.print("On ");
  }
  else if (w == 128) {
    lcd.print("Off");
  }
  lcd.print(" ");
  if (x < 100) { lcd.print(" ");}
  if (x < 10) { lcd.print(" ");}
  lcd.print(x);
  lcd.print(" ");
  if (y < 100) { lcd.print(" ");}
  if (y < 10) { lcd.print(" ");}
  lcd.print(y);
  lcd.print(" ");
  if (z < 10) { lcd.print(" ");}
  lcd.print(z);

}

void displayOutput(int w, int x, int y, int z) {
  lcd.setCursor(18,0);
  lcd.print(w);
  lcd.print(" ");
  lcd.setCursor(18,1);
  lcd.print(x);
  lcd.print(" ");
  lcd.setCursor(18,2);
  lcd.print(y);
  lcd.print(" ");
  lcd.setCursor(18,3);
  lcd.print(z);
  lcd.print(" ");
}

void loop() {

  // midi1.sendNoteOn(42, 127, 1);    // Send a Note (pitch 42, velo 127 on channel 1)
  // delay(1000);		            // Wait for a second
  // midi1.sendNoteOff(42, 0, 1);     // Stop the note
  // delay(1000);

  //  Display output channels on display
  for (uint8_t i = 0; i <= 3; i++) {
    transmitChannel[i] = address(i);
  }
  displayOutput(byte(transmitChannel[0]), byte(transmitChannel[1]), transmitChannel[2], transmitChannel[3]);

  //  Read MIDI inputs
  if (midi1.read()) {
    midiType = midi1.getType();
    midiData1 = midi1.getData1();
    midiData2 = midi1.getData2();
    if (transmitChannel[0] == 0) {midiChannel = midi1.getChannel();} else {midiChannel = transmitChannel[0];}

    midi1.send(midiType, midiData1, midiData2, midiChannel);
    if (midiType != 254) {
      displayInput(0, midiType, midiData1, midiData2, midiChannel);
    }
    // lcd.setCursor(5,0);
    // lcd.print("On");
    // delay(1000);
    // lcd.clear();
  }
  // if (midi2.read()) {
  //   midiType = midi2.getType();
  //   midiData1 = midi2.getData1();
  //   midiData2 = midi2.getData2();
  //   if (transmitChannel[1] == 0) {midiChannel = midi2.getChannel();} else {midiChannel = transmitChannel[1];}

  //   midi1.send(midiType, midiData1, midiData2, midiChannel);
  //   displayInput(1, midiType, midiData1, midiData2, midiChannel);
  // }
  // if (midi3.read()) {
  //   midiType = midi3.getType();
  //   midiData1 = midi3.getData1();
  //   midiData2 = midi3.getData2();
  //   if (transmitChannel[2] == 0) {midiChannel = midi3.getChannel();} else {midiChannel = transmitChannel[2];}

  //   midi1.send(midiType, midiData1, midiData2, midiChannel);
  //   displayInput(2, midiType, midiData1, midiData2, midiChannel);
  // }
  // if (midi4.read()) {
  //   midiType = midi4.getType();
  //   midiData1 = midi4.getData1();
  //   midiData2 = midi4.getData2();
  //   if (transmitChannel[3] == 0) {midiChannel = midi4.getChannel();} else {midiChannel = transmitChannel[3];}

  //   midi1.send(midiType, midiData1, midiData2, midiChannel);
  //   displayInput(3, midiType, midiData1, midiData2, midiChannel);
  // }
}