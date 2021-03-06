/* Malcolm PC Low A, sample length                  */
/* Sample range [-127, 127] for use with Mozzi Synth     */
/* For independant Arduino/PWM use add 127 to each value */

#define INST_NUM_CELLS 193
#define INST_SAMPLERATE 44100

const int8_t __attribute__((section(".progmem.data"))) INST_DATA []  =
/* Malcolm PC */
{0, 7, 11, 9, 5, 2, 3, 3, 1, -2, -5, -7, -9, -10, -9, -6, -1, 4, 7, 12, 19, 22, 16, 8, 2,
-1, -1, 0, 2, 4, 5, 5, 6, 11, 18, 21, 18, 12, 6, 1, -3, -7, -10, -12, -15, -20, -21, -19,
-18, -22, -23, -18, -10, -8, -13, -18, -18, -16, -17, -18, -21, -26, -31, -32, -29, -31,
-42, -56, -63, -66, -71, -79, -87, -92, -94, -93, -92, -96, -103, -107, -100, -85, -66,
-46, -26, -6, 5, -1, -17, -27, -22, -7, 2, 4, 3, 0, -8, -14, -8, 5, 14, 15, 16, 20, 25,
25, 20, 15, 14, 12, 6, -1, -5, -6, -9, -12, -10, -4, 3, 7, 9, 11, 12, 7, -8, -24, -32,
-33, -33, -30, -22, -10, 2, 9, 17, 28, 36, 34, 24, 14, 6, 3, 6, 11, 17, 21, 25, 30, 35,
41, 43, 38, 28, 23, 23, 22, 18, 14, 16, 22, 29, 37, 46, 54, 58, 61, 67, 76, 83, 84, 77,
70, 66, 65, 67, 76, 89, 97, 90, 71, 46, 27, 18, 15, 15, 15, 10, -3, -18, -28, -30, -23,
-11};

