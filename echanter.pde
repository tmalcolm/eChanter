#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

// #include "samples_444.h"		// sample wave tables for GHB at 444Hz
#include "samples_470.h"		// sample wave tables for GHB at 470Hz


//#define DORKBOARD   true // uncomment for Dorkboard specific LED behavior
#define SERIAL_DEBUG false // true/false to enable/desiable serial print debugging

#define SAMPLE_RATE 8000

#if defined(__AVR_ATmega328P__)

  // sensor key analog 0-5
  #define KEYA0    PORTC0        // capture input - analog 0
  #define KEYA1    PORTC1        // capture input - analog 1
  #define KEYA2    PORTC2        // capture input - analog 2
  #define KEYA3    PORTC3        // capture input - analog 3
  #define KEYA4    PORTC4        // capture input - analog 4
  #define KEYA5    PORTC5        // capture input - analog 5

  // sensor key digital 2-7
  //#define KEY0    PORTD0        // capture input - digital 0
  //#define KEY1    PORTD1        // capture input - digital 1
  #define KEY2    PORTD2        // capture input - digital 2
  #define KEY3    PORTD3        // capture input - digital 3
  #define KEY4    PORTD4        // capture input - digital 4
  #define KEY5    PORTD5        // capture input - digital 5
  #define KEY6    PORTD6        // capture input - digital 6
  #define KEY7    PORTD7        // capture input - digital 7

  // sensor key digital 8-13
  #define KEY8    PORTB0        // capture input - digital 8
  #define KEY9    PORTB1        // capture input - digital 9
  #define KEY10   PORTB2        // capture input - digital 10
  #define KEY11   PORTB3        // capture input - digital 11
  #define KEY12   PORTB4        // capture input - digital 12
  #define KEY13   PORTB5        // capture input - digital 13

#else

  // sensor key analog 0-5
  #define KEYA0    PC0        // capture input - analog 0
  #define KEYA1    PC1        // capture input - analog 1
  #define KEYA2    PC2        // capture input - analog 2
  #define KEYA3    PC3        // capture input - analog 3
  #define KEYA4    PC4        // capture input - analog 4
  #define KEYA5    PC5        // capture input - analog 5

  // sensor key digital 2-7
  //#define KEY0    PD0        // capture input - digital 0
  //#define KEY1    PD1        // capture input - digital 1
  #define KEY2    PD2        // capture input - digital 2
  #define KEY3    PD3        // capture input - digital 3
  #define KEY4    PD4        // capture input - digital 4
  #define KEY5    PD5        // capture input - digital 5
  #define KEY6    PD6        // capture input - digital 6
  #define KEY7    PD7        // capture input - digital 7

  // sensor key digital 8-13
  #define KEY8    PB0        // capture input - digital 8
  #define KEY9    PB1        // capture input - digital 9
  #define KEY10   PB2        // capture input - digital 10
  #define KEY11   PB3        // capture input - digital 11
  #define KEY12   PB4        // capture input - digital 12
  #define KEY13   PB5        // capture input - digital 13

  #define DRONE_SENSOR_PIN 0 // analog pin 0
  #define TRIGGER_VAL 2 // sensor trigger value (<val is on, >= val is off)
  #define DRONE_AMPLITUDE_CORRECTION 40 // temp correction to adjust for drone wave table amplitude

#endif

#define DRONE_SENSOR_PIN 0 // analog pin 0
#define TRIGGER_VAL 2 // sensor trigger value (<val is on, >= val is off)
#define DRONE_AMPLITUDE_CORRECTION 40 // temp correction to adjust for drone wave table amplitude


/* AUDIO/VISUAL OUTPUT PINS */
int ledPin = 13;
int speakerPin = 11;

// each wave sample gets mixed into the 'sample' variable which is sent to the PWM output
int mixed_sample = 0;

/* SENSOR RELATED VARIABLES */
int capvalD[6] = {100,100,100,100,100,100};
int capvalB[2] = {100,100};
int capvalC[3] = {100,100,100};
char pinvalD[6] = {1<<KEY2,1<<KEY3,1<<KEY4,1<<KEY5,1<<KEY6,1<<KEY7};
char pinvalB[2] = {1<<KEY8,1<<KEY9};
char pinvalC[3] = {1<<KEYA4,1<<KEYA5,1<<KEYA3};

