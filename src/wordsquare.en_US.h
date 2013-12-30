/**
 * Header file for a WordSquare watchface in English (en_US).
 *
 */
#define LAYER_IT       0
#define LAYER_IS      14
#define LAYER_FIVE    15
#define LAYER_TEN     16
#define LAYER_QUARTER 17
#define LAYER_HALF    18
#define LAYER_TWENTY  19
#define LAYER_PAST    20
#define LAYER_TO      21
#define LAYER_OCLOCK  22
#define LAYER_A       23

/**
 * Array of words required to create all the phrases for designating time.
 *
 * Based on the work of Trammell Hudson (https://bitbucket.org/hudson/pebble/src/words),
 * which is based on qlock2.
 *
 *      en_US
 *   0123456789ab
 * 0 ITxISxxxxxxx
 * 1 AxQUARTERTEN
 * 2 xTWENTYxFIVE
 * 3 HALFxPASTTOx
 * 4 SEVENFIVETWO
 * 5 ONESIXELEVEN
 * 6 TENxNINEFOUR
 * 7 EIGHTTWELVEx
 * 8 THREExOCLOCK
 *   0123456789ab
 */
static const word_t words[] = {
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

  // Minutes
  [LAYER_FIVE]    = { 2, 8, "FIVE",    "five"    },
  [LAYER_TEN]     = { 1, 9, "TEN",     "ten"     },
  [LAYER_A]       = { 1, 0, "A",       "a"       },
  [LAYER_QUARTER] = { 1, 2, "QUARTER", "quarter" },
  [LAYER_HALF]    = { 3, 0, "HALF",    "half"    },
  [LAYER_TWENTY]  = { 2, 1, "TWENTY",  "twenty"  },

  // Relative
  [LAYER_IT]     = { 0, 0, "IT",     "it"     },
  [LAYER_IS]     = { 0, 3, "IS",     "is"     },
  [LAYER_PAST]   = { 3, 5, "PAST",   "past"   },
  [LAYER_TO]     = { 3, 9, "TO",     "to"     },
  [LAYER_OCLOCK] = { 8, 6, "OCLOCK", "oclock" },

  // Extra, "random" characters used to fill in empty spaces between words
  [LAYER_FILLER] =
    { 0,  2, "o", "z" },
    { 0,  5, "apebble", "apebble" },
    { 1,  1, "T", "t" },
    { 2,  0, "K", "k" },
    { 2,  7, "N", "n" },
    { 3,  4, "B", "o" },
    { 3, 11, "F", "f" },
    { 6,  3, "E", "e" },
    { 7, 11, "S", "s" },
    { 8,  5, "H", "h" },
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
  toggle_word(LAYER_OCLOCK, 0);
  toggle_word(LAYER_FIVE, 0);
  toggle_word(LAYER_TEN, 0);
  toggle_word(LAYER_A, 0);
  toggle_word(LAYER_QUARTER, 0);
  toggle_word(LAYER_TWENTY, 0);
  toggle_word(LAYER_HALF, 0);
  toggle_word(LAYER_PAST, 0);
  toggle_word(LAYER_TO, 0);

  // Turn on the words that are always on
  toggle_word(LAYER_IT, 1);
  toggle_word(LAYER_IS, 1);

  // Turn on the words needed for the current time
  if (min < 5) { // It is X o'clock.
    toggle_word(LAYER_OCLOCK, 1);
  } else if (min < 10) { // It is five past X.
    toggle_word(LAYER_FIVE, 1);
    toggle_word(LAYER_PAST, 1);
  } else if (min < 15) { // It is ten past X.
    toggle_word(LAYER_TEN, 1);
    toggle_word(LAYER_PAST, 1);
  } else if (min < 20) { // It is a quarter past X.
    toggle_word(LAYER_A, 1);
    toggle_word(LAYER_QUARTER, 1);
    toggle_word(LAYER_PAST, 1);
  } else if (min < 25) { // It is twenty past X.
    toggle_word(LAYER_TWENTY, 1);
    toggle_word(LAYER_PAST, 1);
  } else if (min < 30) { // It is twenty-five past X.
    toggle_word(LAYER_TWENTY, 1);
    toggle_word(LAYER_FIVE, 1);
    toggle_word(LAYER_PAST, 1);
  } else if (min < 35) { // It is half past X.
    toggle_word(LAYER_HALF, 1);
    toggle_word(LAYER_PAST, 1);
  } else if (min < 40) { // It is twenty-five to X+1.
    toggle_word(LAYER_TWENTY, 1);
    toggle_word(LAYER_FIVE, 1);
    toggle_word(LAYER_TO, 1);
    hour++;
  } else if (min < 45) { // It is twenty to X+1.
    toggle_word(LAYER_TWENTY, 1);
    toggle_word(LAYER_TO, 1);
    hour++;
  } else if (min < 50) { // It is a quarter to X+1.
    toggle_word(LAYER_A, 1);
    toggle_word(LAYER_QUARTER, 1);
    toggle_word(LAYER_TO, 1);
    hour++;
  } else if (min < 55) { // It is ten to X+1.
    toggle_word(LAYER_TEN, 1);
    toggle_word(LAYER_TO, 1);
    hour++;
  } else { // It is five to X+1.
    toggle_word(LAYER_FIVE, 1);
    toggle_word(LAYER_TO, 1);
    hour++;
  }

  // Update the minute box
  minute_num = min % 5;
  layer_mark_dirty(minute_layer);

  // Convert from 24-hour to 12-hour time
  if (hour == 0) hour = 12;
  else if (hour > 12) hour -= 12;

  // Turn on the word needed for the curret hour, turn off the others
  for (int i = 1; i <= 12; i++) {
    toggle_word(i, i == hour ? 1 : 0);
  }
}

