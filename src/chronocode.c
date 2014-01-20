/**
 * ChronoCode watchface for the Pebble Smartwatch
 *
 * @version 2.1.1
 * @license New BSD License (please see LICENSE file)
 * @repo https://github.com/rexmac/pebble-chronocode
 * @author Rex McConnell <rex@rexmac.com>
 */
#include <pebble.h>

#define FONT_H 18
#define FONT_W 12
#define FONT_ON  RESOURCE_ID_FONT_SOURCECODEPRO_BLACK_20
#define FONT_OFF RESOURCE_ID_FONT_SOURCECODEPRO_LIGHT_20

enum { LAYER_FILLER = 27 };

// Settings (bit) flags
enum {
  SETTING_ALL_CAPS = 1 << 0,
  SETTING_INVERTED = 1 << 1
};

// Settings AppSync keys; correspond to appKeys in appinfo.json
enum {
  SETTING_SYNC_KEY_ALL_CAPS = 0,
  SETTING_SYNC_KEY_INVERTED = 1,
  SETTING_SYNC_KEY_LANGUAGE = 2,
};

// Language IDs
enum language_id {
  LANG_EN_US = 0,
  LANG_DA_DK = 1,
  LANG_DE_DE = 2,
  LANG_ES_ES = 3,
  LANG_FR_FR = 4,
  LANG_IT_IT = 5,
  LANG_NL_NL = 6,
  LANG_SV_SE = 7
};

/**
 * Attributes of a single word.
 *
 */
typedef struct {
  int row; /**< The row coordinate of the word's first letter */
  int col; /**< The column coordinate of the word's first letter */
  char text_on[13];  /**< The string to display when the word is "on" */
  char text_off[13]; /**< The string to display when the word is "off" */
} word_t;

static Window *window; /**< The Pebble window */
static TextLayer *text_layers[54]; /**< Array of text layers for displaying the words */
static GFont font_on;  /**< The font used for words that are inactive or "off" */
static GFont font_off; /**< The font used for words that are active or "on" */
static AppSync settings_sync; /**< Keeps settings in sync between phone and watch */
static uint8_t settings_sync_buffer[54]; /**< Buffer used by settings sync */
static Layer *minute_layer; /**< The layer onto which is drawn the box/dot representing the minute_num */
static int minute_num; /**< The number of minutes (1-4) since the last five minute interval */
static uint8_t settings; /**< Current settings (as bit flags) */
static uint8_t language_setting;
static uint8_t word_count;

enum {
  // da_DK
  LAYER_DA_DK_KLOKKEN  =  0,
  LAYER_DA_DK_ER       = 14,
  LAYER_DA_DK_FEM      = 15,
  LAYER_DA_DK_TI       = 16,
  LAYER_DA_DK_KVART    = 17,
  LAYER_DA_DK_TYVE     = 18,
  LAYER_DA_DK_HALV     = 19,
  LAYER_DA_DK_OVER     = 20,
  LAYER_DA_DK_I        = 21,
  LAYER_DA_DK_MINUTTER = 22,
  // de_DE
  LAYER_DE_DE_ES      =  0,
  LAYER_DE_DE_IST     = 14,
  LAYER_DE_DE_FUENF   = 15,
  LAYER_DE_DE_ZEHN    = 16,
  LAYER_DE_DE_VIERTEL = 17,
  LAYER_DE_DE_ZWANZIG = 18,
  LAYER_DE_DE_HALB    = 19,
  LAYER_DE_DE_NACH    = 20,
  LAYER_DE_DE_VOR     = 21,
  LAYER_DE_DE_UHR     = 22,
  // en_US
  LAYER_EN_US_IT      =  0,
  LAYER_EN_US_IS      = 14,
  LAYER_EN_US_A       = 15,
  LAYER_EN_US_FIVE    = 16,
  LAYER_EN_US_TEN     = 17,
  LAYER_EN_US_QUARTER = 18,
  LAYER_EN_US_HALF    = 19,
  LAYER_EN_US_TWENTY  = 20,
  LAYER_EN_US_PAST    = 21,
  LAYER_EN_US_TO      = 22,
  LAYER_EN_US_OCLOCK  = 23,
  // es_ES
  LAYER_ES_ES_ES          =  0,
  LAYER_ES_ES_LA          = 14,
  LAYER_ES_ES_SON         = 15,
  LAYER_ES_ES_LAS         = 16,
  LAYER_ES_ES_CINCO       = 17,
  LAYER_ES_ES_DIEZ        = 18,
  LAYER_ES_ES_CUARTO      = 19,
  LAYER_ES_ES_VEINTE      = 20,
  LAYER_ES_ES_VEINTICINCO = 21,
  LAYER_ES_ES_MEDIA       = 22,
  LAYER_ES_ES_Y           = 23,
  LAYER_ES_ES_MENOS       = 24,
  // fr_FR
  LAYER_FR_FR_IL     =  0,
  LAYER_FR_FR_EST    = 14,
  LAYER_FR_FR_CINQ   = 15,
  LAYER_FR_FR_DIX    = 16,
  LAYER_FR_FR_QUART  = 17,
  LAYER_FR_FR_VINGT  = 18,
  LAYER_FR_FR_TRAIT  = 19,
  LAYER_FR_FR_DEMIE  = 20,
  LAYER_FR_FR_ET     = 21,
  LAYER_FR_FR_LE     = 22,
  LAYER_FR_FR_MOINS  = 23,
  LAYER_FR_FR_HEURE  = 24,
  LAYER_FR_FR_HEURES = 25,
  // it_IT
  LAYER_IT_IT_E_GRAVE =  0,
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
  // nl_NL
  LAYER_NL_NL_HET   =  0,
  LAYER_NL_NL_IS    = 14,
  LAYER_NL_NL_VIJF  = 15,
  LAYER_NL_NL_TIEN  = 16,
  LAYER_NL_NL_KWART = 17,
  LAYER_NL_NL_HALF  = 18,
  LAYER_NL_NL_OVER  = 19,
  LAYER_NL_NL_VOOR  = 20,
  LAYER_NL_NL_UUR   = 21,
  // sv_SE
  LAYER_SV_SE_KLOCKAN =  0,
  LAYER_SV_SE_AR      = 14,
  LAYER_SV_SE_FEM     = 15,
  LAYER_SV_SE_TIO     = 16,
  LAYER_SV_SE_KVART   = 17,
  LAYER_SV_SE_TJUGO   = 18,
  LAYER_SV_SE_HALV    = 19,
  LAYER_SV_SE_OVER    = 20,
  LAYER_SV_SE_I       = 21,
};