int noteTone = 8; // current tone playing
int tv = 8; // temp var for holding tone to play

const int numMenus = 1; // number of configuration menus - better as a define
int currentMenu = 0; // the menu number currently being accessed
int menuStateUp = 0; // state var for "menu up"
int menuStateDown = 0;  // state var for "menu up"

int steadyDrone = 0; // the sensor level that represents "steady" pressure
int droneVal = 0; // the sensor reading
boolean drones = false;
boolean dronesOn = false;
boolean strictDrones = false;
boolean droneSensor = false;
float strictDronesMultiplier = 0.01;
float lazyDronesMultiplier = 0.1;


void setup()
{
  #ifdef DRONE_AMPLITUDE_CORRECTION
  int i=0;
  for (;i<drone_len;i++) {
    // slightly dominant bass
    drone_sw [i] += DRONE_AMPLITUDE_CORRECTION;
  }
  #endif
  
  pinMode(ledPin,OUTPUT);
  #if SERIAL_DEBUG
  Serial.begin(9600);      // connect to the serial por//t
  Serial.println("ready....");
  #endif
  startPlayback();
}

void loop ()
{

  char i=0;
  while (1==1) {
    tv = 8;

    capvalC[0] = getcapPC(pinvalC[0]);
    if (capvalC[0] > TRIGGER_VAL) {
      stopPlayback();
      delay(500);
      #if SERIAL_DEBUG
      Serial.print("menu pin # A4");
      Serial.print(" capval = ");
      Serial.println(capvalC[0]);
      #endif
      currentMenu = 1;
      config();
      #if SERIAL_DEBUG
      Serial.print("configuration complete");
      #endif
      delay(500);
      startPlayback();
    }

    for(i = 0; i < 6; i++)
    {
      capvalD[i] = getcapPD(pinvalD[i]);
      #if SERIAL_DEBUG
      Serial.print("pin # ");
      Serial.print(i+0);
      Serial.print(" capval = ");
      Serial.println(capvalD[i]);
      #endif
      
      if (capvalD[i] <= TRIGGER_VAL) {
        tv=i;
        i=15;  
      }
    }
    if (tv == 8) {
      for(i = 0; i < 2; i++)
      {
        capvalB[i] = getcapPB(pinvalB[i]);
        #if SERIAL_DEBUG
        Serial.print("pin # ");
        Serial.print(i+6);
        Serial.print(" capval = ");
        Serial.println(capvalB[i]);
        #endif
        
        if (capvalB[i] <= TRIGGER_VAL) {
          tv=i+6;
          i=15;  
        }
      }
    }
    noteTone = tv;

    if (drones && droneSensor) {
      droneVal = analogRead(DRONE_SENSOR_PIN);
      #if SERIAL_DEBUG
      Serial.print("drone sensor val = ");
      Serial.print(droneVal);
      #endif

      float range = (strictDrones) ? strictDronesMultiplier : lazyDronesMultiplier ;
      // set the drones on or off
      dronesOn = ( (droneVal < steadyDrone*(1.0-range) ) ||  (droneVal > steadyDrone*(1.0+range)) ) ? false : true;
    }
        
    #if SERIAL_DEBUG
    Serial.println("");
    #endif
  }
}

/* CONFIGURATION MENU CODE */
void config() {
  char i = 0;
  while (currentMenu > 0) {
    for(i = 0; i < 3; i++) {
      capvalC[i] = getcapPC(pinvalC[i]);
    }
    if (capvalC[0] > TRIGGER_VAL) {
      currentMenu = (numMenus == currentMenu) ? 0 : currentMenu++ ;
      ledOff();
      if (currentMenu == 0) {    // reset menu states
        menuStateUp = 0;
        menuStateDown = 0;
      }
      #if SERIAL_DEBUG
      Serial.print("CURRENT MENU:");
      Serial.println(currentMenu);
      #endif
    } else if (capvalC[1] > TRIGGER_VAL) {
      ledOff();
      menuChoiceUp();
    } else if (capvalC[2] > TRIGGER_VAL) {
      ledOff();
      menuChoiceDown();
    }
    delay(500);
  }
}


