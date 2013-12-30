/**
 * Header file for a WordSquare watchface in Italian/Italiano (it_IT) 
 *
 */
#define LAYER_E_GRAVE  0
#define LAYER_SONO    14
#define LAYER_LE      15
#define LAYER_CINQUE  16
#define LAYER_DIECI   17
#define LAYER_QUARTO  18
#define LAYER_VENTI   19
#define LAYER_MEZZO   20
#define LAYER_E       21
#define LAYER_UN      22
#define LAYER_MENO    23

/**
 * Array of words required to create all the phrases for designating time.
 *
 *      it_IT
 *   0123456789ab
 * 0 SONOxLExNOVE
 * 1 ÈxL'UNAxOTTO
 * 2 UNDICICINQUE
 * 3 xTRExSEIxDUE
 * 4 DIECIQUATTRO
 * 5 xDODICISETTE
 * 6 EMENOUNDIECI
 * 7 xVENTICINQUE
 * 8 MEZZOxQUARTO
 *   0123456789ab
 */
static const word_t words[] = {
  // Hours 1-12
  [1]  = { 1, 2, "L'UNA", "l'una" },
  [2]  = { 3, 9, "DUE", "due" },
  [3]  = { 3, 1, "TRE", "tre" },
  [4]  = { 4, 5, "QUATTRO", "quattro" },
  [5]  = { 2, 6, "CINQUE", "cinque" },
  [6]  = { 3, 5, "SEI", "sei" },
  [7]  = { 5, 7, "SETTE", "sette" },
  [8]  = { 1, 8, "OTTO", "otto" },
  [9]  = { 0, 8, "NOVE", "nove" },
  [10] = { 4, 0, "DIECI", "dieci" },
  [11] = { 2, 0, "UNDICI", "undici" },
  [12] = { 5, 1, "DODICI", "dodici" },

  // Minutes before/after the hour
  [LAYER_CINQUE] = { 7, 6, "CINQUE", "cinque" },
  [LAYER_DIECI]  = { 6, 7, "DIECI", "dieci" },
  [LAYER_QUARTO] = { 8, 6, "QUARTO", "quarto" },
  [LAYER_VENTI]  = { 7, 1, "VENTI", "venti" },
  [LAYER_MEZZO]  = { 8, 0, "MEZZO", "mezzo" },

  // Relative
  [LAYER_SONO]    = { 0, 0, "SONO", "sono" },
  [LAYER_LE]      = { 0, 5, "LE", "le" },
  [LAYER_E]       = { 6, 0, "E", "e" },
  [LAYER_E_GRAVE] = { 1, 0, "È", "è" },
  [LAYER_UN]      = { 6, 5, "UN", "un" },
  [LAYER_MENO]    = { 6, 1, "MENO", "meno" },

  // Extra, "random" characters used to fill in empty spaces between words
  [LAYER_FILLER] =
    { 0,  4, "F", "f" },
    { 0,  7, "R", "r" },
    { 1,  1, "Z", "z" },
    { 1,  7, "P", "p" },
    { 3,  0, "A", "a" },
    { 3,  4, "M", "m" },
    { 3,  8, "G", "g" },
    { 5,  0, "L", "l" },
    { 7,  0, "H", "h" },
    { 8,  5, "V", "v" },
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

  // Turn on the words needed for the current time
  if (min < 5) {
    // Sono le X.
  } else if (min < 10) { // Sono le X e cinque.
    toggle_word(LAYER_E, 1);
    toggle_word(LAYER_CINQUE, 1);
  } else if (min < 15) { // Sono le X e dieci.
    toggle_word(LAYER_E, 1);
    toggle_word(LAYER_DIECI, 1);
  } else if (min < 20) { // Sono le X e un quarto.
    toggle_word(LAYER_E, 1);
    toggle_word(LAYER_UN, 1);
    toggle_word(LAYER_QUARTO, 1);
  } else if (min < 25) { // Sono le X e venti.
    toggle_word(LAYER_E, 1);
    toggle_word(LAYER_VENTI, 1);
  } else if (min < 30) { // sono le X e venticinque.
    toggle_word(LAYER_E, 1);
    toggle_word(LAYER_VENTI, 1);
    toggle_word(LAYER_CINQUE, 1);
  } else if (min < 35) { // Sono le X e mezzo.
    toggle_word(LAYER_E, 1);
    toggle_word(LAYER_MEZZO, 1);
  } else if (min < 40) { // Sono le X+1 meno venticinque.
    toggle_word(LAYER_MENO, 1);
    toggle_word(LAYER_VENTI, 1);
    toggle_word(LAYER_CINQUE, 1);
    hour++;
  } else if (min < 45) { // Sono le X+1 meno venti.
    toggle_word(LAYER_MENO, 1);
    toggle_word(LAYER_VENTI, 1);
    hour++;
  } else if (min < 50) { // Sono le X+1 meno un quarto.
    toggle_word(LAYER_MENO, 1);
    toggle_word(LAYER_UN, 1);
    toggle_word(LAYER_QUARTO, 1);
    hour++;
  } else if (min < 55) { // Sono le X+1 meno dieci.
    toggle_word(LAYER_MENO, 1);
    toggle_word(LAYER_DIECI, 1);
    hour++;
  } else { // Sono le X+1 meno cinque.
    toggle_word(LAYER_MENO, 1);
    toggle_word(LAYER_CINQUE, 1);
    hour++;
  }

  // Update the minute box
  minute_num = min % 5;
  layer_mark_dirty(minute_layer);

  // Convert from 24-hour to 12-hour time
  if (hour == 0) hour = 12;
  else if (hour > 12) hour -= 12;

  if (hour == 1) {
    toggle_word(LAYER_E_GRAVE, 1);
  } else {
    toggle_word(LAYER_SONO, 1);
    toggle_word(LAYER_LE, 1);
  }

  // Turn on the word needed for the curret hour, turn off the others
  for (int i = 1; i <= 12; i++) {
    toggle_word(i, i == hour ? 1 : 0);
  }
}

