enum {
  // pt_PT
  LAYER_PT_PT_EE          = 13,
  LAYER_PT_PT_SAO         = 14,
  LAYER_PT_PT_DUAS_SETE_S = 15,
  LAYER_PT_PT_OITO_O      = 16,
  LAYER_PT_PT_OITO_I      = 17,
  LAYER_PT_PT_OITO_T      = 18,
  LAYER_PT_PT_OITO_O2     = 19,
  LAYER_PT_PT_A_CINCO     = 20,
  LAYER_PT_PT_D_CINCO     = 21,
  LAYER_PT_PT_A_DEZ       = 22,
  LAYER_PT_PT_D_DEZ       = 23,
  LAYER_PT_PT_A_QUINZE    = 24,
  LAYER_PT_PT_D_QUINZE    = 25,
  LAYER_PT_PT_A_VINTE     = 26,
  LAYER_PT_PT_D_VINTE     = 27,
  LAYER_PT_PT_A_VCE_E     = 28,
  LAYER_PT_PT_D_VCE_E     = 29,
  LAYER_PT_PT_MEIA        = 30,
  LAYER_PT_PT_E           = 31,
  LAYER_PT_PT_PARA        = 32,
  LAYER_PT_PT_A           = 33,
  LAYER_PT_PT_AS          = 34,
  LAYER_FILLER            = 35
};

/**
 * Array of array of word_t structs required to create all the phrases for designating time.
 *
 *   0123456789ab
 * 0 ÉSÃO_VINTE_E
 * 1 QUINZE_CINCO
 * 2 DEZ_PARA_AS_
 * 3 QUATRONZEUMA
 * 4 DEZSEISCINCO
 * 5 DUASETE_TRÊS
 * 6 NOVEDOZE_E__
 * 7 MEIADEZVINTE
 * 8 QUINZE_CINCO
 *   0123456789ab
 */
static const word_t words[54] = {
  // Hours 1-12
  [1]  = { 3, 9, "UMA",    "uma"    },
  [2]  = { 5, 0, "DUA ",   "dua "   }, // and LAYER_PT_PT_DUAS_SETE_S
  [3]  = { 5, 8, "TRÊS",   "três"   },
  [4]  = { 3, 0, "QUATR ", "quatr " }, // and LAYER_PT_PT_OITO_O
  [5]  = { 4, 7, "CINCO",  "cinco"  },
  [6]  = { 4, 3, "SE S",   "se s"   }, // and LAYER_PT_PT_OITO_I
  [7]  = { 5, 3, " E E",   " e e"   }, // and LAYER_PT_PT_DUAS_SETE_S and LAYER_PT_PT_OITO_T
  [8]  = { 0, 0, "",       ""       }, // LAYER_PT_PT_OITO_O, LAYER_PT_PT_OITO_I, LAYER_PT_PT_OITO_T, LAYER_PT_PT_OITO_O2
  [9]  = { 6, 0, "NOVE",   "nove"   },
  [10] = { 4, 0, "DEZ",    "dez"    },
  [11] = { 3, 5, " NZE",   " nze"   }, // and LAYER_PT_PT_OITO_O
  [12] = { 6, 4, "D ZE",   "d ze"   }, // and LAYER_PT_PT_OITO_O2

  [LAYER_PT_PT_DUAS_SETE_S] = { 5, 3, "S", "s" },
  [LAYER_PT_PT_OITO_O]  = { 3, 5, "O", "o" },
  [LAYER_PT_PT_OITO_I]  = { 4, 5, "I", "i" },
  [LAYER_PT_PT_OITO_T]  = { 5, 5, "T", "t" },
  [LAYER_PT_PT_OITO_O2] = { 6, 5, "O", "o" },

  // Minutes before/after the hour
  [LAYER_PT_PT_A_CINCO]  = { 1,  7, "CINCO",  "cinco"  },
  [LAYER_PT_PT_D_CINCO]  = { 8,  7, "CINCO",  "cinco"  },
  [LAYER_PT_PT_A_DEZ]    = { 2,  0, "DEZ",    "dez"    },
  [LAYER_PT_PT_D_DEZ]    = { 7,  4, "DEZ",    "dez"    },
  [LAYER_PT_PT_A_QUINZE] = { 1,  0, "QUINZE", "quinze" },
  [LAYER_PT_PT_D_QUINZE] = { 8,  0, "QUINZ ", "quinz " }, // and LAYER_PT_PT_D_VCE_E
  [LAYER_PT_PT_A_VINTE]  = { 0,  5, "VINTE",  "vinte"  },
  [LAYER_PT_PT_D_VINTE]  = { 7,  7, "VINTE",  "vinte"  },
  [LAYER_PT_PT_A_VCE_E]  = { 0, 11, "E",      "e"      },
  [LAYER_PT_PT_D_VCE_E]  = { 8,  5, "E",      "e"      },
  [LAYER_PT_PT_MEIA]     = { 7,  0, "MEIA",   "meia"   },

  // Relative
  [LAYER_PT_PT_EE]   = { 0,  0, "É",    "é"    },
  [LAYER_PT_PT_SAO]  = { 0,  1, "SAO",  "sao"  },
  [LAYER_PT_PT_E]    = { 6,  9, "E",    "e"    },
  [LAYER_PT_PT_PARA] = { 2,  4, "PARA", "para" },
  [LAYER_PT_PT_A]    = { 2,  9, "A",    "a"    },
  [LAYER_PT_PT_AS]   = { 2, 10, "S",    "s"    }, // and LAYER_PT_PT_A

  // Extra, "random" characters used to fill in empty spaces between words
  [LAYER_FILLER] =
    { 0,  4, "Z", "z" },
    { 0, 10, "H", "h" },
    { 1,  6, "T", "t" },
    { 2,  3, "X", "x" },
    { 2,  8, "F", "f" },
    { 2, 11, "N", "n" },
    { 5,  7, "L", "l" },
    { 6,  8, "J", "j" },
    { 6, 10, "Ó", "ó" },
    { 6, 11, "G", "g" },
    { 8,  6, "R", "r" }
};

