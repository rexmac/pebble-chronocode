/**
 * Common header file shared by all languages.
 *
 */
#define FONT_H 18
#define FONT_W 12
#define FONT_ON  RESOURCE_ID_FONT_SOURCECODEPRO_BLACK_20
#define FONT_OFF RESOURCE_ID_FONT_SOURCECODEPRO_LIGHT_20

#define LAYER_FILLER 27

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

static void toggle_word(int which, int on);

static Layer *minute_layer; /**< The layer onto which is drawn the box/dot representing the minute_num */
static int minute_num; /**< The number of minutes (1-4) since the last five minute interval */