/**
 * Array of array of word_t structs required to create all the phrases for designating time.
 *
 */
static const word_t words[][54] = {
  [LANG_EN_US] = {
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

    // Minutes before/after the hour
    [LAYER_EN_US_FIVE]    = {  2, 8, "FIVE",    "five"    },
    [LAYER_EN_US_TEN]     = {  1, 9, "TEN",     "ten"     },
    [LAYER_EN_US_A]       = {  1, 0, "A",       "a"       },
    [LAYER_EN_US_QUARTER] = {  1, 2, "QUARTER", "quarter" },
    [LAYER_EN_US_HALF]    = {  3, 0, "HALF",    "half"    },
    [LAYER_EN_US_TWENTY]  = {  2, 1, "TWENTY",  "twenty"  },

    // Relative
    [LAYER_EN_US_IT]     = {  0, 0, "IT",     "it"     },
    [LAYER_EN_US_IS]     = {  0, 3, "IS",     "is"     },
    [LAYER_EN_US_PAST]   = {  3, 5, "PAST",   "past"   },
    [LAYER_EN_US_TO]     = {  3, 9, "TO",     "to",    },
    [LAYER_EN_US_OCLOCK] = {  8, 6, "OCLOCK", "oclock" },

    // Extra, "random" characters used to fill in empty spaces between words
    [LAYER_FILLER] =
      {  0,  2, "Z", "z" },
      {  0,  5, "APEBBLE", "apebble" },
      {  1,  1, "T", "t" },
      {  2,  0, "K", "k" },
      {  2,  7, "N", "n" },
      {  3,  4, "B", "o" },
      {  3, 11, "F", "f" },
      {  6,  3, "E", "e" },
      {  7, 11, "S", "s" },
      {  8,  5, "H", "h" }
  },
  [LANG_DA_DK] = {
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
    [LAYER_DA_DK_FEM]   = { 2, 0, "FEM", "fem" },
    [LAYER_DA_DK_TI]    = { 1, 5, "TI", "ti" },
    [LAYER_DA_DK_KVART] = { 1, 7, "KVART", "kvart" },
    [LAYER_DA_DK_TYVE]  = { 1, 0, "TYVE", "tyve" },
    [LAYER_DA_DK_HALV]  = { 4, 7, "HALV", "halv" },

    // Relative
    [LAYER_DA_DK_KLOKKEN]  = { 0, 0, "KLOKKEN", "klokken" },
    [LAYER_DA_DK_ER]       = { 0, 8, "ER", "er" },
    [LAYER_DA_DK_I]        = { 4, 5, "I", "i" },
    [LAYER_DA_DK_OVER]     = { 4, 0, "OVER", "over" },
    [LAYER_DA_DK_MINUTTER] = { 2, 4, "MINUTTER", "minutter" },

    // Extra, "random" characters used to fill in empty spaces between words
    [LAYER_FILLER] =
      { 0,  7, "P", "p" },
      { 0, 10, "YT", "yt" },
      { 1,  4, "C", "c" },
      { 2,  3, "Z", "z" },
      { 3,  0, "DETERENPBBLE", "deterenpbble" },
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
      { 8,  7, "Y", "y" }
  },
  [LANG_DE_DE] = {
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
    [LAYER_DE_DE_FUENF]   = { 0, 7, "FÜNF",    "fünf"    },
    [LAYER_DE_DE_ZEHN]    = { 1, 0, "ZEHN",    "zehn"    },
    [LAYER_DE_DE_VIERTEL] = { 2, 0, "VIERTEL", "viertel" },
    [LAYER_DE_DE_HALB]    = { 3, 4, "HALB",    "halb"    },
    [LAYER_DE_DE_ZWANZIG] = { 1, 5, "ZWANZIG", "zwanzig" },

    // Relative
    [LAYER_DE_DE_ES]   = { 0, 0, "ES",   "es"   },
    [LAYER_DE_DE_IST]  = { 0, 3, "IST",  "ist"  },
    [LAYER_DE_DE_NACH] = { 2, 8, "NACH", "nach" },
    [LAYER_DE_DE_VOR]  = { 3, 0, "VOR",  "vor"  },
    [LAYER_DE_DE_UHR]  = { 8, 9, "UHR",  "uhr"  },

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
      { 6,  0, "Ä", "ä" }
  },
  [LANG_ES_ES] = {
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

    // Minutes before/after the hour
    [LAYER_ES_ES_CINCO]       = { 6,  7, "CINCO",       "cinco"       },
    [LAYER_ES_ES_DIEZ]        = { 7,  0, "DIEZ",        "diez"        },
    [LAYER_ES_ES_CUARTO]      = { 8,  0, "CUARTO",      "cuarto"      },
    [LAYER_ES_ES_VEINTE]      = { 7,  5, "VEINTE",      "veinte"      },
    [LAYER_ES_ES_VEINTICINCO] = { 6,  1, "VEINTICINCO", "veinticinco" },
    [LAYER_ES_ES_MEDIA]       = { 8,  7, "MEDIA",       "media"       },
    [LAYER_ES_ES_MENOS]       = { 5,  6, "MENOS",       "menos"       },
    [LAYER_ES_ES_Y]           = { 5, 11, "Y",           "y"           },

    // Relative
    [LAYER_ES_ES_ES]  = { 0, 0, "ES",  "e "  },
    [LAYER_ES_ES_SON] = { 0, 1, "SON", " on" },
    [LAYER_ES_ES_LA]  = { 0, 5, "LA",  "  "  },
    [LAYER_ES_ES_LAS] = { 0, 5, "LAS", "  s" },

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
      { 8,  6, "K", "k" }
  },
  [LANG_FR_FR] = {
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

    // Minutes before/after the hour
    [LAYER_FR_FR_CINQ]  = { 7, 8, "CINQ",  "cinq"  },
    [LAYER_FR_FR_DIX]   = { 6, 8, "DIX",   "dix"   },
    [LAYER_FR_FR_QUART] = { 8, 7, "QUART", "quart" },
    [LAYER_FR_FR_VINGT] = { 7, 2, "VINGT", "vingt" },
    [LAYER_FR_FR_TRAIT] = { 7, 7, "-",     "-"     },
    [LAYER_FR_FR_DEMIE] = { 8, 2, "DEMIE", "demie" },

    // Relative
    [LAYER_FR_FR_IL]     = { 0, 0, "IL",     "il"     },
    [LAYER_FR_FR_EST]    = { 0, 3, "EST",    "est"    },
    [LAYER_FR_FR_ET]     = { 7, 0, "ET",     "et"     },
    [LAYER_FR_FR_LE]     = { 8, 0, "LE",     "le"     },
    [LAYER_FR_FR_MOINS]  = { 6, 2, "MOINS",  "moins"  },
    [LAYER_FR_FR_HEURE]  = { 5, 6, "HEURE",  ""       },
    [LAYER_FR_FR_HEURES] = { 5, 6, "HEURES", "     s" },

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
      { 6, 11, "L", "l" }
  },
  [LANG_IT_IT] = {
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
  },
  [LANG_NL_NL] = {
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

    // Minutes before/after the hour
    [LAYER_NL_NL_VIJF]  = { 0, 7, "VIJF",  "vijf"  },
    [LAYER_NL_NL_TIEN]  = { 1, 8, "TIEN",  "tien"  },
    [LAYER_NL_NL_KWART] = { 1, 1, "KWART", "kwart" },
    [LAYER_NL_NL_HALF]  = { 3, 0, "HALF",  "half"  },

    // Relative
    [LAYER_NL_NL_HET]  = { 0, 0, "HET",  "het"  },
    [LAYER_NL_NL_IS]   = { 0, 4, "IS",   "is"   },
    [LAYER_NL_NL_OVER] = { 2, 6, "OVER", "over" },
    [LAYER_NL_NL_VOOR] = { 2, 0, "VOOR", "voor" },
    [LAYER_NL_NL_UUR]  = { 8, 9, "UUR",  "uur"  },

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
      { 8,  8, "O", "o" }
  },
  [LANG_SV_SE] = {
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
  }
};

