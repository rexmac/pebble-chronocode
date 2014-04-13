/**
 *   0123456789ab
 * 0 KLOKKENpERyt
 * 1 shTJUEcmfTIx
 * 2 uFEMziKVARTj
 * 3 deterenpbble
 * 4 OVERPÅpHALVz
 * 5 TOhELLEVEbTI
 * 6 TREbTOLVuFEM
 * 7 sFIREcNIrSJU
 * 8 ENlSEKSyÅTTE
 */

enum {
  // no_NO
  LAYER_NB_NO_KLOKKEN = 13,
  LAYER_NB_NO_ER      = 14,
  LAYER_NB_NO_FEM     = 15,
  LAYER_NB_NO_TI      = 16,
  LAYER_NB_NO_KVART   = 17,
  LAYER_NB_NO_TJUE    = 18,
  LAYER_NB_NO_HALV    = 19,
  LAYER_NB_NO_OVER    = 20,
  LAYER_NB_NO_PA       = 21,

  LAYER_FILLER = 27
};

/**
 * Array of array of word_t structs required to create all the phrases for designating time.
 *
 */
static const word_t words[54] = {
  // Hours 1-12
  [1]  = { 8,   0, "EN", "en" },
  [2]  = { 5,   0, "TO", "to" },
  [3]  = { 6,   0, "TRE", "tre" },
  [4]  = { 7,   1, "FIRE", "fire" },
  [5]  = { 6,   9, "FEM", "fem" },
  [6]  = { 8,   3, "SEKS", "seks" },
  [7]  = { 7,   9, "SJU", "sju" },
  [8]  = { 8,   8, "ÅTTE", "åtte" },
  [9]  = { 7,   6, "NI", "ni" },
  [10] = { 5,  10, "TI", "ti" },
  [11] = { 5,   3, "ELLEVE", "elleve" },
  [12] = { 6,   4, "TOLV", "tolv" },

  // Minutes before/after the hour
  [LAYER_NB_NO_FEM]   = { 2, 1, "FEM", "fem" },
  [LAYER_NB_NO_TI]    = { 1, 9, "TI", "ti" },
  [LAYER_NB_NO_KVART] = { 2, 6, "KVART", "kvart" },
  [LAYER_NB_NO_TJUE]  = { 1, 2, "TJUE", "tjue" },
  [LAYER_NB_NO_HALV]  = { 4, 7, "HALV", "halv" },

  // Relative
  [LAYER_NB_NO_KLOKKEN]  = { 0, 0, "KLOKKEN", "klokken" },
  [LAYER_NB_NO_ER]       = { 0, 8, "ER", "er" },
  [LAYER_NB_NO_PA]       = { 4, 4, "PÅ", "på" },
  [LAYER_NB_NO_OVER]     = { 4, 0, "OVER", "over" },

  // Extra, "random" characters used to fill in empty spaces between words
  [LAYER_FILLER] =
    { 0,  7, "P", "p" },
    { 0, 10, "YT", "yt" },
    { 1,  0, "SH", "sh" },
    { 1,  6, "CMF", "cmf" },
    { 1, 11, "X", "x" },
    { 2,  0, "U", "u" },
    { 2,  4, "Z", "z" },
    { 2,  5, "I", "i" },
    { 2, 11, "J", "j" },
    { 3,  0, "DETERENPBBLE", "deterenpbble" },
    { 4,  6, "P", "p" },
    { 4, 11, "Z", "z" },
    { 5,  2, "H", "h" },
    { 5,  9, "B", "b" },
    { 6,  3, "B", "b" },
    { 6,  8, "U", "u" },
    { 7,  0, "S", "s" },
    { 7,  5, "C", "c" },
    { 7,  8, "R", "r" },
    { 8,  2, "L", "l" },
    { 8,  7, "Y", "y" }
};

static const uint8_t intervals[13][5] = {
  // Always on
  { 0, LAYER_NB_NO_KLOKKEN, LAYER_NB_NO_ER, 0, 0 },
  // XX:00 - XX:04
  { 0, 0, 0, 0, 0 },
  // XX:05 - XX:09
  { 0, LAYER_NB_NO_FEM, LAYER_NB_NO_OVER, 0, 0 },
  // XX:10 - XX:14
  { 0, LAYER_NB_NO_TI, LAYER_NB_NO_OVER, 0, 0 },
  // XX:15 - XX:19
  { 0, LAYER_NB_NO_KVART, LAYER_NB_NO_OVER, 0, 0 },
  // XX:20 - XX:24
  { 1, LAYER_NB_NO_TI, LAYER_NB_NO_PA, LAYER_NB_NO_HALV, 0 },
  // XX:25 - XX:29
  { 1, LAYER_NB_NO_FEM, LAYER_NB_NO_PA, LAYER_NB_NO_HALV, 0 },
  // XX:30 - XX:34
  { 1, LAYER_NB_NO_HALV, 0, 0, 0 },
  // XX:35 - XX:39
  { 1, LAYER_NB_NO_FEM, LAYER_NB_NO_OVER, LAYER_NB_NO_HALV, 0 },
  // XX:40 - XX:44
  { 1, LAYER_NB_NO_TI, LAYER_NB_NO_OVER, LAYER_NB_NO_HALV, 0 },
  // XX:45 - XX:49
  { 1, LAYER_NB_NO_KVART, LAYER_NB_NO_PA, 0, 0 },
  // XX:50 - XX:54
  { 1, LAYER_NB_NO_TI, LAYER_NB_NO_PA, 0, 0 },
  // XX:55 - XX:59
  { 1, LAYER_NB_NO_FEM, LAYER_NB_NO_PA, 0, 0 },
};

