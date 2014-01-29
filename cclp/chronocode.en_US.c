enum {
  // en_US
  LAYER_EN_US_IT      = 13,
  LAYER_EN_US_IS      = 14,
  LAYER_EN_US_A       = 15,
  LAYER_EN_US_FIVE    = 16,
  LAYER_EN_US_TEN     = 17,
  LAYER_EN_US_QUARTER = 18,
  LAYER_EN_US_HALF    = 19,
  LAYER_EN_US_TWENTY  = 20,
  LAYER_EN_US_PAST    = 21,
  LAYER_EN_US_TO      = 22,
  LAYER_EN_US_OCLOCK  = 23,

  LAYER_FILLER = 27
};

/**
 * Array of array of word_t structs required to create all the phrases for designating time.
 *
 */
static const word_t words[54] = {
  // Hours 1-12
  [1]  = { 5, 0, "ONE",    "one"    },
  [2]  = { 4, 9, "TWO",    "two"    },
  [3]  = { 8, 0, "THREE",  "three"  },
  [4]  = { 6, 8, "FOUR",   "four"   },
  [5]  = { 4, 5, "FIVE",   "five"   },
  [6]  = { 5, 3, "SIX",    "six"    },
  [7]  = { 4, 0, "SEVEN",  "seven"  },
  [8]  = { 7, 0, "EIGHT",  "eight"  },
  [9]  = { 6, 4, "NINE",   "nine"   },
  [10] = { 6, 0, "TEN",    "ten"    },
  [11] = { 5, 6, "ELEVEN", "eleven" },
  [12] = { 7, 5, "TWELVE", "twelve" },

  // Minutes before/after the hour
  [LAYER_EN_US_FIVE]    = {  2, 8, "FIVE",    "five"    },
  [LAYER_EN_US_TEN]     = {  1, 9, "TEN",     "ten"     },
  [LAYER_EN_US_A]       = {  1, 0, "A",       "a"       },
  [LAYER_EN_US_QUARTER] = {  1, 2, "QUARTER", "quarter" },
  [LAYER_EN_US_HALF]    = {  3, 0, "HALF",    "half"    },
  [LAYER_EN_US_TWENTY]  = {  2, 1, "TWENTY",  "twenty"  },

  // Relative
  [LAYER_EN_US_IT]     = {  0, 0, "IT",     "it"     },
  [LAYER_EN_US_IS]     = {  0, 3, "IS",     "is"     },
  [LAYER_EN_US_PAST]   = {  3, 5, "PAST",   "past"   },
  [LAYER_EN_US_TO]     = {  3, 9, "TO",     "to",    },
  [LAYER_EN_US_OCLOCK] = {  8, 6, "OCLOCK", "oclock" },

  // Extra, "random" characters used to fill in empty spaces between words
  [LAYER_FILLER] =
    { 0,  2, "Z", "z" },
    { 0,  5, "APEBBLE", "apebble" },
    { 1,  1, "T", "t" },
    { 2,  0, "K", "k" },
    { 2,  7, "N", "n" },
    { 3,  4, "B", "o" },
    { 3, 11, "F", "f" },
    { 6,  3, "E", "e" },
    { 7, 11, "S", "s" },
    { 8,  5, "H", "h" }
};

static const uint8_t intervals[13][5] = {
  // Always on
  { 0, LAYER_EN_US_IT, LAYER_EN_US_IS, 0, 0 },
  // XX:00 - XX:04
  { 0, LAYER_EN_US_OCLOCK, 0, 0, 0 },
  // XX:05 - XX:09
  { 0, LAYER_EN_US_FIVE, LAYER_EN_US_PAST, 0, 0 },
  // XX:10 - XX:14
  { 0, LAYER_EN_US_TEN, LAYER_EN_US_PAST, 0, 0 },
  // XX:15 - XX:19
  { 0, LAYER_EN_US_A, LAYER_EN_US_QUARTER, LAYER_EN_US_PAST, 0 },
  // XX:20 - XX:24
  { 0, LAYER_EN_US_TWENTY, LAYER_EN_US_PAST, 0, 0 },
  // XX:25 - XX:29
  { 0, LAYER_EN_US_TWENTY, LAYER_EN_US_FIVE, LAYER_EN_US_PAST, 0 },
  // XX:30 - XX:34
  { 0, LAYER_EN_US_HALF, LAYER_EN_US_PAST, 0, 0 },
  // XX:35 - XX:39
  { 1, LAYER_EN_US_TWENTY, LAYER_EN_US_FIVE, LAYER_EN_US_TO, 0 },
  // XX:40 - XX:44
  { 1, LAYER_EN_US_TWENTY, LAYER_EN_US_TO, 0, 0 },
  // XX:45 - XX:49
  { 1, LAYER_EN_US_A, LAYER_EN_US_QUARTER, LAYER_EN_US_TO, 0 },
  // XX:50 - XX:54
  { 1, LAYER_EN_US_TEN, LAYER_EN_US_TO, 0, 0 },
  // XX:55 - XX:59
  { 1, LAYER_EN_US_FIVE, LAYER_EN_US_TO, 0,  0 },
};