/**
 * Toggle the on/off state of a single word.
 *
 * @param which The index of the word to toggle
 * @param on    Desired on/off state of the word
 */
static void toggle_word(int which, int on) {
  const word_t * const w = &words[language_setting][which];

  text_layer_set_text(text_layers[which], on ? w->text_on : ((settings & SETTING_ALL_CAPS) > 0 ? w->text_on : w->text_off));
  text_layer_set_font(text_layers[which], on ? font_on : font_off);
}

/**
 * Update time display.
 *
 * @todo Optimize switch statement to (hopefully) reduce code and file size
 *
 * @param time The time to be disaplyed
 */
static void update_time(struct tm *time) {
  int hour = time->tm_hour;
  int min = time->tm_min;

  // Turn off all of the words
  for (unsigned i = 0; i < word_count; i++) {
    toggle_word(i, 0);
  }

  switch(language_setting) {
    case LANG_DA_DK:
      // Turn on the words that are always on
      toggle_word(LAYER_DA_DK_KLOKKEN, 1);
      toggle_word(LAYER_DA_DK_ER, 1);

      // Turn on the words needed for the current time
      if (min < 5) { // Klokken er X.
      } else if (min < 10) { // Klokken er fem minutter over X.
        toggle_word(LAYER_DA_DK_FEM, 1);
        toggle_word(LAYER_DA_DK_MINUTTER, 1);
        toggle_word(LAYER_DA_DK_OVER, 1);
      } else if (min < 15) { // Klokken er ti minutter over X.
        toggle_word(LAYER_DA_DK_TI, 1);
        toggle_word(LAYER_DA_DK_MINUTTER, 1);
        toggle_word(LAYER_DA_DK_OVER, 1);
      } else if (min < 20) { // Klokken er kvart over X.
        toggle_word(LAYER_DA_DK_KVART, 1);
        toggle_word(LAYER_DA_DK_OVER, 1);
      } else if (min < 25) { // Klokken er tyve minutter over X.
        toggle_word(LAYER_DA_DK_TYVE, 1);
        toggle_word(LAYER_DA_DK_MINUTTER, 1);
        toggle_word(LAYER_DA_DK_OVER, 1);
      } else if (min < 30) { // Klokken er fem minutter i halv X+1.
        toggle_word(LAYER_DA_DK_FEM, 1);
        toggle_word(LAYER_DA_DK_MINUTTER, 1);
        toggle_word(LAYER_DA_DK_I, 1);
        toggle_word(LAYER_DA_DK_HALV, 1);
      } else if (min < 35) { // Klokken er halv X+1.
        toggle_word(LAYER_DA_DK_HALV, 1);
      } else if (min < 40) { // Klokken er fem minutter over halv X+1.
        toggle_word(LAYER_DA_DK_FEM, 1);
        toggle_word(LAYER_DA_DK_MINUTTER, 1);
        toggle_word(LAYER_DA_DK_OVER, 1);
        toggle_word(LAYER_DA_DK_HALV, 1);
        hour++;
      } else if (min < 45) { // Klokken er tyve minutter i X+1.
        toggle_word(LAYER_DA_DK_TYVE, 1);
        toggle_word(LAYER_DA_DK_MINUTTER, 1);
        toggle_word(LAYER_DA_DK_I, 1);
        hour++;
      } else if (min < 50) { // Klokken er kvart i X+1.
        toggle_word(LAYER_DA_DK_KVART, 1);
        toggle_word(LAYER_DA_DK_I, 1);
        hour++;
      } else if (min < 55) { // Klokken er ti minutter i X+1.
        toggle_word(LAYER_DA_DK_TI, 1);
        toggle_word(LAYER_DA_DK_MINUTTER, 1);
        toggle_word(LAYER_DA_DK_I, 1);
        hour++;
      } else { // Klokken er fem minutter i X+1.
        toggle_word(LAYER_DA_DK_FEM, 1);
        toggle_word(LAYER_DA_DK_MINUTTER, 1);
        toggle_word(LAYER_DA_DK_I, 1);
        hour++;
      }
      break;
    case LANG_DE_DE:
      // Turn on the words that are always on
      toggle_word(LAYER_DE_DE_ES, 1);
      toggle_word(LAYER_DE_DE_IST, 1);

      // Turn on the words needed for the current time
      if (min < 5) { // Es ist X uhr.
        toggle_word(LAYER_DE_DE_UHR, 1);
      } else if (min < 10) { // Es ist fünf nach X.
        toggle_word(LAYER_DE_DE_FUENF, 1);
        toggle_word(LAYER_DE_DE_NACH, 1);
      } else if (min < 15) { // Es ist zehn nach X.
        toggle_word(LAYER_DE_DE_ZEHN, 1);
        toggle_word(LAYER_DE_DE_NACH, 1);
      } else if (min < 20) { // Es ist viertel nach X.
        toggle_word(LAYER_DE_DE_VIERTEL, 1);
        toggle_word(LAYER_DE_DE_NACH, 1);
      } else if (min < 25) { // Es ist zwanzig nach X.
        toggle_word(LAYER_DE_DE_ZWANZIG, 1);
        toggle_word(LAYER_DE_DE_NACH, 1);
      } else if (min < 30) { // Es ist fünf vor halb X+1.
        toggle_word(LAYER_DE_DE_FUENF, 1);
        toggle_word(LAYER_DE_DE_VOR, 1);
        toggle_word(LAYER_DE_DE_HALB, 1);
        hour++;
      } else if (min < 35) { // Es ist halb X+1.
        toggle_word(LAYER_DE_DE_HALB, 1);
        hour++;
      } else if (min < 40) { // Es ist fünf nach halb X+1.
        toggle_word(LAYER_DE_DE_FUENF, 1);
        toggle_word(LAYER_DE_DE_NACH, 1);
        toggle_word(LAYER_DE_DE_HALB, 1);
        hour++;
      } else if (min < 45) { // Es ist zwanzig vor X+1.
        toggle_word(LAYER_DE_DE_ZWANZIG, 1);
        toggle_word(LAYER_DE_DE_VOR, 1);
        hour++;
      } else if (min < 50) { // Es ist viertel vor X+1.
        toggle_word(LAYER_DE_DE_VIERTEL, 1);
        toggle_word(LAYER_DE_DE_VOR, 1);
        hour++;
      } else if (min < 55) { // Es ist zehn vor X+1.
        toggle_word(LAYER_DE_DE_ZEHN, 1);
        toggle_word(LAYER_DE_DE_VOR, 1);
        hour++;
      } else { // Es ist fünf vor X+1.
        toggle_word(LAYER_DE_DE_FUENF, 1);
        toggle_word(LAYER_DE_DE_VOR, 1);
        hour++;
      }
      break;
    case LANG_EN_US:
      // Turn on the words that are always on
      toggle_word(LAYER_EN_US_IT, 1);
      toggle_word(LAYER_EN_US_IS, 1);

      // Turn on the words needed for the current time
      if (min < 5) { // It is X o'clock.
        toggle_word(LAYER_EN_US_OCLOCK, 1);
      } else if (min < 10) { // It is five past X.
        toggle_word(LAYER_EN_US_FIVE, 1);
        toggle_word(LAYER_EN_US_PAST, 1);
      } else if (min < 15) { // It is ten past X.
        toggle_word(LAYER_EN_US_TEN, 1);
        toggle_word(LAYER_EN_US_PAST, 1);
      } else if (min < 20) { // It is a quarter past X.
        toggle_word(LAYER_EN_US_A, 1);
        toggle_word(LAYER_EN_US_QUARTER, 1);
        toggle_word(LAYER_EN_US_PAST, 1);
      } else if (min < 25) { // It is twenty past X.
        toggle_word(LAYER_EN_US_TWENTY, 1);
        toggle_word(LAYER_EN_US_PAST, 1);
      } else if (min < 30) { // It is twenty-five past X.
        toggle_word(LAYER_EN_US_TWENTY, 1);
        toggle_word(LAYER_EN_US_FIVE, 1);
        toggle_word(LAYER_EN_US_PAST, 1);
      } else if (min < 35) { // It is half past X.
        toggle_word(LAYER_EN_US_HALF, 1);
        toggle_word(LAYER_EN_US_PAST, 1);
      } else if (min < 40) { // It is twenty-five to X+1.
        toggle_word(LAYER_EN_US_TWENTY, 1);
        toggle_word(LAYER_EN_US_FIVE, 1);
        toggle_word(LAYER_EN_US_TO, 1);
        hour++;
      } else if (min < 45) { // It is twenty to X+1.
        toggle_word(LAYER_EN_US_TWENTY, 1);
        toggle_word(LAYER_EN_US_TO, 1);
        hour++;
      } else if (min < 50) { // It is a quarter to X+1.
        toggle_word(LAYER_EN_US_A, 1);
        toggle_word(LAYER_EN_US_QUARTER, 1);
        toggle_word(LAYER_EN_US_TO, 1);
        hour++;
      } else if (min < 55) { // It is ten to X+1.
        toggle_word(LAYER_EN_US_TEN, 1);
        toggle_word(LAYER_EN_US_TO, 1);
        hour++;
      } else { // It is five to X+1.
        toggle_word(LAYER_EN_US_FIVE, 1);
        toggle_word(LAYER_EN_US_TO, 1);
        hour++;
      }
      break;
    case LANG_ES_ES:
      // Turn on the words needed for the current time
      if (min < 5) {
        // Es la una. Son las X.
      } else if (min < 10) { // Es la una y cinco. Son las X y cinco.
        toggle_word(LAYER_ES_ES_Y, 1);
        toggle_word(LAYER_ES_ES_CINCO, 1);
      } else if (min < 15) { // Es la una y diez. Son las X y diez.
        toggle_word(LAYER_ES_ES_Y, 1);
        toggle_word(LAYER_ES_ES_DIEZ, 1);
      } else if (min < 20) { // Es la una y cuarto. Son las X y cuarto.
        toggle_word(LAYER_ES_ES_Y, 1);
        toggle_word(LAYER_ES_ES_CUARTO, 1);
      } else if (min < 25) { // Es la una y veinte. Son las X y veinte.
        toggle_word(LAYER_ES_ES_Y, 1);
        toggle_word(LAYER_ES_ES_VEINTE, 1);
      } else if (min < 30) { // Es la una y veinticinco. Son las X y veinticinco
        toggle_word(LAYER_ES_ES_Y, 1);
        toggle_word(LAYER_ES_ES_VEINTICINCO, 1);
      } else if (min < 35) { // Es la una y media. Son las X y media.
        toggle_word(LAYER_ES_ES_Y, 1);
        toggle_word(LAYER_ES_ES_MEDIA, 1);
      } else if (min < 40) { // Es la una menos veinticinco. Son las X+1 menos veinticinco.
        toggle_word(LAYER_ES_ES_MENOS, 1);
        toggle_word(LAYER_ES_ES_VEINTICINCO, 1);
        hour++;
      } else if (min < 45) { // Es la una menos veinte. Son las X+1 menos veinte.
        toggle_word(LAYER_ES_ES_MENOS, 1);
        toggle_word(LAYER_ES_ES_VEINTE, 1);
        hour++;
      } else if (min < 50) { // Es la una menos menos cuarto. Son las X+1 menos cuarto.
        toggle_word(LAYER_ES_ES_MENOS, 1);
        toggle_word(LAYER_ES_ES_CUARTO, 1);
        hour++;
      } else if (min < 55) { // Es la una menos diez. Son las X+1 menos diez.
        toggle_word(LAYER_ES_ES_MENOS, 1);
        toggle_word(LAYER_ES_ES_DIEZ, 1);
        hour++;
      } else { // Es la una menos cinco. Son las X+1 menos cinco.
        toggle_word(LAYER_ES_ES_MENOS, 1);
        toggle_word(LAYER_ES_ES_CINCO, 1);
        hour++;
      }
      break;
    case LANG_FR_FR:
      // Turn on the words that are always on
      toggle_word(LAYER_FR_FR_IL, 1);
      toggle_word(LAYER_FR_FR_EST, 1);

      // Turn on the words needed for the current time
      if (min < 5) {
        // Il est X heures.
      } else if (min < 10) { // Il est X heures cinq.
        toggle_word(LAYER_FR_FR_CINQ, 1);
      } else if (min < 15) { // Il est X heures dix.
        toggle_word(LAYER_FR_FR_DIX, 1);
      } else if (min < 20) { // Il est X heures et quart.
        toggle_word(LAYER_FR_FR_ET, 1);
        toggle_word(LAYER_FR_FR_QUART, 1);
      } else if (min < 25) { // Il est X heures vingt.
        toggle_word(LAYER_FR_FR_VINGT, 1);
      } else if (min < 30) { // Il est X heures vingt-cinq.
        toggle_word(LAYER_FR_FR_VINGT, 1);
        toggle_word(LAYER_FR_FR_TRAIT, 1);
        toggle_word(LAYER_FR_FR_CINQ, 1);
      } else if (min < 35) { // Il est X heures et demie.
        toggle_word(LAYER_FR_FR_ET, 1);
        toggle_word(LAYER_FR_FR_DEMIE, 1);
      } else if (min < 40) { // Il est X+1 heures moins vingt-cinq.
        toggle_word(LAYER_FR_FR_MOINS, 1);
        toggle_word(LAYER_FR_FR_VINGT, 1);
        toggle_word(LAYER_FR_FR_TRAIT, 1);
        toggle_word(LAYER_FR_FR_CINQ, 1);
        hour++;
      } else if (min < 45) { // Il est X+1 heures moins vingt.
        toggle_word(LAYER_FR_FR_MOINS, 1);
        toggle_word(LAYER_FR_FR_VINGT, 1);
        hour++;
      } else if (min < 50) { // Il est X+1 heures moins le quart.
        toggle_word(LAYER_FR_FR_MOINS, 1);
        toggle_word(LAYER_FR_FR_LE, 1);
        toggle_word(LAYER_FR_FR_QUART, 1);
        hour++;
      } else if (min < 55) { // Il est X+1 heures moins dix.
        toggle_word(LAYER_FR_FR_MOINS, 1);
        toggle_word(LAYER_FR_FR_DIX, 1);
        hour++;
      } else { // Il est X+1 moins cinq.
        toggle_word(LAYER_FR_FR_MOINS, 1);
        toggle_word(LAYER_FR_FR_CINQ, 1);
        hour++;
      }
      break;
    case LANG_IT_IT:
      // Turn on the words needed for the current time
      if (min < 5) {
        // Sono le X.
      } else if (min < 10) { // Sono le X e cinque.
        toggle_word(LAYER_IT_IT_E, 1);
        toggle_word(LAYER_IT_IT_CINQUE, 1);
      } else if (min < 15) { // Sono le X e dieci.
        toggle_word(LAYER_IT_IT_E, 1);
        toggle_word(LAYER_IT_IT_DIECI, 1);
      } else if (min < 20) { // Sono le X e un quarto.
        toggle_word(LAYER_IT_IT_E, 1);
        toggle_word(LAYER_IT_IT_UN, 1);
        toggle_word(LAYER_IT_IT_QUARTO, 1);
      } else if (min < 25) { // Sono le X e venti.
        toggle_word(LAYER_IT_IT_E, 1);
        toggle_word(LAYER_IT_IT_VENTI, 1);
      } else if (min < 30) { // sono le X e venticinque.
        toggle_word(LAYER_IT_IT_E, 1);
        toggle_word(LAYER_IT_IT_VENTI, 1);
        toggle_word(LAYER_IT_IT_CINQUE, 1);
      } else if (min < 35) { // Sono le X e mezzo.
        toggle_word(LAYER_IT_IT_E, 1);
        toggle_word(LAYER_IT_IT_MEZZO, 1);
      } else if (min < 40) { // Sono le X+1 meno venticinque.
        toggle_word(LAYER_IT_IT_MENO, 1);
        toggle_word(LAYER_IT_IT_VENTI, 1);
        toggle_word(LAYER_IT_IT_CINQUE, 1);
        hour++;
      } else if (min < 45) { // Sono le X+1 meno venti.
        toggle_word(LAYER_IT_IT_MENO, 1);
        toggle_word(LAYER_IT_IT_VENTI, 1);
        hour++;
      } else if (min < 50) { // Sono le X+1 meno un quarto.
        toggle_word(LAYER_IT_IT_MENO, 1);
        toggle_word(LAYER_IT_IT_UN, 1);
        toggle_word(LAYER_IT_IT_QUARTO, 1);
        hour++;
      } else if (min < 55) { // Sono le X+1 meno dieci.
        toggle_word(LAYER_IT_IT_MENO, 1);
        toggle_word(LAYER_IT_IT_DIECI, 1);
        hour++;
      } else { // Sono le X+1 meno cinque.
        toggle_word(LAYER_IT_IT_MENO, 1);
        toggle_word(LAYER_IT_IT_CINQUE, 1);
        hour++;
      }
      break;
    case LANG_NL_NL:
      // Turn on the words that are always on
      toggle_word(LAYER_NL_NL_HET, 1);
      toggle_word(LAYER_NL_NL_IS, 1);

      // Turn on the words needed for the current time
      if (min < 5) { // Het is X uur.
        toggle_word(LAYER_NL_NL_UUR, 1);
      } else if (min < 10) { // Het is vijf over X.
        toggle_word(LAYER_NL_NL_VIJF, 1);
        toggle_word(LAYER_NL_NL_OVER, 1);
      } else if (min < 15) { // Het is tien over X.
        toggle_word(LAYER_NL_NL_TIEN, 1);
        toggle_word(LAYER_NL_NL_OVER, 1);
      } else if (min < 20) { // Het is kwart over X.
        toggle_word(LAYER_NL_NL_KWART, 1);
        toggle_word(LAYER_NL_NL_OVER, 1);
      } else if (min < 25) { // Het is tien voor half X+1.
        toggle_word(LAYER_NL_NL_TIEN, 1);
        toggle_word(LAYER_NL_NL_VOOR, 1);
        toggle_word(LAYER_NL_NL_HALF, 1);
        hour++;
      } else if (min < 30) { // Het is vijf voor half X+1.
        toggle_word(LAYER_NL_NL_VIJF, 1);
        toggle_word(LAYER_NL_NL_VOOR, 1);
        toggle_word(LAYER_NL_NL_HALF, 1);
        hour++;
      } else if (min < 35) { // Het is half X+1.
        toggle_word(LAYER_NL_NL_HALF, 1);
        hour++;
      } else if (min < 40) { // Het is vijf over half X+1.
        toggle_word(LAYER_NL_NL_VIJF, 1);
        toggle_word(LAYER_NL_NL_OVER, 1);
        toggle_word(LAYER_NL_NL_HALF, 1);
        hour++;
      } else if (min < 45) { // Het is tien over half X+1.
        toggle_word(LAYER_NL_NL_TIEN, 1);
        toggle_word(LAYER_NL_NL_OVER, 1);
        toggle_word(LAYER_NL_NL_HALF, 1);
        hour++;
      } else if (min < 50) { // Het is kwart voor X+1.
        toggle_word(LAYER_NL_NL_KWART, 1);
        toggle_word(LAYER_NL_NL_VOOR, 1);
        hour++;
      } else if (min < 55) { // Het is tien voor X+1.
        toggle_word(LAYER_NL_NL_TIEN, 1);
        toggle_word(LAYER_NL_NL_VOOR, 1);
        hour++;
      } else { // Het is vijf voor X+1.
        toggle_word(LAYER_NL_NL_VIJF, 1);
        toggle_word(LAYER_NL_NL_VOOR, 1);
        hour++;
      }
      break;
    case LANG_SV_SE:
      // Turn on the words that are always on
      toggle_word(LAYER_SV_SE_KLOCKAN, 1);
      toggle_word(LAYER_SV_SE_AR, 1);

      // Turn on the words needed for the current time
      if (min < 5) { // Klokken är X.
      } else if (min < 10) { // Klokken är fem over X.
        toggle_word(LAYER_SV_SE_FEM, 1);
        toggle_word(LAYER_SV_SE_OVER, 1);
      } else if (min < 15) { // Klokken är ti over X.
        toggle_word(LAYER_SV_SE_TIO, 1);
        toggle_word(LAYER_SV_SE_OVER, 1);
      } else if (min < 20) { // Klokken är kvart over X.
        toggle_word(LAYER_SV_SE_KVART, 1);
        toggle_word(LAYER_SV_SE_OVER, 1);
      } else if (min < 25) { // Klokken är tyve over X.
        toggle_word(LAYER_SV_SE_TJUGO, 1);
        toggle_word(LAYER_SV_SE_OVER, 1);
      } else if (min < 30) { // Klokken är fem i halv X+1.
        toggle_word(LAYER_SV_SE_FEM, 1);
        toggle_word(LAYER_SV_SE_I, 1);
        toggle_word(LAYER_SV_SE_HALV, 1);
      } else if (min < 35) { // Klokken är halv X+1.
        toggle_word(LAYER_SV_SE_HALV, 1);
      } else if (min < 40) { // Klokken är fem over halv X+1.
        toggle_word(LAYER_SV_SE_FEM, 1);
        toggle_word(LAYER_SV_SE_OVER, 1);
        toggle_word(LAYER_SV_SE_HALV, 1);
        hour++;
      } else if (min < 45) { // Klokken är tyve i X+1.
        toggle_word(LAYER_SV_SE_TJUGO, 1);
        toggle_word(LAYER_SV_SE_I, 1);
        hour++;
      } else if (min < 50) { // Klokken är kvart i X+1.
        toggle_word(LAYER_SV_SE_KVART, 1);
        toggle_word(LAYER_SV_SE_I, 1);
        hour++;
      } else if (min < 55) { // Klokken är ti i X+1.
        toggle_word(LAYER_SV_SE_TIO, 1);
        toggle_word(LAYER_SV_SE_I, 1);
        hour++;
      } else { // Klokken är fem i X+1.
        toggle_word(LAYER_SV_SE_FEM, 1);
        toggle_word(LAYER_SV_SE_I, 1);
        hour++;
      }
      break;
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

  // Special consideration
  if(LANG_DE_DE == language_setting) {
    if (hour == 1 && min >= 5) {
      toggle_word(1, 0);
      toggle_word(13, 1);
    } else {
      toggle_word(13, 0);
    }
  } else if(LANG_ES_ES == language_setting) {
    if (hour == 1) {
      toggle_word(LAYER_ES_ES_ES, 1);
      toggle_word(LAYER_ES_ES_LA, 1);
    } else {
      toggle_word(LAYER_ES_ES_SON, 1);
      toggle_word(LAYER_ES_ES_LAS, 1);
    }
  } else if(LANG_FR_FR == language_setting) {
    if (hour == 1) {
      toggle_word(LAYER_FR_FR_HEURE, 1);
    } else {
      toggle_word(LAYER_FR_FR_HEURES, 1);
    }
  } else if(LANG_IT_IT == language_setting) {
    if (hour == 1) {
      toggle_word(LAYER_IT_IT_E_GRAVE, 1);
    } else {
      toggle_word(LAYER_IT_IT_SONO, 1);
      toggle_word(LAYER_IT_IT_LE, 1);
    }
  }
}

/**
 * Called once per minute to update time display.
 *
 * @param tick_time     The time at which the tick event was triggered
 * @param units_changes Which unit change triggered this tick event
 */
static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
}

