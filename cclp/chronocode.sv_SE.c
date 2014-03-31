enum {
  // sv_SE
  LAYER_SV_SE_KLOCKAN = 13,
  LAYER_SV_SE_AR      = 14,
  LAYER_SV_SE_FEM     = 15,
  LAYER_SV_SE_TIO     = 16,
  LAYER_SV_SE_KVART   = 17,
  LAYER_SV_SE_TJUGO   = 18,
  LAYER_SV_SE_HALV    = 19,
  LAYER_SV_SE_OVER    = 20,
  LAYER_SV_SE_I       = 21,

  LAYER_FILLER = 27
};

/**
 * Array of array of word_t structs required to create all the phrases for designating time.
 *
 */
static const word_t words[54] = {
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
  [LAYER_SV_SE_FEM]   = { 2, 1, "FEM", "fem" },
  [LAYER_SV_SE_TIO]   = { 1, 8, "TIO", "tio" },
  [LAYER_SV_SE_KVART] = { 2, 6, "KVART", "kvart" },
  [LAYER_SV_SE_TJUGO] = { 1, 2, "TJUGO", "tjugo" },
  [LAYER_SV_SE_HALV]  = { 4, 7, "HALV", "halv" },

  // Relative
  [LAYER_SV_SE_KLOCKAN]  = { 0, 0, "KLOCKAN", "klockan" },
  [LAYER_SV_SE_AR]       = { 0, 8, "ÄR", "är" },
  [LAYER_SV_SE_I]        = { 4, 5, "I", "i" },
  [LAYER_SV_SE_OVER]     = { 4, 0, "ÖVER", "över" },

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
    { 3,  0, "DETÄRENPBBLE", "detärenpbble" },
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
    { 8,  7, "Y", "y" }
};

static const uint8_t intervals[13][5] = {
  // Always on
  { 0, LAYER_SV_SE_KLOCKAN, LAYER_SV_SE_AR, 0, 0 },
  // XX:00 - XX:04
  { 0, 0, 0, 0, 0 },
  // XX:05 - XX:09
  { 0, LAYER_SV_SE_FEM, LAYER_SV_SE_OVER, 0, 0 },
  // XX:10 - XX:14
  { 0, LAYER_SV_SE_TIO, LAYER_SV_SE_OVER, 0, 0 },
  // XX:15 - XX:19
  { 0, LAYER_SV_SE_KVART, LAYER_SV_SE_OVER, 0, 0 },
  // XX:20 - XX:24
  { 0, LAYER_SV_SE_TJUGO, LAYER_SV_SE_OVER, 0, 0 },
  // XX:25 - XX:29
  { 1, LAYER_SV_SE_FEM, LAYER_SV_SE_I, LAYER_SV_SE_HALV, 0 },
  // XX:30 - XX:34
  { 1, LAYER_SV_SE_HALV, 0, 0, 0 },
  // XX:35 - XX:39
  { 1, LAYER_SV_SE_FEM, LAYER_SV_SE_OVER, LAYER_SV_SE_HALV, 1 },
  // XX:40 - XX:44
  { 1, LAYER_SV_SE_TJUGO, LAYER_SV_SE_I, 0, 0 },
  // XX:45 - XX:49
  { 1, LAYER_SV_SE_KVART, LAYER_SV_SE_I, 0, 0 },
  // XX:50 - XX:54
  { 1, LAYER_SV_SE_TIO, LAYER_SV_SE_I, 0, 0 },
  // XX:55 - XX:59
  { 1, LAYER_SV_SE_FEM, LAYER_SV_SE_I, 0, 0 },
};

