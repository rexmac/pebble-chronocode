/**
 * ChronoCode watchface for the Pebble Smartwatch
 *
 * @version 2.2.4
 * @license New BSD License (please see LICENSE file)
 * @repo https://github.com/rexmac/pebble-chronocode
 * @author Rex McConnell <rex@rexmac.com>
 */
#include <pebble.h>

#define STORAGE_SETTINGS_KEY 42

#define FONT_H 18
#define FONT_W 12
#define FONT_ON  RESOURCE_ID_FONT_SOURCECODEPRO_BLACK_20
#define FONT_OFF RESOURCE_ID_FONT_SOURCECODEPRO_LIGHT_20

enum { LAYER_FILLER = 27 };

// Settings (bit) flags
enum {
  SETTING_ALL_CAPS     = 1 << 0,
  SETTING_INVERTED     = 1 << 1,
  SETTING_TWO_MIN_DOTS = 1 << 2
};

// Settings AppSync keys; correspond to appKeys in appinfo.json
enum {
  SETTING_SYNC_KEY_ALL_CAPS     = 0,
  SETTING_SYNC_KEY_INVERTED     = 1,
  SETTING_SYNC_KEY_LANGUAGE     = 2,
  SETTING_SYNC_KEY_TWO_MIN_DOTS = 3,
};

// Settings Store Object used to persist settings
typedef struct SettingsStoreObject {
  uint8_t flags;
  uint8_t language;
} __attribute__((__packed__)) SettingsStoreObject;

// Language IDs
enum language_id {
  LANG_EN_US = 0,
  LANG_DA_DK = 1,
  LANG_DE_DE = 2,
  LANG_ES_ES = 3,
  LANG_FR_FR = 4,
  LANG_IT_IT = 5,
  LANG_NL_NL = 6,
  LANG_SV_SE = 7,
  LANG_NL_BE = 8,
  LANG_PT_PT = 9,
  LANG_NB_NO = 10
};

/**
 * Attributes of a single word.
 *
 */