/**
 * Draws a box in a corner of the screen to indicate the number of minutes past the five.
 *
 * @param me  Pointer to layer to be rendered
 * @param ctx The destination graphics context to draw into
 */
static void minute_layer_update_callback(Layer * const me, GContext * ctx) {
  const int w = 4; // Width of the box to be drawn
  GRect box = GRect(0, 0, w, w);

  if (minute_num == 0) return; // Nothing to draw
  else if (minute_num == 1) box.origin = GPoint(0, 0); // top-left corner
  else if (minute_num == 2) box.origin = GPoint(144-w, 0); // top-right corner
  else if (minute_num == 3) box.origin = GPoint(144-w, 168-w); // bottom-left corner
  else if (minute_num == 4) box.origin = GPoint(0, 168-w); // bottom-right corner

  //graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, (settings & SETTING_INVERTED) > 0 ? GColorBlack : GColorWhite);
  graphics_fill_rect(ctx, box, 1, GCornersAll);
}

/**
 * Initialize TextLayer for displaying a single word.
 *
 * @param which The index of the word to be displayed by the new layer
 */
static void word_layer_init(int which) {
  const word_t * const w = &words[language_setting][which];

  GRect frame = GRect(
    w->col*FONT_W,
    w->row*FONT_H - 2,
    strlen(w->text_on)*(FONT_W+4),
    FONT_H+8
  );

  text_layers[which] = text_layer_create(frame);
  text_layer_set_text_color(text_layers[which], (settings & SETTING_INVERTED) > 0 ? GColorBlack : GColorWhite);
  text_layer_set_background_color(text_layers[which], GColorClear);
  text_layer_set_font(text_layers[which], font_off);
  Layer *window_layer = window_get_root_layer(window);
  layer_add_child(window_layer, text_layer_get_layer(text_layers[which]));
  toggle_word(which, 0); // all are "off" initially
}

