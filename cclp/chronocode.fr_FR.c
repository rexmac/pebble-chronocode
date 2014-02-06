enum {
  // fr_FR
  LAYER_FR_FR_IL     = 13,
  LAYER_FR_FR_EST    = 14,
  LAYER_FR_FR_CINQ   = 15,
  LAYER_FR_FR_DIX    = 16,
  LAYER_FR_FR_QUART  = 17,
  LAYER_FR_FR_VINGT  = 18,
  LAYER_FR_FR_TRAIT  = 19,
  LAYER_FR_FR_DEMIE  = 20,
  LAYER_FR_FR_ET     = 21,
  LAYER_FR_FR_LE     = 22,
  LAYER_FR_FR_MOINS  = 23,
  LAYER_FR_FR_HEURE  = 24,
  LAYER_FR_FR_HEURES = 25,

  LAYER_FILLER = 27
};

/**
 * Array of array of word_t structs required to create all the phrases for designating time.
 *
 */
static const word_t words[54] = {
  // Hours 1-12
  [1]  = { 4, 5, "UNE",    "une"    },
  [2]  = { 3, 6, "DEUX",   "deux"   },
  [3]  = { 0, 7, "TROIS",  "trois"  },
  [4]  = { 2, 0, "QUATRE", "quatre" },
  [5]  = { 5, 0, "CINQ",   "cinq"   },
  [6]  = { 4, 9, "SIX",    "six"    },
  [7]  = { 1, 4, "SEPT",   "sept"   },
  [8]  = { 3, 1, "HUIT",   "huit"   },
  [9]  = { 1, 0, "NEUF",   "neuf"   },
  [10] = { 4, 1, "DIX",    "dix"    },
  [11] = { 1, 8, "ONZE",   "onze"   },
  [12] = { 2, 7, "DOUZE",  "douze"  },

  // Minutes before/after the hour
  [LAYER_FR_FR_CINQ]  = { 7, 8, "CINQ",  "cinq"  },
  [LAYER_FR_FR_DIX]   = { 6, 8, "DIX",   "dix"   },
  [LAYER_FR_FR_QUART] = { 8, 7, "QUART", "quart" },
  [LAYER_FR_FR_VINGT] = { 7, 2, "VINGT", "vingt" },
  [LAYER_FR_FR_TRAIT] = { 7, 7, "-",     "-"     },
  [LAYER_FR_FR_DEMIE] = { 8, 2, "DEMIE", "demie" },

  // Relative
  [LAYER_FR_FR_IL]     = { 0, 0, "IL",     "il"     },
  [LAYER_FR_FR_EST]    = { 0, 3, "EST",    "est"    },
  [LAYER_FR_FR_ET]     = { 7, 0, "ET",     "et"     },
  [LAYER_FR_FR_LE]     = { 8, 0, "LE",     "le"     },
  [LAYER_FR_FR_MOINS]  = { 6, 2, "MOINS",  "moins"  },
  [LAYER_FR_FR_HEURE]  = { 5, 6, "HEURE",  ""       },
  [LAYER_FR_FR_HEURES] = { 5, 6, "HEURES", "     s" },

  // Extra, "random" characters used to fill in empty spaces between words
  [LAYER_FILLER] =
    { 0,  2, "X", "x" },
    { 0,  6, "Ç", "ç" },
    { 2,  6, "F", "f" },
    { 3,  0, "K", "k" },
    { 3,  5, "O", "o" },
    { 3, 10, "A", "a" },
    { 3, 11, "P", "p" },
    { 4,  0, "M", "m" },
    { 4,  4, "V", "v" },
    { 4,  8, "T", "t" },
    { 5,  4, "O", "o" },
    { 5,  5, "F", "f" },
    { 6,  0, "G", "g" },
    { 6,  1, "K", "k" },
    { 6,  7, "W", "w" },
    { 6, 11, "L", "l" }
};

static const uint8_t intervals[13][5] = {
  // Always on
  { 0, LAYER_FR_FR_IL, LAYER_FR_FR_EST, 0, 0 },
  // XX:00 - XX:04
  { 0, 0, 0, 0, 0 },
  // XX:05 - XX:09
  { 0, LAYER_FR_FR_CINQ, 0, 0, 0 },
  // XX:10 - XX:14
  { 0, LAYER_FR_FR_DIX, 0, 0, 0 },
  // XX:15 - XX:19
  { 0, LAYER_FR_FR_ET, LAYER_FR_FR_QUART, 0, 0 },
  // XX:20 - XX:24
  { 0, LAYER_FR_FR_VINGT, 0, 0, 0 },
  // XX:25 - XX:29
  { 0, LAYER_FR_FR_VINGT, LAYER_FR_FR_TRAIT, LAYER_FR_FR_CINQ, 0 },
  // XX:30 - XX:34
  { 0, LAYER_FR_FR_ET, LAYER_FR_FR_DEMIE, 0, 0 },
  // XX:35 - XX:39
  { 1, LAYER_FR_FR_MOINS, LAYER_FR_FR_VINGT, LAYER_FR_FR_TRAIT, LAYER_FR_FR_CINQ },
  // XX:40 - XX:44
  { 1, LAYER_FR_FR_MOINS, LAYER_FR_FR_VINGT, 0, 0 },
  // XX:45 - XX:49
  { 1, LAYER_FR_FR_MOINS, LAYER_FR_FR_LE, LAYER_FR_FR_QUART, 0 },
  // XX:50 - XX:54
  { 1, LAYER_FR_FR_MOINS, LAYER_FR_FR_DIX, 0, 0 },
  // XX:55 - XX:59
  { 1, LAYER_FR_FR_MOINS, LAYER_FR_FR_CINQ, 0, 0 },
};

