enum {
  // da_DK
  LAYER_DA_DK_KLOKKEN  = 13,
  LAYER_DA_DK_ER       = 14,
  LAYER_DA_DK_FEM      = 15,
  LAYER_DA_DK_TI       = 16,
  LAYER_DA_DK_KVART    = 17,
  LAYER_DA_DK_TYVE     = 18,
  LAYER_DA_DK_HALV     = 19,
  LAYER_DA_DK_OVER     = 20,
  LAYER_DA_DK_I        = 21,
  LAYER_DA_DK_MINUTTER = 22,

  LAYER_FILLER = 27
};

/**
 * Array of array of word_t structs required to create all the phrases for designating time.
 *
 */
static const word_t words[54] = {
  // Hours 1-12
  [1]  = { 5,  0, "ET", "et" },
  [2]  = { 8,  0, "TO", "to" },
  [3]  = { 6,  0, "TRE", "tre" },
  [4]  = { 7,  1, "FIRE", "fire" },
  [5]  = { 6,  9, "FEM", "fem" },
  [6]  = { 8,  3, "SEKS", "seks" },
  [7]  = { 7,  9, "SYV", "syv" },
  [8]  = { 8,  8, "OTTE", "otte" },
  [9]  = { 7,  6, "NI", "ni" },
  [10] = { 5, 10, "TI", "ti" },
  [11] = { 5,  3, "ELLEVE", "elleve" },
  [12] = { 6,  3, "TOLV", "tolv" },

  // Minutes before/after the hour
  [LAYER_DA_DK_FEM]   = { 2, 0, "FEM", "fem" },
  [LAYER_DA_DK_TI]    = { 1, 5, "TI", "ti" },
  [LAYER_DA_DK_KVART] = { 1, 7, "KVART", "kvart" },
  [LAYER_DA_DK_TYVE]  = { 1, 0, "TYVE", "tyve" },
  [LAYER_DA_DK_HALV]  = { 4, 7, "HALV", "halv" },

  // Relative
  [LAYER_DA_DK_KLOKKEN]  = { 0, 0, "KLOKKEN", "klokken" },
  [LAYER_DA_DK_ER]       = { 0, 8, "ER", "er" },
  [LAYER_DA_DK_I]        = { 4, 5, "I", "i" },
  [LAYER_DA_DK_OVER]     = { 4, 0, "OVER", "over" },
  [LAYER_DA_DK_MINUTTER] = { 2, 4, "MINUTTER", "minutter" },

  // Extra, "random" characters used to fill in empty spaces between words
  [LAYER_FILLER] =
    { 0,  7, "P", "p" },
    { 0, 10, "YT", "yt" },
    { 1,  4, "C", "c" },
    { 2,  3, "Z", "z" },
    { 3,  0, "DETERENPBBLE", "deterenpbble" },
    { 4,  4, "G", "g" },
    { 4,  6, "P", "p" },
    { 4, 11, "Z", "z" },
    { 5,  2, "J", "j" },
    { 5,  9, "Ø", "ø" },
    { 6,  7, "B", "b" },
    { 6,  8, "U", "u" },
    { 7,  0, "S", "s" },
    { 7,  5, "C", "c" },
    { 7,  8, "Å", "å" },
    { 8,  2, "L", "l" },
    { 8,  7, "Y", "y" }
};

static const uint8_t intervals[13][5] = {
  // Always on
  { 0, LAYER_DA_DK_KLOKKEN, LAYER_DA_DK_ER, 0, 0 },
  // XX:00 - XX:04
  { 0, 0, 0, 0, 0 },
  // XX:05 - XX:09
  { 0, LAYER_DA_DK_FEM, LAYER_DA_DK_MINUTTER, LAYER_DA_DK_OVER, 0 },
  // XX:10 - XX:14
  { 0, LAYER_DA_DK_TI, LAYER_DA_DK_MINUTTER, LAYER_DA_DK_OVER, 0 },
  // XX:15 - XX:19
  { 0, LAYER_DA_DK_KVART, LAYER_DA_DK_OVER, 0, 0 },
  // XX:20 - XX:24
  { 0, LAYER_DA_DK_TYVE, LAYER_DA_DK_MINUTTER, LAYER_DA_DK_OVER, 0 },
  // XX:25 - XX:29
  { 1, LAYER_DA_DK_FEM, LAYER_DA_DK_MINUTTER, LAYER_DA_DK_I, LAYER_DA_DK_HALV },
  // XX:30 - XX:34
  { 1, LAYER_DA_DK_HALV, 0, 0, 0 },
  // XX:35 - XX:39
  { 1, LAYER_DA_DK_FEM, LAYER_DA_DK_MINUTTER, LAYER_DA_DK_OVER, LAYER_DA_DK_HALV },
  // XX:40 - XX:44
  { 1, LAYER_DA_DK_TYVE, LAYER_DA_DK_MINUTTER, LAYER_DA_DK_I, 0 },
  // XX:45 - XX:49
  { 1, LAYER_DA_DK_KVART, LAYER_DA_DK_I, 0, 0 },
  // XX:50 - XX:54
  { 1, LAYER_DA_DK_TI, LAYER_DA_DK_MINUTTER, LAYER_DA_DK_I, 0 },
  // XX:55 - XX:59
  { 1, LAYER_DA_DK_FEM, LAYER_DA_DK_MINUTTER, LAYER_DA_DK_I, 0 },
};