/**
 * Clear the watchface by destroying and recreating all text layers
 *
 */
static void clear_watchface() {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  //  Destroy existing text layers
  for (unsigned i = 0; i < word_count; i++) {
    text_layer_destroy(text_layers[i]);
  }
  layer_destroy(minute_layer);

  // Set background color
  window_set_background_color(window, (settings & SETTING_INVERTED) > 0 ? GColorWhite : GColorBlack);

  // Reset word count for the current language setting
  word_count = sizeof(words[language_setting]) / sizeof(*words[language_setting]);

  // Create new text layers
  for (unsigned i = 0; i < word_count; i++) {
    word_layer_init(i);
  }
  minute_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_set_update_proc(minute_layer, minute_layer_update_callback);
  layer_add_child(window_layer, minute_layer);
}

/**
 * Called when there is a settings sync error.
 *
 * @see https://developer.getpebble.com/2/api-reference/group___app_sync.html#ga144a1a8d8050f8f279b11cfb5d526212
 */
static void settings_sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Settings Sync Error: %d", app_message_error);
}

/**
 * Called when a settings tuple has changed.
 *
 * @todo only update if new_tuple != old_tuple?
 *
 * @see https://developer.getpebble.com/2/api-reference/group___app_sync.html#ga448af36883189f6345cc7d5cd8a3cc29
 */
