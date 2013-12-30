/**
 * Header file for a WordSquare watchface in French/Français (fr_FR).
 *
 */
#define LAYER_IL      0
#define LAYER_EST    14
#define LAYER_CINQ   15
#define LAYER_DIX    16
#define LAYER_QUART  17
#define LAYER_VINGT  18
#define LAYER_TRAIT  19
#define LAYER_DEMIE  20
#define LAYER_ET     21
#define LAYER_LE     22
#define LAYER_MOINS  23
#define LAYER_HEURE  24
#define LAYER_HEURES 25

/**
 * Array of words required to create all the phrases for designating time.
 *
 *      fr_FR
 *   0123456789ab
 * 0 ILxESTxTROIS
 * 1 NEUFSEPTONZE
 * 2 QUATRExDOUZE
 * 3 xHUITxDEUXxx
 * 4 xDIXxUNExSIX
 * 5 CINQxxHEURES
 * 6 xxMOINSxDIXx
 * 7 ETVINGT-CINQ
 * 8 LEDEMIEQUART
 *   0123456789ab
 */
static const word_t words[] = {
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

  // Minutes
  [LAYER_CINQ]  = { 7, 8, "CINQ",  "cinq"  },
  [LAYER_DIX]   = { 6, 8, "DIX",   "dix"   },
  [LAYER_QUART] = { 8, 7, "QUART", "quart" },
  [LAYER_VINGT] = { 7, 2, "VINGT", "vingt" },
  [LAYER_TRAIT] = { 7, 7, "-",     "-"     },
  [LAYER_DEMIE] = { 8, 2, "DEMIE", "demie" },

  // Relative
  [LAYER_IL]     = { 0, 0, "IL",     "il"     },
  [LAYER_EST]    = { 0, 3, "EST",    "est"    },
  [LAYER_ET]     = { 7, 0, "ET",     "et"     },
  [LAYER_LE]     = { 8, 0, "LE",     "le"     },
  [LAYER_MOINS]  = { 6, 2, "MOINS",  "moins"  },
  [LAYER_HEURE]  = { 5, 6, "HEURE",  ""       },
  [LAYER_HEURES] = { 5, 6, "HEURES", "     s" },

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
    { 6, 11, "L", "l" },
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
  for (unsigned i = 0; i < WORD_COUNT; i++) {
    toggle_word(i, 0);
  }

  // Turn on the words that are always on
  toggle_word(LAYER_IL, 1);
  toggle_word(LAYER_EST, 1);

  // Turn on the words needed for the current time
  if (min < 5) {
    // Il est X heures.
  } else if (min < 10) { // Il est X heures cinq.
    toggle_word(LAYER_CINQ, 1);
  } else if (min < 15) { // Il est X heures dix.
    toggle_word(LAYER_DIX, 1);
  } else if (min < 20) { // Il est X heures et quart.
    toggle_word(LAYER_ET, 1);
    toggle_word(LAYER_QUART, 1);
  } else if (min < 25) { // Il est X heures vingt.
    toggle_word(LAYER_VINGT, 1);
  } else if (min < 30) { // Il est X heures vingt-cinq.
    toggle_word(LAYER_VINGT, 1);
    toggle_word(LAYER_TRAIT, 1);
    toggle_word(LAYER_CINQ, 1);
  } else if (min < 35) { // Il est X heures et demie.
    toggle_word(LAYER_ET, 1);
    toggle_word(LAYER_DEMIE, 1);
  } else if (min < 40) { // Il est X+1 heures moins vingt-cinq.
    toggle_word(LAYER_MOINS, 1);
    toggle_word(LAYER_VINGT, 1);
    toggle_word(LAYER_TRAIT, 1);
    toggle_word(LAYER_CINQ, 1);
    hour++;
  } else if (min < 45) { // Il est X+1 heures moins vingt.
    toggle_word(LAYER_MOINS, 1);
    toggle_word(LAYER_VINGT, 1);
    hour++;
  } else if (min < 50) { // Il est X+1 heures moins le quart.
    toggle_word(LAYER_MOINS, 1);
    toggle_word(LAYER_LE, 1);
    toggle_word(LAYER_QUART, 1);
    hour++;
  } else if (min < 55) { // Il est X+1 heures moins dix.
    toggle_word(LAYER_MOINS, 1);
    toggle_word(LAYER_DIX, 1);
    hour++;
  } else { // Il est X+1 moins cinq.
    toggle_word(LAYER_MOINS, 1);
    toggle_word(LAYER_CINQ, 1);
    hour++;
  }

  // Update the minute box
  minute_num = min % 5;
  layer_mark_dirty(minute_layer);

  // Convert from 24-hour to 12-hour time
  if (hour == 0) hour = 12;
  else if (hour > 12) hour -= 12;

  if (hour == 1) {
    toggle_word(LAYER_HEURE, 1);
  } else {
    toggle_word(LAYER_HEURES, 1);
  }

  // Turn on the word needed for the curret hour, turn off the others
  for (int i = 1; i <= 12; i++) {
    toggle_word(i, i == hour ? 1 : 0);
  }
}

