enum {
  // es_ES
  LAYER_ES_ES_ES          = 13,
  LAYER_ES_ES_LA          = 14,
  LAYER_ES_ES_SON         = 15,
  LAYER_ES_ES_LAS         = 16,
  LAYER_ES_ES_CINCO       = 17,
  LAYER_ES_ES_DIEZ        = 18,
  LAYER_ES_ES_CUARTO      = 19,
  LAYER_ES_ES_VEINTE      = 20,
  LAYER_ES_ES_VEINTICINCO = 21,
  LAYER_ES_ES_MEDIA       = 22,
  LAYER_ES_ES_Y           = 23,
  LAYER_ES_ES_MENOS       = 24,

  LAYER_FILLER = 27
};

/**
 * Array of array of word_t structs required to create all the phrases for designating time.
 *
 */
static const word_t words[54] = {
  // Hours 1-12
  [1]  = { 0, 8, "UNA",    "una"    },
  [2]  = { 4, 0, "DOS",    "dos"    },
  [3]  = { 1, 0, "TRES",   "tres"   },
  [4]  = { 2, 6, "CUATRO", "cuatro" },
  [5]  = { 3, 5, "CINCO",  "cinco"  },
  [6]  = { 2, 1, "SEIS",   "seis"   },
  [7]  = { 4, 7, "SIETE",  "siete"  },
  [8]  = { 3, 0, "OCHO",   "ocho"   },
  [9]  = { 5, 0, "NUEVE",  "nueve"  },
  [10] = { 1, 8, "DIEZ",   "diez"   },
  [11] = { 1, 4, "ONCE",   "once"   },
  [12] = { 4, 3, "DOCE",   "doce"   },

  // Minutes before/after the hour
  [LAYER_ES_ES_CINCO]       = { 6,  7, "CINCO",       "cinco"       },
  [LAYER_ES_ES_DIEZ]        = { 7,  0, "DIEZ",        "diez"        },
  [LAYER_ES_ES_CUARTO]      = { 8,  0, "CUARTO",      "cuarto"      },
  [LAYER_ES_ES_VEINTE]      = { 7,  5, "VEINTE",      "veinte"      },
  [LAYER_ES_ES_VEINTICINCO] = { 6,  1, "VEINTICINCO", "veinticinco" },
  [LAYER_ES_ES_MEDIA]       = { 8,  7, "MEDIA",       "media"       },
  [LAYER_ES_ES_MENOS]       = { 5,  6, "MENOS",       "menos"       },
  [LAYER_ES_ES_Y]           = { 5, 11, "Y",           "y"           },

  // Relative
  [LAYER_ES_ES_ES]  = { 0, 0, "ES",  "e "  },
  [LAYER_ES_ES_SON] = { 0, 1, "SON", " on" },
  [LAYER_ES_ES_LA]  = { 0, 5, "LA",  "  "  },
  [LAYER_ES_ES_LAS] = { 0, 5, "LAS", "  s" },

  // Extra, "random" characters used to fill in empty spaces between words
  [LAYER_FILLER] =
    { 0,  4, "Z", "z" },
    { 0, 11, "H", "h" },
    { 2,  0, "X", "x" },
    { 2,  5, "P", "p" },
    { 3,  4, "Ñ", "ñ" },
    { 3, 10, "L", "l" },
    { 3, 11, "X", "x" },
    { 5,  5, "F", "f" },
    { 6,  0, "J", "j" },
    { 7,  4, "Ó", "ó" },
    { 7, 11, "G", "g" },
    { 8,  6, "K", "k" }
};

static const uint8_t intervals[13][5] = {
  // Always on
  { 0, 0, 0, 0, 0 },
  // XX:00 - XX:04
  { 0, 0, 0, 0, 0 },
  // XX:05 - XX:09
  { 0, LAYER_ES_ES_Y, LAYER_ES_ES_CINCO, 0, 0 },
  // XX:10 - XX:14
  { 0, LAYER_ES_ES_Y, LAYER_ES_ES_DIEZ, 0, 0 },
  // XX:15 - XX:19
  { 0, LAYER_ES_ES_Y, LAYER_ES_ES_CUARTO, 0, 0 },
  // XX:20 - XX:24
  { 0, LAYER_ES_ES_Y, LAYER_ES_ES_VEINTE, 0, 0 },
  // XX:25 - XX:29
  { 0, LAYER_ES_ES_Y, LAYER_ES_ES_VEINTICINCO, LAYER_ES_ES_CINCO, 0 },
  // XX:30 - XX:34
  { 0, LAYER_ES_ES_Y, LAYER_ES_ES_MEDIA, 0, 0 },
  // XX:35 - XX:39
  { 1, LAYER_ES_ES_MENOS, LAYER_ES_ES_VEINTICINCO, LAYER_ES_ES_CINCO, 0 },
  // XX:40 - XX:44
  { 1, LAYER_ES_ES_MENOS, LAYER_ES_ES_VEINTE, 0, 0 },
  // XX:45 - XX:49
  { 1, LAYER_ES_ES_MENOS, LAYER_ES_ES_CUARTO, 0, 0 },
  // XX:50 - XX:54
  { 1, LAYER_ES_ES_MENOS, LAYER_ES_ES_DIEZ, 0, 0 },
  // XX:55 - XX:59
  { 1, LAYER_ES_ES_MENOS, LAYER_ES_ES_CINCO, 0, 0 },
};

