#define INST_NUM_CELLS 104 
#define DRONE_NUM_CELLS 365
#define INST_SAMPLERATE 44100

/* Kyo chanter 104 samples */
const int8_t __attribute__((section(".progmem.data"))) INST_DATA []  =
{1, 4, 7, 8, 9, 9, 5, -1, -5, -7, -5, -2, 2, 6, 11, 11, 10, 10, 12, 13, 12, 11, 11, 11, 12,
13, 9, -3, -14, -24, -36, -48, -49, -43, -36, -26, -14, -7, -3, 1, 0, -1, 7, 23, 35, 39,
39, 38, 33, 22, 12, 4, -3, -10, -14, -14, -12, -9, -4, -1, 0, 1, 3, 4, 3, 4, 6, 7, 6, 5,
1, -4, -6, -5, -5, -4, -3, -4, -5, -4, -2, 1, 4, 5, 5, 5, 4, 4, 4, 3, 1, 1, 1, 0, 0, 0,
-2, -5, -7, -7, -7, -8, -7, -4};

/* Gallaitry drones 365 samples */
const int8_t __attribute__((section(".progmem.data"))) DRONE_DATA []  =
{1, 5, 6, 6, 1, -5, -9, -13, -17, -19, -20, -21, -23, -24, -23, -20, -11, 3, 19, 31, 34,
32, 25, 15, 7, 4, 6, 13, 20, 22, 21, 16, 8, 0, -4, -5, -3, 0, 3, 3, 3, 2, 0, -1, -1, 0, 0,
-2, -7, -11, -10, -8, -7, -7, -6, -7, -11, -17, -22, -23, -22, -18, -12, -5, 2, 6, 8, 8,
10, 14, 16, 15, 13, 11, 10, 8, 3, -8, -19, -26, -30, -34, -39, -39, -36, -34, -32, -31,
-26, -20, -18, -21, -27, -28, -27, -25, -19, -16, -18, -19, -20, -23, -26, -23, -12, 0,
10, 21, 34, 44, 52, 55, 59, 66, 73, 79, 83, 80, 72, 64, 54, 45, 40, 41, 46, 52, 55, 55,
52, 45, 38, 32, 28, 24, 23, 25, 24, 22, 17, 13, 7, 1, -7, -15, -22, -29, -37, -44, -49,
-53, -56, -59, -62, -65, -67, -64, -58, -51, -43, -35, -32, -34, -37, -38, -39, -36, -27,
-19, -15, -15, -20, -29, -39, -46, -48, -49, -48, -45, -43, -43, -43, -42, -41, -39, -36,
-32, -28, -25, -24, -22, -20, -20, -20, -19, -20, -21, -22, -21, -16, -5, 10, 25, 35, 41,
44, 47, 47, 46, 50, 58, 66, 72, 73, 67, 56, 43, 32, 25, 21, 18, 18, 19, 20, 21, 19, 18,
21, 23, 24, 24, 20, 16, 12, 8, 3, -3, -9, -14, -20, -27, -33, -35, -31, -25, -18, -12, -5,
0, 4, 7, 9, 13, 19, 22, 23, 23, 20, 17, 13, 5, -7, -21, -32, -39, -47, -53, -55, -54, -52,
-51, -46, -37, -27, -19, -15, -12, -2, 9, 18, 24, 27, 26, 24, 22, 18, 16, 20, 31, 41, 45,
52, 61, 67, 74, 79, 84, 92, 103, 112, 114, 108, 95, 80, 67, 56, 50, 49, 52, 55, 55, 54,
49, 41, 35, 29, 25, 21, 18, 15, 9, 1, -7, -15, -23, -31, -37, -41, -43, -44, -44, -42,
-39, -36, -36, -38, -41, -44, -46, -46, -42, -34, -28, -25, -27, -35, -42, -47, -47, -41,
-30, -20, -14, -11, -11, -16, -21, -23, -22, -22, -19, -15, -10, -7, -6, -4};