void menuChoiceUp() {
  switch (currentMenu) {
    case 1:
      drones = (drones) ? false : true;
      dronesOn = drones;
      if (drones) ledOn();
      else ledOff();
      #if SERIAL_DEBUG
      Serial.print("DRONES ON:");
      Serial.println(drones, DEC);
      #endif
      steadyDrone = analogRead(DRONE_SENSOR_PIN);
      #if SERIAL_DEBUG
      Serial.print("  DRONES SENSOR:");
      Serial.println(steadyDrone);
      #endif
      break;
    case 2:
        break;
       default: break;
  }
       
}

void menuChoiceDown() {
  switch (currentMenu) {
    case 1:
      switch (menuStateDown) {
        case 0: 
                droneSensor = true; 
                strictDrones = true;
                flashLed(1,300);
                break;
        case 1: 
                droneSensor = true; 
                strictDrones = false; // ie "lazy mode"
                flashLed(2,300);
                break;
        case 2: 
                droneSensor = false; // ie no sensor - always on
                flashLed(3,300);
                break;
      }
      #if SERIAL_DEBUG
      Serial.print("DRONE STRICT MODE:");
      Serial.println(strictDrones, DEC);
      Serial.print("DRONE SENSOR IN USE:");
      Serial.println(droneSensor, DEC);
      #endif
      if (menuStateDown > 2) menuStateDown = 0;
      else menuStateDown++;
      break;
    case 2:
        break;
       default: break;
  }
       
}


/* UTILITY FUNCTIONS */

// flashes LED on then off for the given number of millis
// eg. flashLed(3,100) will do 3 cycles of : flash the LED on for 100ms, off for 100ms
void flashLed(int count, int ms) {
  for (int i =0; i<count;i++) {
    #ifdef DORKBOARD
    digitalWrite(ledPin,LOW);
    delay(ms);
    digitalWrite(ledPin,HIGH);
    delay(ms);
    #endif
    #ifndef DORKBOARD
    digitalWrite(ledPin,HIGH);
    delay(ms);
    digitalWrite(ledPin,LOW);
    delay(ms);
    #endif
  }
}  

void ledOn() {
    #ifdef DORKBOARD
    digitalWrite(ledPin,LOW);
    #endif
    #ifndef DORKBOARD
    digitalWrite(ledPin,HIGH);
    #endif
}  

void ledOff() {
    #ifdef DORKBOARD
    digitalWrite(ledPin,HIGH);
    #endif
    #ifndef DORKBOARD
    digitalWrite(ledPin,LOW);
    #endif
}  


/* SENSOR MEASUREMENT CODE */

// returns capacity on one input pin
// pin must be the bitmask for the pin e.g. (1<<PB0)
char getcapPD(char pin)
{
  char i = 0;
  DDRD &= ~pin;          // input
  PORTD |= pin;          // pullup on
  for(i = 0; i < 16; i++)
    if( (PIND & pin) ) break;
  PORTD &= ~pin;         // low level
  DDRD |= pin;           // discharge
  return i;
}

// returns capacity on one input pin
// pin must be the bitmask for the pin e.g. (1<<PB0)
char getcapPC(char pin)
{
  char i = 0;
  DDRC &= ~pin;          // input
  PORTC |= pin;          // pullup on
  for(i = 0; i < 16; i++)
    if( (PINC & pin) ) break;
  PORTC &= ~pin;         // low level
  DDRC |= pin;           // discharge
  return i;
}


// returns capacity on one input pin
// pin must be the bitmask for the pin e.g. (1<<PB0)
char getcapPB(char pin)
{
  char i = 0;
  DDRB &= ~pin;          // input
  PORTB |= pin;          // pullup on
  for(i = 0; i < 16; i++)
    if( (PINB & pin) ) break;
  PORTB &= ~pin;         // low level
  DDRB |= pin;           // discharge
  return i;
}

