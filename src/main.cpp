#include <Arduino.h>
#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, midiA)
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiB)
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, midiC)
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, midiD)


int dipPins[] = {26, 28, 30, 32, 34, 36, 38, 40, 27, 29, 31, 33, 35, 37, 39, 41};
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

  int i;
  for(i = 0; i<=15; i++){
    pinMode(dipPins[i], INPUT_PULLUP);      // set the digital pins (defined above) as input
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

void loop() {
  transAChannel = address(0);
  transBChannel = address(1);
  transCChannel = address(2);
  transDChannel = address(3);
  if (midiA.read()) {
    if (transAChannel == 0) {
      transAChannel = midiA.getChannel();
    }
    midiA.send(midiA.getType(),
               midiA.getData1(),
               midiA.getData2(),
               transAChannel == 0 ? midiA.getChannel() : transAChannel);
  }
  if (midiB.read()) {
    midiA.send(midiB.getType(),
               midiB.getData1(),
               midiB.getData2(),
               transBChannel == 0 ? midiB.getChannel() : transBChannel);
  }
  if (midiC.read()) {
    midiA.send(midiC.getType(),
               midiC.getData1(),
               midiC.getData2(),
               transCChannel == 0 ? midiC.getChannel() : transCChannel);
  }
  if (midiD.read()) {
    midiA.send(midiD.getType(),
               midiD.getData1(),
               midiD.getData2(),
               transDChannel == 0 ? midiD.getChannel() : transDChannel);
  }  
}