typedef struct {
  uint8_t row; /**< The row coordinate of the word's first letter */
  uint8_t col; /**< The column coordinate of the word's first letter */
  char text_on[16];  /**< The string to display when the word is "on" */
  char text_off[16]; /**< The string to display when the word is "off" */
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
static const uint8_t word_count = 54;

/**
 * Array of word_t structs required to create all the phrases for displaying the time.
 *
 */
static word_t mywords[54];

/**
 * Interval logic array
 *
 * Twelve five-minute intervals (e.g., 0-4, 5-9, 10-14, etc.).
 * Index 0 is for indicating the "always on" or permanent words (e.g., "It is").
 * Each index after that is for indicating the enabled words for successive
 * five-minute intervals, e.g. index 1 is fo 0-4, index 2 is for 5-9, etc.
 *
 * Each interval has a five element array. The first element (index 0) holds
 * the value to be added to the current hour, usually (always?) 0 or 1. This
 * value is used for many languages which refer to the next hour when it is
 * more than 30 minutes past the current hour.
 *
 * The next four elements of an interval array are for indicating up to four
 * words to be enabled to display the current time.
 *
 */
static uint8_t intervals[13][5];

/**
 * Store settings for seamless restauration
 */
static void store_settings(){
  SettingsStoreObject unstored_settings;
  unstored_settings.flags = settings;
  unstored_settings.language = language_setting;
  persist_write_data(STORAGE_SETTINGS_KEY, &unstored_settings, sizeof(unstored_settings));
}

/**
 * Toggle the on/off state of a single word.
 *
 * @param which The index of the word to toggle
 * @param on    Desired on/off state of the word
 */
static void toggle_word(int which, int on) {
  if(which == 0) return;
  const word_t * const w = &mywords[which];

  text_layer_set_text(text_layers[which], on ? w->text_on : ((settings & SETTING_ALL_CAPS) > 0 ? w->text_on : w->text_off));
  text_layer_set_font(text_layers[which], on ? font_on : font_off);
}

/**
 * Update time display.
 *
 * @param time The time to be disaplyed
 */
static void update_time(struct tm *time) {
  int hour = time->tm_hour;
  const int min = time->tm_min;
  uint8_t i = (min / 5) + 1;

  // Two-minute dot intervals?
  if ((settings & SETTING_TWO_MIN_DOTS) > 0) {
    i = ((min + 2.5) / 5) + 1;
    if (i > 12) {
      i = 1;
      hour++;
    }
  }

  // Turn off all of the words
  for (unsigned i = 0; i < word_count; i++) {
    toggle_word(i, 0);
  }

  // Toggle permanent words
  toggle_word(intervals[0][1], 1);
  toggle_word(intervals[0][2], 1);
  toggle_word(intervals[0][3], 1);
  toggle_word(intervals[0][4], 1);

  // Toggle interval words
  toggle_word(intervals[i][1], 1);
  toggle_word(intervals[i][2], 1);
  toggle_word(intervals[i][3], 1);
  toggle_word(intervals[i][4], 1);

  // Update the minute box
  minute_num = min % 5;
  layer_mark_dirty(minute_layer);

  // Refer to current hour or next hour?
  if(intervals[i][0]) {
    if(++hour > 24) hour = 0;
  }

  // Convert from 24-hour to 12-hour time
  if (hour == 0) hour = 12;
  else if (hour > 12) hour -= 12;

  // Turn on the word needed for the curret hour, turn off the others
  for (int i = 1; i <= 12; i++) {
    toggle_word(i, i == hour ? 1 : 0);
  }

  // Special circumstances
  if(LANG_DE_DE == language_setting) {
    if (hour == 1 && min >= 5) {
      toggle_word(1, 0);
      toggle_word(13, 1);
    } else {
      toggle_word(13, 0);
    }
  } else if(LANG_ES_ES == language_setting) {
    if (hour == 1) {
      toggle_word(13, 1);
      toggle_word(14, 1);
    } else {
      toggle_word(15, 1);
      toggle_word(16, 1);
    }
  } else if(LANG_FR_FR == language_setting) {
    if (hour == 1) {
      toggle_word(24, 1);
    } else {
      toggle_word(25, 1);
    }
  } else if(LANG_IT_IT == language_setting) {
    if (hour == 1) {
      toggle_word(13, 1);
    } else {
      toggle_word(14, 1);
      toggle_word(15, 1);
    }
  } else if(LANG_PT_PT == language_setting) {
    if (hour == 1) {
      toggle_word(13, 1);
    } else {
      toggle_word(14, 1);
      if (hour == 2) {
        toggle_word(15, 1); // S
      } else if (hour == 4) {
        toggle_word(16, 1); // O
      } else if (hour == 6) {
        toggle_word(17, 1); // I
      } else if (hour == 7) {
        toggle_word(15, 1); // S
        toggle_word(18, 1); // T
      } else if (hour == 8) {
        toggle_word(16, 1); // O
        toggle_word(17, 1); // I
        toggle_word(18, 1); // T
        toggle_word(19, 1); // O
      } else if (hour == 11) {
        toggle_word(16, 1); // O
      } else if (hour == 12) {
        toggle_word(19, 1); // O
      }
    }

    if (min > 30 && hour > 1) {
      toggle_word(33, 1); // A
      if (hour != 6 && hour != 7) {
        toggle_word(34, 1); // S
      }
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
  if (minute_num == 0) return; // Nothing to draw

  graphics_context_set_stroke_color(ctx, (settings & SETTING_INVERTED) > 0 ? GColorBlack : GColorWhite);
  graphics_context_set_fill_color(ctx, (settings & SETTING_INVERTED) > 0 ? GColorBlack : GColorWhite);

  if ((settings & SETTING_TWO_MIN_DOTS) > 0) {
    const uint16_t r = 1; // Radius of dot to be drawn
    if (minute_num == 1) {
      graphics_fill_circle(ctx, GPoint(r, 168-2*r), r);
    } else if (minute_num == 2) {
      graphics_fill_circle(ctx, GPoint(r, 168-2*r), r);
      graphics_fill_circle(ctx, GPoint(144-2*r, 168-2*r), r);
    } else if (minute_num == 3) {
      graphics_draw_circle(ctx, GPoint(144-2*r, r), r);
      graphics_draw_circle(ctx, GPoint(r, r), r);
    } else if (minute_num == 4) {
      graphics_draw_circle(ctx, GPoint(r, r), r);
    }
  } else {
    const int w = 4; // Width of the box to be drawn
    GRect box = GRect(0, 0, w, w);
    if (minute_num == 1) box.origin = GPoint(0, 0); // top-left corner
    else if (minute_num == 2) box.origin = GPoint(144-w, 0); // top-right corner
    else if (minute_num == 3) box.origin = GPoint(144-w, 168-w); // bottom-right corner
    else if (minute_num == 4) box.origin = GPoint(0, 168-w); // bottom-left corner
    graphics_fill_rect(ctx, box, 1, GCornersAll);
  }
}

/**
 * Initialize TextLayer for displaying a single word.
 *
 * @param which The index of the word to be displayed by the new layer
 */
static void word_layer_init(int which) {
  //const word_t * const w = &words[language_setting][which];
  const word_t * const w = &mywords[which];

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

  // Create new text layers
  for (unsigned i = 0; i < word_count; i++) {
    word_layer_init(i);
  }
  minute_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_set_update_proc(minute_layer, minute_layer_update_callback);
  layer_add_child(window_layer, minute_layer);
}

/**
 * Load language pack from resource file.
 *
 * Each language pack includes all of the words and the positions needed to
 * display the current time in that language.
 */
static void load_language_from_resource_file() {
  uint32_t resource_id = RESOURCE_ID_CHRONOCODE_EN_US;

  switch(language_setting) {
    case LANG_DA_DK: resource_id = RESOURCE_ID_CHRONOCODE_DA_DK; break;
    case LANG_DE_DE: resource_id = RESOURCE_ID_CHRONOCODE_DE_DE; break;
    case LANG_EN_US: resource_id = RESOURCE_ID_CHRONOCODE_EN_US; break;
    case LANG_ES_ES: resource_id = RESOURCE_ID_CHRONOCODE_ES_ES; break;
    case LANG_FR_FR: resource_id = RESOURCE_ID_CHRONOCODE_FR_FR; break;
    case LANG_IT_IT: resource_id = RESOURCE_ID_CHRONOCODE_IT_IT; break;
    case LANG_NB_NO: resource_id = RESOURCE_ID_CHRONOCODE_NB_NO; break;
    case LANG_NL_BE: resource_id = RESOURCE_ID_CHRONOCODE_NL_BE; break;
    case LANG_NL_NL: resource_id = RESOURCE_ID_CHRONOCODE_NL_NL; break;
    case LANG_PT_PT: resource_id = RESOURCE_ID_CHRONOCODE_PT_PT; break;
    case LANG_SV_SE: resource_id = RESOURCE_ID_CHRONOCODE_SV_SE; break;
  }

  // Load language file
  uint8_t i;
  uint16_t j;
  uint8_t buffer[34];
  ResHandle rh = resource_get_handle(resource_id);
  for(i = 1; i < word_count; i++) {
    resource_load_byte_range(rh, (i-1)*33, buffer, 33);
    mywords[i].row = (buffer[0] & 0b11110000) >> 4;
    mywords[i].col = (buffer[0] & 0b00001111);
    memcpy(mywords[i].text_on, &buffer[1], 16);
    memcpy(mywords[i].text_off, &buffer[17], 16);
  }
  // Load interval logic
  j = (i-1)*33;
  for(i = 0; i < 13; i++) {
    resource_load_byte_range(rh, j+(i*5), buffer, 5);
    intervals[i][0] = buffer[0];
    intervals[i][1] = buffer[1];
    intervals[i][2] = buffer[2];
    intervals[i][3] = buffer[3];
    intervals[i][4] = buffer[4];
  }
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
  uint8_t new = 0;

  switch (key) {
    case SETTING_SYNC_KEY_ALL_CAPS:
      if (0 == ((uint8_t) new_tuple->value->uint8)) settings = settings & ~SETTING_ALL_CAPS;
      else settings = settings | SETTING_ALL_CAPS;
      break;
    case SETTING_SYNC_KEY_INVERTED:
      if (0 == ((uint8_t) new_tuple->value->uint8)) settings = settings & ~SETTING_INVERTED;
      else settings = settings | SETTING_INVERTED;
      break;
    case SETTING_SYNC_KEY_TWO_MIN_DOTS:
      if (0 == ((uint8_t) new_tuple->value->uint8)) settings = settings & ~SETTING_TWO_MIN_DOTS;
      else settings = settings | SETTING_TWO_MIN_DOTS;
      break;
    case SETTING_SYNC_KEY_LANGUAGE:
      new = (uint8_t) new_tuple->value->uint8;
      if(new != language_setting) {
        language_setting = new;
        load_language_from_resource_file();
      }
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

  // Load language from resource file
  load_language_from_resource_file();

  // Initialize text layers for displaying words
  memset(&text_layers, 0, sizeof(text_layers));
  for (unsigned i = 0; i < word_count; i++) {
    word_layer_init(i);
  }

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
  // Try to read stored settings.
  SettingsStoreObject stored_settings;
  
  if (persist_exists(STORAGE_SETTINGS_KEY)) {
    persist_read_data(STORAGE_SETTINGS_KEY, &stored_settings, sizeof(stored_settings));
    settings = stored_settings.flags;
    language_setting = stored_settings.language;
  } else {
    settings = 0;
    language_setting = LANG_EN_US;
  }
 
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
    TupletInteger(SETTING_SYNC_KEY_ALL_CAPS, (settings & SETTING_ALL_CAPS)),
    TupletInteger(SETTING_SYNC_KEY_INVERTED, (settings & SETTING_INVERTED)),
    TupletInteger(SETTING_SYNC_KEY_LANGUAGE, language_setting),
    TupletInteger(SETTING_SYNC_KEY_TWO_MIN_DOTS, (settings & SETTING_TWO_MIN_DOTS))
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
  store_settings();
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

