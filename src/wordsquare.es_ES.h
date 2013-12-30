/**
 * Header file for a WordSquare watchface in Spanish/Español (es_ES).
 *
 */
#define LAYER_ES           0
#define LAYER_LA          14
#define LAYER_SON         15
#define LAYER_LAS         16
#define LAYER_CINCO       17
#define LAYER_DIEZ        18
#define LAYER_CUARTO      19
#define LAYER_VEINTE      20
#define LAYER_VEINTICINCO 21
#define LAYER_MEDIA       22
#define LAYER_Y           23
#define LAYER_MENOS       24

/**
 * Array of words required to create all the phrases for designating time.
 *
 *      es_ES
 *   0123456789ab
 * 0 ESONxLASUNAx
 * 1 TRESONCEDIEZ
 * 2 xSEISxCUATRO
 * 3 OCHOxCINCOxx
 * 4 DOSDOCESIETE
 * 5 NUEVExMENOSY
 * 6 xVEINTICINCO
 * 7 DIEZxVEINTEx
 * 8 CUARTOxMEDIA
 *   0123456789ab
 */
static const word_t words[] = {
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

  // Minutes
  [LAYER_CINCO]       = { 6,  7, "CINCO",       "cinco"       },
  [LAYER_DIEZ]        = { 7,  0, "DIEZ",        "diez"        },
  [LAYER_CUARTO]      = { 8,  0, "CUARTO",      "cuarto"      },
  [LAYER_VEINTE]      = { 7,  5, "VEINTE",      "veinte"      },
  [LAYER_VEINTICINCO] = { 6,  1, "VEINTICINCO", "veinticinco" },
  [LAYER_MEDIA]       = { 8,  7, "MEDIA",       "media"       },
  [LAYER_MENOS]       = { 5,  6, "MENOS",       "menos"       },
  [LAYER_Y]           = { 5, 11, "Y",           "y"           },

  // Relative
  [LAYER_ES]  = { 0, 0, "ES",  "e "  },
  [LAYER_SON] = { 0, 1, "SON", " on" },
  [LAYER_LA]  = { 0, 5, "LA",  "  "  },
  [LAYER_LAS] = { 0, 5, "LAS", "  s" },

  // Es la una.  "It is 1:00"
  // Son las dos.  "It is 2:00"
  // Son las once. "It is 11:00"
  // Es la una y media. "It is 1:30"
  // Son las cuatro y media. "It is 4:30"
  // Es la una y cuarto. "It is 1:15"

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
    { 8,  6, "K", "k" },
};

#define WORD_COUNT ((sizeof(words) / sizeof(*words)))

/**
 * Update time display.
 *
 * @param time The time to be disaplyed
 */
static void update_time(struct tm *time) {
  int hour = time->tm_hour;
  int min = time->tm_min;

  // Turn off all of the words
  toggle_word(LAYER_ES, 0);
  toggle_word(LAYER_SON, 0);
  toggle_word(LAYER_LA, 0);
  toggle_word(LAYER_LAS, 0);
  toggle_word(LAYER_CINCO, 0);
  toggle_word(LAYER_DIEZ, 0);
  toggle_word(LAYER_CUARTO, 0);
  toggle_word(LAYER_VEINTE, 0);
  toggle_word(LAYER_VEINTICINCO, 0);
  toggle_word(LAYER_MEDIA, 0);
  toggle_word(LAYER_MENOS, 0);
  toggle_word(LAYER_Y, 0);

  // Turn on the words needed for the current time
  if (min < 5) {
    // Es la una. Son las X.
  } else if (min < 10) { // Es la una y cinco. Son las X y cinco.
    toggle_word(LAYER_Y, 1);
    toggle_word(LAYER_CINCO, 1);
  } else if (min < 15) { // Es la una y diez. Son las X y diez.
    toggle_word(LAYER_Y, 1);
    toggle_word(LAYER_DIEZ, 1);
  } else if (min < 20) { // Es la una y cuarto. Son las X y cuarto.
    toggle_word(LAYER_Y, 1);
    toggle_word(LAYER_CUARTO, 1);
  } else if (min < 25) { // Es la una y veinte. Son las X y veinte.
    toggle_word(LAYER_Y, 1);
    toggle_word(LAYER_VEINTE, 1);
  } else if (min < 30) { // Es la una y veinticinco. Son las X y veinticinco
    toggle_word(LAYER_Y, 1);
    toggle_word(LAYER_VEINTICINCO, 1);
  } else if (min < 35) { // Es la una y media. Son las X y media.
    toggle_word(LAYER_Y, 1);
    toggle_word(LAYER_MEDIA, 1);
  } else if (min < 40) { // Es la una menos veinticinco. Son las X+1 menos veinticinco.
    toggle_word(LAYER_MENOS, 1);
    toggle_word(LAYER_VEINTICINCO, 1);
    hour++;
  } else if (min < 45) { // Es la una menos veinte. Son las X+1 menos veinte.
    toggle_word(LAYER_MENOS, 1);
    toggle_word(LAYER_VEINTE, 1);
    hour++;
  } else if (min < 50) { // Es la una menos menos cuarto. Son las X+1 menos cuarto.
    toggle_word(LAYER_MENOS, 1);
    toggle_word(LAYER_CUARTO, 1);
    hour++;
  } else if (min < 55) { // Es la una menos diez. Son las X+1 menos diez.
    toggle_word(LAYER_MENOS, 1);
    toggle_word(LAYER_DIEZ, 1);
    hour++;
  } else { // Es la una menos cinco. Son las X+1 menos cinco.
    toggle_word(LAYER_MENOS, 1);
    toggle_word(LAYER_CINCO, 1);
    hour++;
  }

  // Update the minute box
  minute_num = min % 5;
  layer_mark_dirty(minute_layer);

  // Convert from 24-hour to 12-hour time
  if (hour == 0) hour = 12;
  else if (hour > 12) hour -= 12;

  if (hour == 1) {
    toggle_word(LAYER_ES, 1);
    toggle_word(LAYER_LA, 1);
  } else {
    toggle_word(LAYER_SON, 1);
    toggle_word(LAYER_LAS, 1);
  }

  // Turn on the word needed for the curret hour, turn off the others
  for (int i = 1; i <= 12; i++) {
    toggle_word(i, i == hour ? 1 : 0);
  }
}

