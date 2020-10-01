// FIXME: drone samples from 444 header
const int drone_len = 72;
volatile uint16_t  drone_idx = 0;
unsigned char drone_sw [72] =
{ 49, 60, 70, 70, 59, 53, 59, 66, 67, 59, 48, 41, 44, 56, 64, 56,
  44, 41, 44, 45, 38, 31, 34, 40, 43, 47, 50, 52, 51, 46, 44, 51, 56, 55, 48, 43,
  45, 53, 63, 65, 57, 51, 55, 59, 62, 62, 54, 42, 37, 44, 51, 46, 35, 34, 41, 48,
  49, 47, 48, 49, 49, 52, 57, 56, 47, 36, 35, 46, 55, 56, 50, 45 };

const int tone_lg_len = 19;
volatile uint16_t  tone_lg_idx = 0;
const unsigned char tone_lg_sw [19] = 
{ 100,179,125,121,147,155,139,124,134,145,72,55,77,65,48,47,69,84,32 };

const int tone_la_len = 17;
volatile uint16_t  tone_la_idx = 0;
const unsigned char tone_la_sw [17] = 
{ 100,176,116,132,154,146,127,130,145,63,63,75,57,44,62,85,32 };

const int tone_b_len = 15;
volatile uint16_t  tone_b_idx = 0;
const unsigned char tone_b_sw [15] = 
{ 92,110,125,135,137,132,120,103,85,67,53,46,47,55,69 };

const int tone_cnat_len = 14;
volatile uint16_t  tone_cnat_idx = 0;
const unsigned char tone_cnat_sw [14] = 
{ 92,111,127,136,137,129,114,95,75,58,48,46,52,66 };

const int tone_c_len = 13;
volatile uint16_t  tone_c_idx = 0;
const unsigned char tone_c_sw [13] = 
{ 92,112,128,137,136,126,109,89,69,53,46,48,59 };

const int tone_d_len = 12;
volatile uint16_t  tone_d_idx = 0;
const unsigned char tone_d_sw [12] = 
{ 92,113,130,137,134,121,101,79,60,48,46,55 };

const int tone_e_len = 11;
volatile uint16_t  tone_e_idx = 0;
const unsigned char tone_e_sw [11] =
{ 92,116,132,137,129,109,85,62,48,47,59 };

const int tone_fnat_len = 10;
volatile uint16_t  tone_fnat_idx = 0;
const unsigned char tone_fnat_sw [10] = 
{ 92,117,134,137,124,101,75,54,46,53 };

const int tone_f_len = 10;
volatile uint16_t  tone_f_idx = 0;
const unsigned char tone_f_sw [10] = 
{ 92,118,135,136,121,96,69,50,46,59 };

const int tone_hg_len = 9;
volatile uint16_t  tone_hg_idx = 0;
const unsigned char tone_hg_sw [9] = 
{ 92,120,136,134,114,84,58,46,53 };

const int tone_ha_len = 8;
volatile uint16_t  tone_ha_idx = 0;
const unsigned char tone_ha_sw [8] = 
{ 92,122,137,129,101,69,48,49 };


