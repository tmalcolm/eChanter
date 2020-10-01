/*
    eChanter, Tim Malcolm 2010-2015 CC by-nc-sa.
*/
/*  Audio based on the Mozzi Synth libraries and examples
    Download and install from http://sensorium.github.com/Mozzi/
    Mozzi, Tim Barrass 2012, CC by-nc-sa.
*/

/*
    Circuit Diagrams  https://sourceforge.net/p/echanter/code/HEAD/tree/diagrams

    Finger Sensor inputs
      D2 -> LA
      D3 -> B
      D4 -> C
      D5 -> D
      D6 -> E
      D7 -> F
      D8 -> HG
      D12-> HA

    Pressure Sensor input
      A0

    Audio Outputs
      D9 in standard mode
      D9 + D10 in HIFI mode

*/

//#include <ADC.h>  // Teensy 3.0/3.1 uncomment this line and install http://github.com/pedvide/ADC
#include <MozziGuts.h>
#include <Sample.h> // Sample template

#include "echanter_config.h"
#if INSTRUMENT == GHB
#include "ghb.h"
#else
#include "pc.h"
#endif

#define CONTROL_RATE 256 //512 // 64 // powers of 2 please
#define PRESSURE_INPUT_PIN  0 // set the input for the input to analog pin 0

Sample <INST_NUM_CELLS, AUDIO_RATE>instrument(INST_DATA);
#if INSTRUMENT == GHB
Sample <DRONE_NUM_CELLS, AUDIO_RATE>drone(DRONE_DATA);
#endif

int curr_sensor = 0;

// for volume control
unsigned char vol = 255;
bool pressure_sensor = false;
int pressure_min = 0;

byte fmap = 0;  //D2-D7 (bits 2,3,4,5,6,7)
int i, j, k = 0; //general counters
int note_detected = 0;
int note_playing = 0;
int pressure_delta = 0;


void setup(){

#if CAPTOUCH
#else
  pinMode(2,INPUT);pinMode(3,INPUT);pinMode(4,INPUT);pinMode(5,INPUT);
  pinMode(6,INPUT);pinMode(7,INPUT);pinMode(8,INPUT);pinMode(12,INPUT);
#endif

  #if SERIAL_DEBUG
    Serial.begin(115200);
  #endif

#if CAPTOUCH // compiler didn't like narrower defs so we have code repetition :(
  if (readCapacitivePin(sensor_pins[3]) >= CAPTOUCH_TRIGGER) { /* D sensor touched */
    pressure_sensor = true;
    #if SERIAL_DEBUG
      Serial.println("Pressure Sensor ON");
    #endif

    while (readCapacitivePin(sensor_pins[3]) >= CAPTOUCH_TRIGGER) {
       pressure_min = analogRead(PRESSURE_INPUT_PIN); // value is 0-1023
       #if SERIAL_DEBUG
         Serial.print("Pressure value: ");
         Serial.println(pressure_min);
       #endif

    }
   }
#else
  if (digitalRead(sensor_pins[3]) == LOW) {  /* D sensor touched */
    pressure_sensor = true;
    #if SERIAL_DEBUG
      Serial.println("Pressure Sensor ON");
    #endif

    while (digitalRead(sensor_pins[3]) == LOW) {
       pressure_min = analogRead(PRESSURE_INPUT_PIN); // value is 0-1023
       #if SERIAL_DEBUG
         Serial.print("Pressure value: ");
         Serial.println(pressure_min);
       #endif

    }
   }
#endif


  set_freqs(INST_SAMPLERATE/INST_NUM_CELLS);

  startMozzi(CONTROL_RATE); // set a control rate of 64 (powers of 2 please)

  instrument.setLoopingOn();
  instrument.setFreq(note_freqs[0]); // set the frequency

#if INSTRUMENT == GHB
  drone.setLoopingOn();
  drone.setFreq((float)INST_SAMPLERATE/DRONE_NUM_CELLS); // set the frequency
#endif

}

void set_freqs(float f) {
  #if SERIAL_DEBUG
    Serial.println("Frequency table:");
  #endif

  // setup freq. table
  for (i=0; i<11; i++) {
    note_freqs[i] = (f*note_ratios[i][0])/note_ratios[i][1];
    #if SERIAL_DEBUG
      Serial.println(note_freqs[i]);
    #endif
   }


}