static const uint8_t intervals[13][5] = {
  // Always on
  { 0, 0, 0, 0, 0 },
  // XX:00 - XX:04
  { 0, 0, 0, 0, 0 },
  // XX:05 - XX:09
  { 0, LAYER_PT_PT_E, LAYER_PT_PT_D_CINCO, 0, 0 },
  // XX:10 - XX:14
  { 0, LAYER_PT_PT_E, LAYER_PT_PT_D_DEZ, 0, 0 },
  // XX:15 - XX:19
  { 0, LAYER_PT_PT_E, LAYER_PT_PT_D_QUINZE, LAYER_PT_PT_D_VCE_E, 0 },
  // XX:20 - XX:24
  { 0, LAYER_PT_PT_E, LAYER_PT_PT_D_VINTE, 0, 0 },
  // XX:25 - XX:29
  { 0, LAYER_PT_PT_E, LAYER_PT_PT_D_VINTE, LAYER_PT_PT_D_VCE_E, LAYER_PT_PT_D_CINCO },
  // XX:30 - XX:34
  { 0, LAYER_PT_PT_E, LAYER_PT_PT_MEIA, 0, 0 },
  // XX:35 - XX:39
  { 1, LAYER_PT_PT_A_VINTE, LAYER_PT_PT_A_VCE_E, LAYER_PT_PT_A_CINCO, LAYER_PT_PT_PARA },
  // XX:40 - XX:44
  { 1, LAYER_PT_PT_A_VINTE, LAYER_PT_PT_PARA, 0, 0 },
  // XX:45 - XX:49
  { 1, LAYER_PT_PT_A_QUINZE, LAYER_PT_PT_PARA, 0, 0 },
  // XX:50 - XX:54
  { 1, LAYER_PT_PT_A_DEZ, LAYER_PT_PT_PARA, 0, 0 },
  // XX:55 - XX:59
  { 1, LAYER_PT_PT_A_CINCO, LAYER_PT_PT_PARA, 0, 0 },
};

