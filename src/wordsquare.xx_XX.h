/**
 * Header file for adding a WordSquare watchface in x/x (xx_XX) 
 *
 */
#define LAYER_FIVE       14
#define LAYER_TEN        15
#define LAYER_QUARTER    16
#define LAYER_TWENTY     17
#define LAYER_TWENTYFIVE 18
#define LAYER_HALF       19
#define LAYER_IT          0
#define LAYER_IS         20
#define LAYER_A          21
#define LAYER_PAST       22
#define LAYER_TO         23
#define LAYER_OCLOCK     24

/**
 * Array of words required to create all the phrases for designating time.
 *
 *      xx_XX
 *   0123456789ab
 * 0 xxxxxxxxxxxx
 * 1 xxxxxxxxxxxx
 * 2 xxxxxxxxxxxx
 * 3 xxxxxxxxxxxx
 * 4 xxxxxxxxxxxx
 * 5 xxxxxxxxxxxx
 * 6 xxxxxxxxxxxx
 * 7 xxxxxxxxxxxx
 * 8 xxxxxxxxxxxx
 *   0123456789ab
 */
static const word_t words[] = {
  // Hours 1-12
  [1]  = { x, y, "", "" },
  [2]  = { x, y, "", "" },
  [3]  = { x, y, "", "" },
  [4]  = { x, y, "", "" },
  [5]  = { x, y, "", "" },
  [6]  = { x, y, "", "" },
  [7]  = { x, y, "", "" },
  [8]  = { x, y, "", "" },
  [9]  = { x, y, "", "" },
  [10] = { x, y, "", "" },
  [11] = { x, y, "", "" },
  [12] = { x, y, "", "" },

  // Minutes before/after the hour
  [LAYER_FIVE]       = { x, y, "", "" },
  [LAYER_TEN]        = { x, y, "", "" },
  [LAYER_QUARTER]    = { x, y, "", "" },
  [LAYER_TWENTY]     = { x, y, "", "" },
  [LAYER_TWENTYFIVE] = { x, y, "", "" },
  [LAYER_HALF]       = { x, y, "", "" },

  // Relative
  [LAYER_IT]     = { x, y, "", "" },
  [LAYER_IS]     = { x, y, "", "" },
  [LAYER_A]      = { x, y, "", "" },
  [LAYER_PAST]   = { x, y, "", "" },
  [LAYER_TO]     = { x, y, "", "" },
  [LAYER_OCLOCK] = { x, y, "", "" },

  // Extra, "random" characters used to fill in empty spaces between words
  [LAYER_FILLER] =
    { x,  y, "", "" },
    { x,  y, "", "" },
    { x,  y, "", "" },
    { x,  y, "", "" },
    { x,  y, "", "" },
    { x,  y, "", "" },
    { x,  y, "", "" },
    { x,  y, "", "" },
    { x,  y, "", "" },
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
  toggle_word(LAYER_IT, 1);
  toggle_word(LAYER_IS, 1);

  // Turn on the words needed for the current time
  if (min < 5) {
  } else if (min < 10) {
  } else if (min < 15) {
  } else if (min < 20) {
  } else if (min < 25) {
  } else if (min < 30) {
  } else if (min < 35) {
  } else if (min < 40) {
    hour++;
  } else if (min < 45) {
    hour++;
  } else if (min < 50) {
    hour++;
  } else if (min < 55) {
    hour++;
  } else {
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

