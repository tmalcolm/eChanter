/* eChanter configuration file */
/* SERIAL_DEBUG   true/false
 *    Toggle serial debugging output
 *    Recommended to be false for final upload
 *
 * CAPTOUCH       true/false
 *    true if using apacitive touch style sensors
 *    false if using touchpad or push button style switches
 *
 * CAPTOUCH_TRIGGER  0-16
 *    Values in the range 6-8 tend to work well
 *
 * INSTRUMENT
 * Can be "GHB" or "PC"
 *
 * BASE_FREQUENCY  Hz
 *
 * sensor_pins
 *    Digital inputs for sensors or buttons,
 *    in order from HA to LA.
 *
 * finger_table
 *    Finger mapings for various notes.
 *
 * drone_freqs
 *    Calculated at setup from base frequency for tenor
 *    and bass drones.
 *
 *
 * ==== DATA RESATIONSHIPS ====
 * For sensor pins, eChanter recognizes 8 pins which are polled
 * and used to set an 8bit 'touch map,' so it is important that
 * the HA to LA order is preserved.
 * NOTE: eChanter is coded to recognize LOW as a touch.
 *
 * Finger table, note ratios and note frequencies are all related
 * and should be in identical index positions in the appropriate array.
 * For example, HG (B10001110) is stored as finger_table[2],
 * the ratio of HG from the base frequency is 16:9 which is stored
 * as note_ratios[2]. At setup, the note ratio is used to calculate
 * the actual frequency for each note; HG is stored as note_freqs[2].
 * At runtime the pin states are compared to the finger table.
 * When playing HG, the array index for the HG finger map is used to
 * lookup that note's frequency in the note-freqs array, which is
 * then used to change the note being plated by changing Mozzi's
 * output frequency.
 *
 *
 */

#ifndef ECHANTER_CONFIG_H_
#define ECHANTER_CONFIG_H_
#define GHB 100
#define PC 200


#define SERIAL_DEBUG false

#define CAPTOUCH true
#define CAPTOUCH_TRIGGER 6

#define INSTRUMENT PC  
#define BASE_FREQUENCY 472

int num_sensors = 8;
int sensor_pins[] = {12,8,7,6,5,4,3,2};

int table_len = 11;
byte finger_table [11] = {
  B11100001, /*HA */
  B10100001, /*PA */
  B10001110, /*HG */
  B10101110, /*PG */
  B11001110, /* F */
  B11101110, /* E */
  B11110001, /* D */
  B11111001, /* C */
  B11111100, /* B */
  B11111110, /*LA */
  B11111111  /*LG */
};

int note_ratios [11][2]= {
  {2,1}, /*HA */
  {2,1}, /*PA */
  {16,9}, /*HG */
  {16,9}, /*PG */
  {5,3}, /* F */
  {3,2}, /* E */
  {4,3}, /* D */
  {5,4}, /* C */
  {9,8}, /* B */
  {1,1}, /*LA */
  {8,9}  /*LG */
};

float note_freqs [11]= {1,1,1,1,1,1,1,1,1,1,1};

#endif /* ECHANTER_CONFIG_H_ */
