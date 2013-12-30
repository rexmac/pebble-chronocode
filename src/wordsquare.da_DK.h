/**
 * Header file for a WordSquare watchface in Danish/Dansk (da_DK) 
 *
 */
#define LAYER_KLOKKEN   0
#define LAYER_ER       14
#define LAYER_FEM      15
#define LAYER_TI       16
#define LAYER_KVART    17
#define LAYER_TYVE     18
#define LAYER_HALV     19
#define LAYER_OVER     20
#define LAYER_I        21
#define LAYER_MINUTTER 22

/**
 * Array of words required to create all the phrases for designating time.
 *
 *      da_DK
 *   0123456789ab
 * 0 KLOKKENxERxx
 * 1 TYVExTIKVART
 * 2 FEMxMINUTTER
 * 3 xxxxxxxxxxxx
 * 4 OVERxIxHALVx
 * 5 ETxELLEVExTI
 * 6 TRETOLVxxFEM
 * 7 xFIRExNIxSYV
 * 8 TOxSEKSxOTTE
 *   0123456789ab
 */
static const word_t words[] = {
  // Hours 1-12
  [1]  = { 5,  0, "ET", "et" },
  [2]  = { 8,  0, "TO", "to" },
  [3]  = { 6,  0, "TRE", "tre" },
  [4]  = { 7,  1, "FIRE", "fire" },
  [5]  = { 6,  9, "FEM", "fem" },
  [6]  = { 8,  3, "SEKS", "seks" },
  [7]  = { 7,  9, "SYV", "syv" },
  [8]  = { 8,  8, "OTTE", "otte" },
  [9]  = { 7,  6, "NI", "ni" },
  [10] = { 5, 10, "TI", "ti" },
  [11] = { 5,  3, "ELLEVE", "elleve" },
  [12] = { 6,  3, "TOLV", "tolv" },

  // Minutes before/after the hour
  [LAYER_FEM]   = { 2, 0, "FEM", "fem" },
  [LAYER_TI]    = { 1, 5, "TI", "ti" },
  [LAYER_KVART] = { 1, 7, "KVART", "kvart" },
  [LAYER_TYVE]  = { 1, 0, "TYVE", "tyve" },
  [LAYER_HALV]  = { 4, 7, "HALV", "halv" },

  // Relative
  [LAYER_KLOKKEN]  = { 0, 0, "KLOKKEN", "klokken" },
  [LAYER_ER]       = { 0, 8, "ER", "er" },
  [LAYER_I]        = { 4, 5, "I", "i" },
  [LAYER_OVER]     = { 4, 0, "OVER", "over" },
  [LAYER_MINUTTER] = { 2, 4, "MINUTTER", "minutter" },

  // Extra, "random" characters used to fill in empty spaces between words
  [LAYER_FILLER] =
    { 0,  7, "P", "p" },
    { 0, 10, "YT", "yt" },
    { 1,  4, "C", "c" },
    { 2,  3, "Z", "z" },
    { 3,  0, "deterenpbble", "deterenpbble" },
    { 4,  4, "G", "g" },
    { 4,  6, "P", "p" },
    { 4, 11, "Z", "z" },
    { 5,  2, "J", "j" },
    { 5,  9, "Ø", "ø" },
    { 6,  7, "B", "b" },
    { 6,  8, "U", "u" },
    { 7,  0, "S", "s" },
    { 7,  5, "C", "c" },
    { 7,  8, "Å", "å" },
    { 8,  2, "L", "l" },
    { 8,  7, "Y", "y" },
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
  toggle_word(LAYER_KLOKKEN, 1);
  toggle_word(LAYER_ER, 1);

  // Turn on the words needed for the current time
  if (min < 5) { // Klokken er X.
  } else if (min < 10) { // Klokken er fem minutter over X.
    toggle_word(LAYER_FEM, 1);
    toggle_word(LAYER_MINUTTER, 1);
    toggle_word(LAYER_OVER, 1);
  } else if (min < 15) { // Klokken er ti minutter over X.
    toggle_word(LAYER_TI, 1);
    toggle_word(LAYER_MINUTTER, 1);
    toggle_word(LAYER_OVER, 1);
  } else if (min < 20) { // Klokken er kvart over X.
    toggle_word(LAYER_KVART, 1);
    toggle_word(LAYER_OVER, 1);
  } else if (min < 25) { // Klokken er tyve minutter over X.
    toggle_word(LAYER_TYVE, 1);
    toggle_word(LAYER_MINUTTER, 1);
    toggle_word(LAYER_OVER, 1);
  } else if (min < 30) { // Klokken er fem minutter i halv X+1.
    toggle_word(LAYER_FEM, 1);
    toggle_word(LAYER_MINUTTER, 1);
    toggle_word(LAYER_I, 1);
    toggle_word(LAYER_HALV, 1);
  } else if (min < 35) { // Klokken er halv X+1.
    toggle_word(LAYER_HALV, 1);
  } else if (min < 40) { // Klokken er fem minutter over halv X+1.
    toggle_word(LAYER_FEM, 1);
    toggle_word(LAYER_MINUTTER, 1);
    toggle_word(LAYER_OVER, 1);
    toggle_word(LAYER_HALV, 1);
    hour++;
  } else if (min < 45) { // Klokken er tyve minutter i X+1.
    toggle_word(LAYER_TYVE, 1);
    toggle_word(LAYER_MINUTTER, 1);
    toggle_word(LAYER_I, 1);
    hour++;
  } else if (min < 50) { // Klokken er kvart i X+1.
    toggle_word(LAYER_KVART, 1);
    toggle_word(LAYER_I, 1);
    hour++;
  } else if (min < 55) { // Klokken er ti minutter i X+1.
    toggle_word(LAYER_TI, 1);
    toggle_word(LAYER_MINUTTER, 1);
    toggle_word(LAYER_I, 1);
    hour++;
  } else { // Klokken er fem minutter i X+1.
    toggle_word(LAYER_FEM, 1);
    toggle_word(LAYER_MINUTTER, 1);
    toggle_word(LAYER_I, 1);
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