void updateControl(){

  byte fb = 0;

  int sensor_val = 0;
  // put changing controls in here
  if (pressure_sensor) {
    sensor_val = mozziAnalogRead(PRESSURE_INPUT_PIN); // value is 0-1023

    // volume is on or off based on sensor input.
    // for instruments with dynamics (eg clarinet) uncomment mappings below
    // map it to an 8 bit range for efficient calculations in updateAudio
    // vol = map(sensor_value, 0, 1023, 0, 255);

    if (sensor_val >= pressure_min) vol = 255;
    else vol = 0;

    // update frequency delta to simulate over/under blowing

  }

  /* Read the relevant pin registers and construct a single byte 'map'   */
  /* of the pin states. Touched pins will be HIGH, untouched pins LOW    */


#if CAPTOUCH
  // set map
  if (readCapacitivePin(sensor_pins[curr_sensor]) >= CAPTOUCH_TRIGGER) {
    // make bit to be 1
    fmap |= (1 << (7-curr_sensor));
  }
  curr_sensor++;

  //increment, return until all sensors are sampled, then toggle bitmap, reset counter and continue
  if (curr_sensor % num_sensors == 0) {
    fmap = ~fmap; // toggle bitmap because bushbutton code is LOW when touched.
    curr_sensor = 0; // rset counter
  } else {
    return;
  }

    #if SERIAL_DEBUG
      Serial.print("FMAP ");
      Serial.println(fmap, BIN);
    #endif


#else // pushbuttonetc, finger sensors

  fmap = PIND >> 2;  // get rid of lowest 2 bytes,  fill top 2 bytes

  #if SERIAL_DEBUG
    Serial.print(fmap, BIN);
    Serial.print("  ");
  #endif

  fb = PINB; // D8, D11 (bits 0, 3)
  #if SERIAL_DEBUG
    Serial.print(fb, BIN);
    Serial.print("  ");
  #endif

  if (fb & B00000001) { // true only if bit ZERO is 1
    // make  bit 6 in fmap to be 1
    fmap |= (1 << 6);
  } else {
    // untouched make bit 6 to be 0
    fmap &= ~(1 << 6);
  }

  if (fb & B00010000) { // true only if bit 3 is 1
    // make  bit 7 in fmap to be 1
    fmap |= (1 << 7);
  } else {
    // untouched make bit 7 to be 0
    fmap &= ~(1 << 7);
  }

  #if SERIAL_DEBUG
    Serial.println(fmap, BIN);
  #endif

#endif

  note_detected = -1;

  for (i=0; i< table_len; i++) {
    if ((fmap ^ finger_table[i]) == 0) {
      note_detected = i;
    }
  }
  // gracenotes
  if (note_detected == -1) {
    if ((fmap >> 7) & 1 ) { note_detected = 0; }
    else if ((fmap >> 6) & 1 ) { note_detected = 2; }
    else if ((fmap >> 5) & 1 ) { note_detected = 4; }
    else if ((fmap >> 4) & 1 ) { note_detected = 5; }
    else if ((fmap >> 3) & 1 ) { note_detected = 6; }
    else if ((fmap >> 2) & 1 ) { note_detected = 7; }
    else if ((fmap >> 1) & 1 ) { note_detected = 8; }
    else if ((fmap >> 0) & 1 ) { note_detected = 9; }
    else { /* LG or no note? */ note_detected = 10; }
  }


  if (note_detected != note_playing) {
    note_playing =  note_detected;
    instrument.setFreq(note_freqs[note_playing] + pressure_delta);

  }

  // reset fingermap
  fmap = 0;

}


int updateAudio(){

  if (pressure_sensor) {
    if(AUDIO_MODE == HIFI)
      #if INSTRUMENT == GHB
      return ((int)instrument.next() + drone.next())<<4;
      #else
      return ( ( instrument.next() * vol)>>2);
      #endif

    else 
      return ( ( instrument.next() * vol)>>8);
  
  } else {
    if(AUDIO_MODE == HIFI)
       #if INSTRUMENT == GHB
       return ((int)instrument.next() + drone.next())<<4;
       #else
       return ( ((int)instrument.next() * vol)>>2); 
       #endif
      
    else
      return ( ( instrument.next() * vol)>>8); 
  }

// original unmodified signal
//return aSin.next(); // return an int signal centred around 0


}


void loop(){
  audioHook(); // required here


}

#if CAPTOUCH

// captouch code from eChanter, unrolled to be faster and more reliable, courtesy of
//  Danial Martinez, GPL v2
//  https://github.com/danielmartinez/eClarin
//
uint8_t readCapacitivePin(int pinToMeasure) {
  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  port = portOutputRegister(digitalPinToPort(pinToMeasure));
  ddr = portModeRegister(digitalPinToPort(pinToMeasure));
  bitmask = digitalPinToBitMask(pinToMeasure);
  pin = portInputRegister(digitalPinToPort(pinToMeasure));
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
 // delay(1);
  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;

  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.
  uint8_t cycles = 17;
       if (*pin & bitmask) { cycles =  0;}
  else if (*pin & bitmask) { cycles =  1;}
  else if (*pin & bitmask) { cycles =  2;}
  else if (*pin & bitmask) { cycles =  3;}
  else if (*pin & bitmask) { cycles =  4;}
  else if (*pin & bitmask) { cycles =  5;}
  else if (*pin & bitmask) { cycles =  6;}
  else if (*pin & bitmask) { cycles =  7;}
  else if (*pin & bitmask) { cycles =  8;}
  else if (*pin & bitmask) { cycles =  9;}
  else if (*pin & bitmask) { cycles = 10;}
  else if (*pin & bitmask) { cycles = 11;}
  else if (*pin & bitmask) { cycles = 12;}
  else if (*pin & bitmask) { cycles = 13;}
  else if (*pin & bitmask) { cycles = 14;}
  else if (*pin & bitmask) { cycles = 15;}
  else if (*pin & bitmask) { cycles = 16;}

  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;

  #if SERIAL_DEBUG
    if (cycles >= CAPTOUCH_TRIGGER) {
      Serial.print("Sensor::");
      Serial.print(pinToMeasure);
      Serial.print("::");
      Serial.print(cycles);
      Serial.print(" TOUCHED");
      Serial.println();
    }
  #endif
  return cycles;
}

#endif

