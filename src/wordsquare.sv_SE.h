/**
 * Header file for a WordSquare watchface in Swedish/Svenska (sv_SE) 
 *
 */
#define LAYER_KLOCKAN   0
#define LAYER_AR       14
#define LAYER_FEM      15
#define LAYER_TIO      16
#define LAYER_KVART    17
#define LAYER_TJUGO    18
#define LAYER_HALV     19
#define LAYER_OVER     20
#define LAYER_I        21

/**
 * Array of words required to create all the phrases for designating time.
 *
 *      da_DK
 *   0123456789ab
 * 0 KLOCKANxÄRxx
 * 1 xTJUGOxxTIOx
 * 2 xFEMxxKVARTx
 * 3 xxxxxxxxxxxx
 * 4 ÖVERxIxHALVx
 * 5 TVÅxELVAxTIO
 * 6 TRETOLVxxFEM
 * 7 xFYRAxNIOSJU
 * 8 ETTxSEXxÅTTA
 *   0123456789ab
 */
static const word_t words[] = {
  // Hours 1-12
  [1]  = { 8,  0, "ETT", "ett" },
  [2]  = { 5,  0, "TVÅ", "två" },
  [3]  = { 6,  0, "TRE", "tre" },
  [4]  = { 7,  1, "FYRA", "fyra" },
  [5]  = { 6,  9, "FEM", "fem" },
  [6]  = { 8,  4, "SEX", "sex" },
  [7]  = { 7,  9, "SJU", "sju" },
  [8]  = { 8,  8, "ÅTTA", "åtta" },
  [9]  = { 7,  6, "NIO", "nio" },
  [10] = { 5,  9, "TIO", "tio" },
  [11] = { 5,  4, "ELVA", "elva" },
  [12] = { 6,  3, "TOLV", "tolv" },

  // Minutes before/after the hour
  [LAYER_FEM]   = { 2, 1, "FEM", "fem" },
  [LAYER_TIO]   = { 1, 8, "TIO", "tio" },
  [LAYER_KVART] = { 2, 6, "KVART", "kvart" },
  [LAYER_TJUGO] = { 1, 2, "TJUGO", "tjugo" },
  [LAYER_HALV]  = { 4, 7, "HALV", "halv" },

  // Relative
  [LAYER_KLOCKAN]  = { 0, 0, "KLOCKAN", "klockan" },
  [LAYER_AR]       = { 0, 8, "ÄR", "är" },
  [LAYER_I]        = { 4, 5, "I", "i" },
  [LAYER_OVER]     = { 4, 0, "ÖVER", "över" },

  // Extra, "random" characters used to fill in empty spaces between words
  [LAYER_FILLER] =
    { 0,  7, "P", "p" },
    { 0, 10, "YT", "yt" },
    { 1,  0, "S", "s" },
    { 1,  1, "H", "h" },
    { 1,  7, "C", "c" },
    { 1, 11, "X", "x" },
    { 2,  0, "U", "u" },
    { 2,  4, "Z", "z" },
    { 2,  5, "I", "i" },
    { 2, 11, "J", "j" },
    { 3,  0, "detärenpbble", "detärenpbble" },
    { 4,  4, "M", "m" },
    { 4,  6, "P", "p" },
    { 4, 11, "Z", "z" },
    { 5,  3, "J", "j" },
    { 5,  8, "O", "o" },
    { 6,  7, "B", "b" },
    { 6,  8, "U", "u" },
    { 7,  0, "S", "s" },
    { 7,  5, "C", "c" },
    { 8,  3, "L", "l" },
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
  toggle_word(LAYER_KLOCKAN, 1);
  toggle_word(LAYER_AR, 1);

  // Turn on the words needed for the current time
  if (min < 5) { // Klokken är X.
  } else if (min < 10) { // Klokken är fem over X.
    toggle_word(LAYER_FEM, 1);
    toggle_word(LAYER_OVER, 1);
  } else if (min < 15) { // Klokken är ti over X.
    toggle_word(LAYER_TIO, 1);
    toggle_word(LAYER_OVER, 1);
  } else if (min < 20) { // Klokken är kvart over X.
    toggle_word(LAYER_KVART, 1);
    toggle_word(LAYER_OVER, 1);
  } else if (min < 25) { // Klokken är tyve over X.
    toggle_word(LAYER_TJUGO, 1);
    toggle_word(LAYER_OVER, 1);
  } else if (min < 30) { // Klokken är fem i halv X+1.
    toggle_word(LAYER_FEM, 1);
    toggle_word(LAYER_I, 1);
    toggle_word(LAYER_HALV, 1);
  } else if (min < 35) { // Klokken är halv X+1.
    toggle_word(LAYER_HALV, 1);
  } else if (min < 40) { // Klokken är fem over halv X+1.
    toggle_word(LAYER_FEM, 1);
    toggle_word(LAYER_OVER, 1);
    toggle_word(LAYER_HALV, 1);
    hour++;
  } else if (min < 45) { // Klokken är tyve i X+1.
    toggle_word(LAYER_TJUGO, 1);
    toggle_word(LAYER_I, 1);
    hour++;
  } else if (min < 50) { // Klokken är kvart i X+1.
    toggle_word(LAYER_KVART, 1);
    toggle_word(LAYER_I, 1);
    hour++;
  } else if (min < 55) { // Klokken är ti i X+1.
    toggle_word(LAYER_TIO, 1);
    toggle_word(LAYER_I, 1);
    hour++;
  } else { // Klokken är fem i X+1.
    toggle_word(LAYER_FEM, 1);
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

