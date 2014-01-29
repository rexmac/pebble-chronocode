enum {
  // it_IT
  LAYER_IT_IT_E_GRAVE = 13,
  LAYER_IT_IT_SONO    = 14,
  LAYER_IT_IT_LE      = 15,
  LAYER_IT_IT_CINQUE  = 16,
  LAYER_IT_IT_DIECI   = 17,
  LAYER_IT_IT_QUARTO  = 18,
  LAYER_IT_IT_VENTI   = 19,
  LAYER_IT_IT_MEZZO   = 20,
  LAYER_IT_IT_E       = 21,
  LAYER_IT_IT_UN      = 22,
  LAYER_IT_IT_MENO    = 23,

  LAYER_FILLER = 27
};

/**
 * Array of array of word_t structs required to create all the phrases for designating time.
 *
 */
static const word_t words[54] = {
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
  [LAYER_IT_IT_CINQUE] = { 7, 6, "CINQUE", "cinque" },
  [LAYER_IT_IT_DIECI]  = { 6, 7, "DIECI", "dieci" },
  [LAYER_IT_IT_QUARTO] = { 8, 6, "QUARTO", "quarto" },
  [LAYER_IT_IT_VENTI]  = { 7, 1, "VENTI", "venti" },
  [LAYER_IT_IT_MEZZO]  = { 8, 0, "MEZZO", "mezzo" },

  // Relative
  [LAYER_IT_IT_SONO]    = { 0, 0, "SONO", "sono" },
  [LAYER_IT_IT_LE]      = { 0, 5, "LE", "le" },
  [LAYER_IT_IT_E]       = { 6, 0, "E", "e" },
  [LAYER_IT_IT_E_GRAVE] = { 1, 0, "È", "è" },
  [LAYER_IT_IT_UN]      = { 6, 5, "UN", "un" },
  [LAYER_IT_IT_MENO]    = { 6, 1, "MENO", "meno" },

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
    { 8,  5, "V", "v" }
};

static const uint8_t intervals[13][5] = {
  // Always on
  { 0, 0, 0, 0, 0 },
  // XX:00 - XX:04
  { 0, 0, 0, 0, 0 },
  // XX:05 - XX:09
  { 0, LAYER_IT_IT_E, LAYER_IT_IT_CINQUE, 0, 0 },
  // XX:10 - XX:14
  { 0, LAYER_IT_IT_E, LAYER_IT_IT_DIECI, 0, 0 },
  // XX:15 - XX:19
  { 0, LAYER_IT_IT_E, LAYER_IT_IT_UN, LAYER_IT_IT_QUARTO, 0 },
  // XX:20 - XX:24
  { 0, LAYER_IT_IT_E, LAYER_IT_IT_VENTI, 0, 0 },
  // XX:25 - XX:29
  { 0, LAYER_IT_IT_E, LAYER_IT_IT_VENTI, LAYER_IT_IT_CINQUE, 0 },
  // XX:30 - XX:34
  { 0, LAYER_IT_IT_E, LAYER_IT_IT_MEZZO, 0, 0 },
  // XX:35 - XX:39
  { 1, LAYER_IT_IT_MENO, LAYER_IT_IT_VENTI, LAYER_IT_IT_CINQUE, 0 },
  // XX:40 - XX:44
  { 1, LAYER_IT_IT_MENO, LAYER_IT_IT_VENTI, 0, 0 },
  // XX:45 - XX:49
  { 1, LAYER_IT_IT_MENO, LAYER_IT_IT_UN, LAYER_IT_IT_QUARTO, 0 },
  // XX:50 - XX:54
  { 1, LAYER_IT_IT_MENO, LAYER_IT_IT_DIECI, 0, 0 },
  // XX:55 - XX:59
  { 1, LAYER_IT_IT_MENO, LAYER_IT_IT_CINQUE, 0, 0 },
};

