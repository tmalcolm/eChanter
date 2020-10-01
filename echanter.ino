#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

// #include "samples_444.h"	// sample wave tables for GHB at 444Hz
//#include "samples_470.h"	// sample wave tables for GHB at 470Hz
#include "samples_pc.h"		// sample wave tables for PC at 466Hz
#include "samples_test.h"		// sample wave tables for PC at 466Hz


//#define DORKBOARD   true // uncomment for Dorkboard specific LED behavior
//#define SERIAL_DEBUG true // true/false to enable/desiable serial print debugging

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


void setup()
{
  
  pinMode(ledPin,OUTPUT);
  #if SERIAL_DEBUG
  Serial.begin(57600);      // connect to the serial por//t
  Serial.println("ready....");
  #endif
  startPlayback();
}


uint16_t phase_inc;
uint16_t counter = 0;


void set_freq(float freq) {
  //  http://codeandlife.com/2012/03/13/fast-dds-with-atmega88/  
  // Freq = ( (F_CPU / 11) / 256) * (step/256)
  // Solving this for step we get:
  // step = Freq * 256 * 256 * 11 / F_CPU

  // f = (8000/256) * step/256
  //   = 31.25*s/256
  // s = f * 256 / 31.25

  phase_inc = (uint16_t) ((freq * 256) / 31.25);

}


void loop ()
{

  char i=0;
  while (1==1) {
    tv = 8;

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


  if (noteTone == 8) { //LG
    set_freq(414.0);
  } else if (noteTone == 7) { // LA
    set_freq(466.0);
  } else if (noteTone == 6) { // B
    set_freq(524.0);
  } else if (noteTone == 5) { // C
    set_freq(583.0);
  } else if (noteTone == 4) { // D
    set_freq(621.0);
  } else if (noteTone == 3) { // E
    set_freq(699.0);
  } else if (noteTone == 2) { // F
    set_freq(777.0);
  } else if (noteTone == 1) { // HG
    set_freq(828.0);
  } else if (noteTone == 0) { // HA
    set_freq(932.0);
  }

    #if SERIAL_DEBUG
    Serial.println("");
    #endif
  }
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
/*
  if (idx == len) idx = 0;


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

*/
  counter += phase_inc;
  OCR2A = sample[counter >> 8];


//  OCR2A = sample[idx];
  
//  idx++;

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