static void settings_sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch (key) {
    case SETTING_SYNC_KEY_ALL_CAPS:
      if (0 == ((uint8_t) new_tuple->value->uint8)) settings = settings & ~SETTING_ALL_CAPS;
      else settings = settings | SETTING_ALL_CAPS;
      break;
    case SETTING_SYNC_KEY_INVERTED:
      if (0 == ((uint8_t) new_tuple->value->uint8)) settings = settings & ~SETTING_INVERTED;
      else settings = settings | SETTING_INVERTED;
      break;
    case SETTING_SYNC_KEY_LANGUAGE:
      language_setting = (uint8_t) new_tuple->value->uint8;
      break;
  }

  // Redraw watchface
  clear_watchface();
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  update_time(t);
}

/**
 * Called when the window is pushed to the screen when it's not loaded.
 *
 * Create layout.
 *
 * @param window Pointer to Window object
 */
static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Load fonts
  font_on = fonts_load_custom_font(resource_get_handle(FONT_ON));
  font_off = fonts_load_custom_font(resource_get_handle(FONT_OFF));

  // Initialize text layers for displaying words
  memset(&text_layers, 0, sizeof(text_layers));
  for (unsigned i = 0; i < word_count; i++) {
    word_layer_init(i);
  }
  //toggle_word(LAYER_FILLER, 1);

  // Initialize a graphics layer for the minute indicator
  minute_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_set_update_proc(minute_layer, minute_layer_update_callback);
  layer_add_child(window_layer, minute_layer);
}

