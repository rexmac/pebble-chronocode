/**
 * Header file for a WordSquare watchface in German/Deutsch (de_DE).
 *
 */
#define LAYER_ES       0
#define LAYER_IST     14
#define LAYER_FUENF   15
#define LAYER_ZEHN    16
#define LAYER_VIERTEL 17
#define LAYER_ZWANZIG 18
#define LAYER_HALB    19
#define LAYER_NACH    20
#define LAYER_VOR     21
#define LAYER_UHR     22

/**
 * Array of words required to create all the phrases for designating time.
 *
 * Based on the work of Robert Gies (https://github.com/rgies)
 *
 *      de_DE
 *   0123456789ab
 * 0 ESxISTxFÜNFx
 * 1 ZEHNxZWANZIG
 * 2 VIERTELxNACH
 * 3 VORxHALBxEIN
 * 4 xZWÖLFxDREIx
 * 5 SIEBENxxFÜNF
 * 6 xZWEIELFNEUN
 * 7 VIERACHTZEHN
 * 8 SECHSEINSUHR
 *   0123456789ab
 */
static const word_t words[] = {
  // Hours 1-12
  [1]  = { 3, 9, "EIN",    "ein"    },
  [2]  = { 6, 1, "ZWEI",   "zwei"   },
  [3]  = { 4, 7, "DREI",   "drei"   },
  [4]  = { 7, 0, "VIER",   "vier"   },
  [5]  = { 5, 8, "FÜNF",   "fünf"   },
  [6]  = { 8, 0, "SECHS",  "sechs"  },
  [7]  = { 5, 0, "SIEBEN", "sieben" },
  [8]  = { 7, 4, "ACHT",   "acht"   },
  [9]  = { 6, 8, "NEUN",   "neun"   },
  [10] = { 7, 8, "ZEHN",   "zehn"   },
  [11] = { 6, 5, "ELF",    "elf"    },
  [12] = { 4, 1, "ZWÖLF",  "zwölf"  },
  [13] = { 8, 5, "EINS",   "eins"   },

  // Minutes before/after the hour
  [LAYER_FUENF]   = { 0, 7, "FÜNF",    "fünf"    },
  [LAYER_ZEHN]    = { 1, 0, "ZEHN",    "zehn"    },
  [LAYER_VIERTEL] = { 2, 0, "VIERTEL", "viertel" },
  [LAYER_HALB]    = { 3, 4, "HALB",    "halb"    },
  [LAYER_ZWANZIG] = { 1, 5, "ZWANZIG", "zwanzig" },

  // Relative
  [LAYER_ES]   = { 0, 0, "ES",   "es"   },
  [LAYER_IST]  = { 0, 3, "IST",  "ist"  },
  [LAYER_NACH] = { 2, 8, "NACH", "nach" },
  [LAYER_VOR]  = { 3, 0, "VOR",  "vor"  },
  [LAYER_UHR]  = { 8, 9, "UHR",  "uhr"  },

  // Extra, "random" characters used to fill in empty spaces between words
  [LAYER_FILLER] =
    { 0,  2, "Z", "z" },
    { 0,  6, "A", "a" },
    { 0, 11, "T", "t" },
    { 1,  4, "L", "l" },
    { 2,  7, "K", "k" },
    { 3,  3, "P", "p" },
    { 3,  8, "O", "o" },
    { 4,  0, "F", "f" },
    { 4,  6, "G", "g" },
    { 4, 11, "P", "p" },
    { 5,  6, "J", "j" },
    { 5,  7, "T", "t" },
    { 6,  0, "Ä", "ä" },
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
  toggle_word(LAYER_UHR, 0);
  toggle_word(LAYER_FUENF, 0);
  toggle_word(LAYER_ZEHN, 0);
  toggle_word(LAYER_VIERTEL, 0);
  toggle_word(LAYER_ZWANZIG, 0);
  toggle_word(LAYER_HALB, 0);
  toggle_word(LAYER_NACH, 0);
  toggle_word(LAYER_VOR, 0);

  // Turn on the words that are always on
  toggle_word(LAYER_ES, 1);
  toggle_word(LAYER_IST, 1);

  // Turn on the words needed for the current time
  if (min < 5) { // Es ist X uhr.
    toggle_word(LAYER_UHR, 1);
  } else if (min < 10) { // Es ist fünf nach X.
    toggle_word(LAYER_FUENF, 1);
    toggle_word(LAYER_NACH, 1);
  } else if (min < 15) { // Es ist zehn nach X.
    toggle_word(LAYER_ZEHN, 1);
    toggle_word(LAYER_NACH, 1);
  } else if (min < 20) { // Es ist viertel nach X.
    toggle_word(LAYER_VIERTEL, 1);
    toggle_word(LAYER_NACH, 1);
  } else if (min < 25) { // Es ist zwanzig nach X.
    toggle_word(LAYER_ZWANZIG, 1);
    toggle_word(LAYER_NACH, 1);
  } else if (min < 30) { // Es ist fünf vor halb X+1.
    toggle_word(LAYER_FUENF, 1);
    toggle_word(LAYER_VOR, 1);
    toggle_word(LAYER_HALB, 1);
    hour++;
  } else if (min < 35) { // Es ist halb X+1.
    toggle_word(LAYER_HALB, 1);
    hour++;
  } else if (min < 40) { // Es ist fünf nach halb X+1.
    toggle_word(LAYER_FUENF, 1);
    toggle_word(LAYER_NACH, 1);
    toggle_word(LAYER_HALB, 1);
    hour++;
  } else if (min < 45) { // Es ist zwanzig vor X+1.
    toggle_word(LAYER_ZWANZIG, 1);
    toggle_word(LAYER_VOR, 1);
    hour++;
  } else if (min < 50) { // Es ist viertel vor X+1.
    toggle_word(LAYER_VIERTEL, 1);
    toggle_word(LAYER_VOR, 1);
    hour++;
  } else if (min < 55) { // Es ist zehn vor X+1.
    toggle_word(LAYER_ZEHN, 1);
    toggle_word(LAYER_VOR, 1);
    hour++;
  } else { // Es ist fünf vor X+1.
    toggle_word(LAYER_FUENF, 1);
    toggle_word(LAYER_VOR, 1);
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

  // Use "eins" instead of "ein"
  if (hour == 1 && min >= 5) {
    toggle_word(1, 0);
    toggle_word(13, 1);
  } else {
    toggle_word(13, 0);
  }
}

