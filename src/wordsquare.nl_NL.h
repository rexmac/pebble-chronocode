/**
 * Header file for a WordSquare watchface in Dutch/Nederlands (nl_NL).
 *
 */
#define LAYER_HET    0
#define LAYER_IS    14
#define LAYER_VIJF  15
#define LAYER_TIEN  16
#define LAYER_KWART 17
#define LAYER_HALF  18
#define LAYER_OVER  19
#define LAYER_VOOR  20
#define LAYER_UUR   21

/**
 * Array of words required to create all the phrases for designating time.
 *
 *      nl_NL
 *   0123456789ab
 * 0 HETxISxVIJFx
 * 1 xKWARTxxTIEN
 * 2 VOORxxOVERxx
 * 3 HALFxZESxÉÉN
 * 4 xTWAALFxDRIE
 * 5 ZEVENxxVIJFx
 * 6 xTWEExxVIERx
 * 7 ELFxxNEGENxx
 * 8 ACHTTIENxUUR
 *   0123456789ab
 */
static const word_t words[] = {
  // Hours 1-12
  [1]  = { 3, 9, "ÉÉN",    "één"    },
  [2]  = { 6, 1, "TWEE",   "twee"   },
  [3]  = { 4, 8, "DRIE",   "drie"   },
  [4]  = { 6, 7, "VIER",   "vier"   },
  [5]  = { 5, 7, "VIJF",   "vijf"   },
  [6]  = { 3, 5, "ZES",    "zes"    },
  [7]  = { 5, 0, "ZEVEN",  "zeven"  },
  [8]  = { 8, 0, "ACHT",   "acht"   },
  [9]  = { 7, 5, "NEGEN",  "negen"  },
  [10] = { 8, 4, "TIEN",   "tien"   },
  [11] = { 7, 0, "ELF",    "elf"    },
  [12] = { 4, 1, "TWAALF", "twaalf" },

  // Minutes
  [LAYER_VIJF]  = { 0, 7, "VIJF",  "vijf"  },
  [LAYER_TIEN]  = { 1, 8, "ZEHN",  "zehn"  },
  [LAYER_KWART] = { 1, 1, "KWART", "kwart" },
  [LAYER_HALF]  = { 3, 0, "HALF",  "half"  },

  // Relative
  [LAYER_HET]  = { 0, 0, "HET",  "het"  },
  [LAYER_IS]   = { 0, 4, "IS",   "is"   },
  [LAYER_OVER] = { 2, 6, "OVER", "over" },
  [LAYER_VOOR] = { 2, 0, "VOOR", "voor" },
  [LAYER_UUR]  = { 8, 9, "UUR",  "uur"  },

  // Extra, "random" characters used to fill in empty spaces between words
  [LAYER_FILLER] =
    { 0,  3, "R", "r" },
    { 0,  6, "J", "j" },
    { 0, 11, "Z", "z" },
    { 1,  0, "O", "o" },
    { 1,  6, "P", "p" },
    { 1,  7, "L", "l" },
    { 2,  4, "X", "x" },
    { 2,  5, "A", "a" },
    { 2, 10, "K", "k" },
    { 2, 11, "A", "a" },
    { 3,  4, "P", "p" },
    { 3,  8, "U", "u" },
    { 4,  0, "R", "r" },
    { 4,  7, "M", "m" },
    { 5,  5, "O", "o" },
    { 5,  6, "G", "g" },
    { 5, 11, "X", "x" },
    { 6,  0, "C", "c" },
    { 6,  5, "Q", "q" },
    { 6,  6, "B", "b" },
    { 6, 11, "W", "w" },
    { 7,  3, "J", "j" },
    { 7,  4, "U", "u" },
    { 7, 10, "K", "k" },
    { 7, 11, "D", "d" },
    { 8,  8, "O", "o" },
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
  toggle_word(LAYER_HET, 1);
  toggle_word(LAYER_IS, 1);

  // Turn on the words needed for the current time
  if (min < 5) { // Het is X uur.
    toggle_word(LAYER_UUR, 1);
  } else if (min < 10) { // Het is vijf over X.
    toggle_word(LAYER_VIJF, 1);
    toggle_word(LAYER_OVER, 1);
  } else if (min < 15) { // Het is tien over X.
    toggle_word(LAYER_TIEN, 1);
    toggle_word(LAYER_OVER, 1);
  } else if (min < 20) { // Het is kwart over X.
    toggle_word(LAYER_KWART, 1);
    toggle_word(LAYER_OVER, 1);
  } else if (min < 25) { // Het is tien voor half X+1.
    toggle_word(LAYER_TIEN, 1);
    toggle_word(LAYER_VOOR, 1);
    toggle_word(LAYER_HALF, 1);
    hour++;
  } else if (min < 30) { // Het is vijf voor half X+1.
    toggle_word(LAYER_VIJF, 1);
    toggle_word(LAYER_VOOR, 1);
    toggle_word(LAYER_HALF, 1);
    hour++;
  } else if (min < 35) { // Het is half X+1.
    toggle_word(LAYER_HALF, 1);
    hour++;
  } else if (min < 40) { // Het is vijf over half X+1.
    toggle_word(LAYER_VIJF, 1);
    toggle_word(LAYER_OVER, 1);
    toggle_word(LAYER_HALF, 1);
    hour++;
  } else if (min < 45) { // Het is tien over half X+1.
    toggle_word(LAYER_TIEN, 1);
    toggle_word(LAYER_OVER, 1);
    toggle_word(LAYER_HALF, 1);
    hour++;
  } else if (min < 50) { // Het is kwart voor X+1.
    toggle_word(LAYER_KWART, 1);
    toggle_word(LAYER_VOOR, 1);
    hour++;
  } else if (min < 55) { // Het is tien voor X+1.
    toggle_word(LAYER_TIEN, 1);
    toggle_word(LAYER_VOOR, 1);
    hour++;
  } else { // Het is vijf voor X+1.
    toggle_word(LAYER_VIJF, 1);
    toggle_word(LAYER_VOOR, 1);
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