/**
 * Called when the window is de-initialized.
 *
 * Perform clean-up.
 *
 * @param window Pointer to Window object
 */
static void window_unload(Window *window) {
  for (unsigned i = 0; i < (sizeof(text_layers) / sizeof(*text_layers)); i++) {
    text_layer_destroy(text_layers[i]);
  }
  layer_destroy(minute_layer);
}

/**
 * Initialize the app
 *
 */
static void init(void) {
  settings = 0;
  language_setting = LANG_EN_US;
  word_count = sizeof(words[language_setting]) / sizeof(*words[language_setting]);

  // Initialize window
  window = window_create();
  window_set_background_color(window, (settings & SETTING_INVERTED) > 0 ? GColorWhite : GColorBlack);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(window, true);

  // Update time immediately to avoid flash of "timeless" clock
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  update_time(t);

  // Subscribe to tick timer service to update watchface every minute
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);

  // Load settings and init sync with JS app on phone
  Tuplet initial_settings[] = {
    TupletInteger(SETTING_SYNC_KEY_ALL_CAPS, 0),
    TupletInteger(SETTING_SYNC_KEY_INVERTED, 0),
    TupletInteger(SETTING_SYNC_KEY_LANGUAGE, LANG_EN_US)
  };
  app_sync_init(&settings_sync, settings_sync_buffer, sizeof(settings_sync_buffer), initial_settings, ARRAY_LENGTH(initial_settings),
    settings_sync_tuple_changed_callback, settings_sync_error_callback, NULL
  );
  app_message_open(64, 64);
}

/**
 * De-initialize the app
 *
 */
static void deinit(void) {
  app_sync_deinit(&settings_sync);
  tick_timer_service_unsubscribe();
  window_destroy(window);
}

/**
 * App entry point.
 *
 */
int main(void) {
  init();
  app_event_loop();
  deinit();
}