/* PWM AUDIO CODE : This is called at 8000 Hz to load the next sample. */
ISR(TIMER1_COMPA_vect) {
  if (tone_ha_idx == tone_ha_len) tone_ha_idx = 0;
  if (tone_hg_idx == tone_hg_len) tone_hg_idx = 0;
  if (tone_f_idx == tone_f_len) tone_f_idx = 0;
  if (tone_e_idx == tone_e_len) tone_e_idx = 0;
  if (tone_d_idx == tone_d_len) tone_d_idx = 0;
  if (tone_c_idx == tone_c_len) tone_c_idx = 0;
  if (tone_b_idx == tone_b_len) tone_b_idx = 0;
  if (tone_la_idx == tone_la_len) tone_la_idx = 0;
  if (tone_lg_idx == tone_lg_len) tone_lg_idx = 0;
  if (drone_idx == drone_len) drone_idx = 0;

  if (noteTone == 8) { //LG
    mixed_sample = tone_lg_sw[tone_lg_idx];
  } else if (noteTone == 7) { // LA
    mixed_sample = tone_la_sw[tone_la_idx];
  } else if (noteTone == 6) { // B
    mixed_sample = tone_b_sw[tone_b_idx];
  } else if (noteTone == 5) { // C
    mixed_sample = tone_c_sw[tone_c_idx];
  } else if (noteTone == 4) { // D
    mixed_sample = tone_d_sw[tone_d_idx];
  } else if (noteTone == 3) { // E
    mixed_sample = tone_e_sw[tone_e_idx];
  } else if (noteTone == 2) { // F
    mixed_sample = tone_f_sw[tone_f_idx];
  } else if (noteTone == 1) { // HG
    mixed_sample = tone_hg_sw[tone_hg_idx];
  } else if (noteTone == 0) { // HA
    mixed_sample = tone_ha_sw[tone_ha_idx];
  }
  mixed_sample += 50; // dc offset correction
  if (dronesOn) mixed_sample = (mixed_sample + drone_sw[drone_idx]) / 2;

  OCR2A = mixed_sample;
  
  tone_ha_idx++;
  tone_hg_idx++;
  tone_f_idx++;
  tone_e_idx++;
  tone_d_idx++;
  tone_c_idx++;
  tone_b_idx++;
  tone_la_idx++;
  tone_lg_idx++;
  drone_idx++;
}


void startPlayback()
{
    pinMode(speakerPin, OUTPUT);

    // Set up Timer 2 to do pulse width modulation on the speaker
    // pin.

    // Use internal clock (datasheet p.160)
    ASSR &= ~(_BV(EXCLK) | _BV(AS2));

    // Set fast PWM mode  (p.157)
    TCCR2A |= _BV(WGM21) | _BV(WGM20);
    TCCR2B &= ~_BV(WGM22);

    // Do non-inverting PWM on pin OC2A (p.155)
    // On the Arduino this is pin 11.
    TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
    TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));

    // No prescaler (p.158)
    TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

    // Set initial pulse width to the first sample.
    OCR2A = tone_ha_sw[0];


    // Set up Timer 1 to send a sample every interrupt.

    cli();

    // Set CTC mode (Clear Timer on Compare Match) (p.133)
    // Have to set OCR1A *after*, otherwise it gets reset to 0!
    TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
    TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));

    // No prescaler (p.134)
    TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

    // Set the compare register (OCR1A).
    // OCR1A is a 16-bit register, so we have to do this with
    // interrupts disabled to be safe.
    OCR1A = F_CPU / SAMPLE_RATE;    // 16e6 / 8000 = 2000

    // Enable interrupt when TCNT1 == OCR1A (p.136)
    TIMSK1 |= _BV(OCIE1A);

    sei();
}

void stopPlayback()
{
    // Disable playback per-sample interrupt.
    TIMSK1 &= ~_BV(OCIE1A);

    // Disable the per-sample timer completely.
    TCCR1B &= ~_BV(CS10);

    // Disable the PWM timer.
    TCCR2B &= ~_BV(CS10);

    digitalWrite(speakerPin, LOW);
}


/* END PWM AUDIO CODE */

