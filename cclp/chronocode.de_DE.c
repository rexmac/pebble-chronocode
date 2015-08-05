enum {
  // de_DE
  LAYER_DE_DE_ES      = 14,
  LAYER_DE_DE_IST     = 15,
  LAYER_DE_DE_FUENF   = 16,
  LAYER_DE_DE_ZEHN    = 17,
  LAYER_DE_DE_VIERTEL = 18,
  LAYER_DE_DE_ZWANZIG = 19,
  LAYER_DE_DE_HALB    = 20,
  LAYER_DE_DE_NACH    = 21,
  LAYER_DE_DE_VOR     = 22,
  LAYER_DE_DE_UHR     = 23,

  LAYER_FILLER = 27
};

/**
 * Array of array of word_t structs required to create all the phrases for designating time.
 *
 */
static const word_t words[54] = {
  // Hours 1-12
  [1]  = { 3, 9, "EIN",    "ein"    },
  [2]  = { 6, 0, "ZWEI",   "zwei"   },
  [3]  = { 4, 8, "DREI",   "drei"   },
  [4]  = { 7, 0, "VIER",   "vier"   },
  [5]  = { 5, 8, "FÜNF",   "fünf"   },
  [6]  = { 8, 0, "SECHS",  "sechs"  },
  [7]  = { 5, 0, "SIEBEN", "sieben" },
  [8]  = { 7, 4, "ACHT",   "acht"   },
  [9]  = { 6, 8, "NEUN",   "neun"   },
  [10] = { 7, 8, "ZEHN",   "zehn"   },
  [11] = { 6, 5, "ELF",    "elf"    },
  [12] = { 4, 0, "ZWÖLF",  "zwölf"  },
  [13] = { 8, 5, "EINS",   "eins"   },

  // Minutes before/after the hour
  [LAYER_DE_DE_FUENF]   = { 0, 7, "FÜNF",    "fünf"    },
  [LAYER_DE_DE_ZEHN]    = { 1, 0, "ZEHN",    "zehn"    },
  [LAYER_DE_DE_VIERTEL] = { 2, 0, "VIERTEL", "viertel" },
  [LAYER_DE_DE_HALB]    = { 3, 4, "HALB",    "halb"    },
  [LAYER_DE_DE_ZWANZIG] = { 1, 5, "ZWANZIG", "zwanzig" },

  // Relative
  [LAYER_DE_DE_ES]   = { 0, 0, "ES",   "es"   },
  [LAYER_DE_DE_IST]  = { 0, 3, "IST",  "ist"  },
  [LAYER_DE_DE_NACH] = { 2, 8, "NACH", "nach" },
  [LAYER_DE_DE_VOR]  = { 3, 0, "VOR",  "vor"  },
  [LAYER_DE_DE_UHR]  = { 8, 9, "UHR",  "uhr"  },

  // Extra, "random" characters used to fill in empty spaces between words
  [LAYER_FILLER] =
    { 0,  2, "Z", "z" },
    { 0,  6, "A", "a" },
    { 0, 11, "T", "t" },
    { 1,  4, "L", "l" },
    { 2,  7, "K", "k" },
    { 3,  3, "S", "s" },
    { 3,  8, "O", "o" },
    { 4,  5, "G", "g" },
    { 4,  6, "K", "k" },
    { 4,  7, "P", "p" },
    { 5,  6, "J", "j" },
    { 5,  7, "T", "t" },
    { 6,  4, "U", "u" }
};

static const uint8_t intervals[13][5] = {
  // Always on
  { 0, LAYER_DE_DE_ES, LAYER_DE_DE_IST, 0, 0 },
  // XX:00 - XX:04
  { 0, LAYER_DE_DE_UHR, 0, 0, 0 },
  // XX:05 - XX:09
  { 0, LAYER_DE_DE_FUENF, LAYER_DE_DE_NACH, 0, 0 },
  // XX:10 - XX:14
  { 0, LAYER_DE_DE_ZEHN, LAYER_DE_DE_NACH, 0, 0 },
  // XX:15 - XX:19
  { 0, LAYER_DE_DE_VIERTEL, LAYER_DE_DE_NACH, 0, 0 },
  // XX:20 - XX:24
  { 0, LAYER_DE_DE_ZWANZIG, LAYER_DE_DE_NACH, 0, 0 },
  // XX:25 - XX:29
  { 1, LAYER_DE_DE_FUENF, LAYER_DE_DE_VOR, LAYER_DE_DE_HALB, 0 },
  // XX:30 - XX:34
  { 1, LAYER_DE_DE_HALB, 0, 0, 0 },
  // XX:35 - XX:39
  { 1, LAYER_DE_DE_FUENF, LAYER_DE_DE_NACH, LAYER_DE_DE_HALB, 0 },
  // XX:40 - XX:44
  { 1, LAYER_DE_DE_ZWANZIG, LAYER_DE_DE_VOR, 0, 0 },
  // XX:45 - XX:49
  { 1, LAYER_DE_DE_VIERTEL, LAYER_DE_DE_VOR, 0, 0 },
  // XX:50 - XX:54
  { 1, LAYER_DE_DE_ZEHN, LAYER_DE_DE_VOR, 0, 0 },
  // XX:55 - XX:59
  { 1, LAYER_DE_DE_FUENF, LAYER_DE_DE_VOR, 0, 0 },
};

