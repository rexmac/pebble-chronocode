/**
 * ChronoCode watchface for the Pebble Smartwatch
 *
 * @version 3.0.0
 * @license New BSD License (please see LICENSE file)
 * @repo https://github.com/rexmac/pebble-chronocode
 * @author Rex McConnell <rex@rexmac.com>
 */
#include <pebble.h>
#include "chronocode.h"

enum { LAYER_FILLER = 27 };

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
static Layer *minute_layer; /**< The layer onto which is drawn the box/dot representing the minute_num */
static int minute_num; /**< The number of minutes (1-4) since the last five minute interval */
static ChronoCodeSettings settings; /**< Current settings */
static uint8_t flags; /**< Current flags (as bit flags) */
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

// Forward declarations
static void prv_toggle_word(int which, int on);
static void prv_update_display(struct tm *time);
static void prv_update_display_now(void);
static void prv_load_language_from_resource_file(void);
static void prv_word_layer_init(int which);
static void prv_minute_layer_update_callback(Layer * const me, GContext * ctx);
static void prv_handle_minute_tick(struct tm *tick_time, TimeUnits units_changed);

static void prv_default_settings() {
  settings.allCaps = false;
  settings.inverted = false;
  settings.twoMinDots = false;
  settings.language = LANG_EN_US;
}

static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();

  // Read settings from persistent storage, if they exist
  if (persist_exists(SETTINGS_KEY)) {
    persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
  }

  // Convert settings to flags
  flags = (settings.allCaps ? SETTING_ALL_CAPS : 0) |
          (settings.inverted ? SETTING_INVERTED : 0) |
          (settings.twoMinDots ? SETTING_TWO_MIN_DOTS : 0);
}

/**
 * Store settings for seamless rehydration
 */
static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));

  // Update flags from settings
  flags = (settings.allCaps ? SETTING_ALL_CAPS : 0) |
          (settings.inverted ? SETTING_INVERTED : 0) |
          (settings.twoMinDots ? SETTING_TWO_MIN_DOTS : 0);
}

/**
 * Toggle the on/off state of a single word.
 *
 * @param which The index of the word to toggle
 * @param on    Desired on/off state of the word
 */
static void prv_toggle_word(int which, int on) {
  if (which == 0) return;
  const word_t * const w = &mywords[which];

  text_layer_set_text(text_layers[which], on ? w->text_on : ((flags & SETTING_ALL_CAPS) > 0 ? w->text_on : w->text_off));
  text_layer_set_font(text_layers[which], on ? font_on : font_off);

  #ifdef PBL_COLOR
  if ((flags & SETTING_INVERTED) > 0) {
    text_layer_set_text_color(text_layers[which], on ? GColorBlack : GColorLightGray);
  } else {
    text_layer_set_text_color(text_layers[which], on ? GColorWhite : GColorDarkGray);
  }
  #endif
}

/**
 * Update time display.
 *
 * @param time The time to be displayed
 */
static void prv_update_display(struct tm *time) {
  int hour = time->tm_hour;
  const int min = time->tm_min;
  uint8_t interval_idx = (min / 5) + 1;

  // Two-minute dot intervals?
  if ((flags & SETTING_TWO_MIN_DOTS) > 0) {
    interval_idx = (int)((min + 2.5) / 5) + 1;
    if (interval_idx > 12) {
      interval_idx = 1;
      hour++;
    }
  }

  // Turn off all of the words
  for (unsigned i = 0; i < word_count; i++) {
    prv_toggle_word(i, 0);
  }

  // Toggle permanent words
  prv_toggle_word(intervals[0][1], 1);
  prv_toggle_word(intervals[0][2], 1);
  prv_toggle_word(intervals[0][3], 1);
  prv_toggle_word(intervals[0][4], 1);

  // Toggle interval words
  prv_toggle_word(intervals[interval_idx][1], 1);
  prv_toggle_word(intervals[interval_idx][2], 1);
  prv_toggle_word(intervals[interval_idx][3], 1);
  prv_toggle_word(intervals[interval_idx][4], 1);

  // Update the minute box
  minute_num = min % 5;
  layer_mark_dirty(minute_layer);

  // Refer to current hour or next hour?
  if (intervals[interval_idx][0]) {
    if (++hour > 24) hour = 0;
  }

  // Convert from 24-hour to 12-hour time
  if (hour == 0) hour = 12;
  else if (hour > 12) hour -= 12;

  // Turn on the word needed for the current hour, turn off the others
  for (int h = 1; h <= 12; h++) {
    prv_toggle_word(h, h == hour ? 1 : 0);
  }

  // Special circumstances for various languages
  if (LANG_DE_DE == settings.language) {
    if (hour == 1 && min >= 5) {
      prv_toggle_word(1, 0);
      prv_toggle_word(13, 1);
    } else {
      prv_toggle_word(13, 0);
    }
  } else if (LANG_ES_ES == settings.language) {
    if (hour == 1) {
      prv_toggle_word(13, 1);
      prv_toggle_word(14, 1);
    } else {
      prv_toggle_word(15, 1);
      prv_toggle_word(16, 1);
    }
  } else if (LANG_FR_FR == settings.language) {
    if (hour == 1) {
      prv_toggle_word(24, 1);
    } else {
      prv_toggle_word(25, 1);
    }
  } else if (LANG_IT_IT == settings.language) {
    if (hour == 1) {
      prv_toggle_word(13, 1);
    } else {
      prv_toggle_word(14, 1);
      prv_toggle_word(15, 1);
    }
  } else if (LANG_PT_PT == settings.language) {
    if (hour == 1) {
      prv_toggle_word(13, 1);
    } else {
      prv_toggle_word(14, 1);
      if (hour == 2) {
        prv_toggle_word(15, 1); // S
      } else if (hour == 4) {
        prv_toggle_word(16, 1); // O
      } else if (hour == 6) {
        prv_toggle_word(17, 1); // I
      } else if (hour == 7) {
        prv_toggle_word(15, 1); // S
        prv_toggle_word(18, 1); // T
      } else if (hour == 8) {
        prv_toggle_word(16, 1); // O
        prv_toggle_word(17, 1); // I
        prv_toggle_word(18, 1); // T
        prv_toggle_word(19, 1); // O
      } else if (hour == 11) {
        prv_toggle_word(16, 1); // O
      } else if (hour == 12) {
        prv_toggle_word(19, 1); // O
      }
    }

    if (min > 30 && hour > 1) {
      prv_toggle_word(33, 1); // A
      if (hour != 6 && hour != 7) {
        prv_toggle_word(34, 1); // S
      }
    }
  }
}

/**
 * Update time display with current time.
 */
static void prv_update_display_now(void) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  prv_update_display(t);
}

/**
 * Called once per minute to update time display.
 *
 * @param tick_time     The time at which the tick event was triggered
 * @param units_changes Which unit change triggered this tick event
 */
static void prv_handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  prv_update_display(tick_time);
}

/**
 * Draws a box in a corner of the screen to indicate the number of minutes past the five.
 *
 * @param me  Pointer to layer to be rendered
 * @param ctx The destination graphics context to draw into
 */
static void prv_minute_layer_update_callback(Layer * const me, GContext * ctx) {
  if (minute_num == 0) return; // Nothing to draw

  graphics_context_set_stroke_color(ctx, (flags & SETTING_INVERTED) > 0 ? GColorBlack : GColorWhite);
  graphics_context_set_fill_color(ctx, (flags & SETTING_INVERTED) > 0 ? GColorBlack : GColorWhite);

  GRect bounds = layer_get_bounds(me);
  int screen_w = bounds.size.w;
  int screen_h = bounds.size.h;

  if ((flags & SETTING_TWO_MIN_DOTS) > 0) {
    const uint16_t r = 1; // Radius of dot to be drawn
    if (minute_num == 1) {
      graphics_fill_circle(ctx, GPoint(r, screen_h - 2*r), r);
    } else if (minute_num == 2) {
      graphics_fill_circle(ctx, GPoint(r, screen_h - 2*r), r);
      graphics_fill_circle(ctx, GPoint(screen_w - 2*r, screen_h - 2*r), r);
    } else if (minute_num == 3) {
      graphics_draw_circle(ctx, GPoint(screen_w - 2*r, r), r);
      graphics_draw_circle(ctx, GPoint(r, r), r);
    } else if (minute_num == 4) {
      graphics_draw_circle(ctx, GPoint(r, r), r);
    }
  } else {
    const int w = 4; // Width of the box to be drawn
    GRect box = GRect(0, 0, w, w);
    if (minute_num == 1) box.origin = GPoint(0, 0); // top-left corner
    else if (minute_num == 2) box.origin = GPoint(screen_w - w, 0); // top-right corner
    else if (minute_num == 3) box.origin = GPoint(screen_w - w, screen_h - w); // bottom-right corner
    else if (minute_num == 4) box.origin = GPoint(0, screen_h - w); // bottom-left corner
    graphics_fill_rect(ctx, box, 1, GCornersAll);
  }
}

/**
 * Initialize TextLayer for displaying a single word.
 *
 * @param which The index of the word to be displayed by the new layer
 */
static void prv_word_layer_init(int which) {
  const word_t * const w = &mywords[which];

  // 144x168 aplite, basalt, diorite, flint
  // 180x180 chalk
  // 200x228 emery
  // 260x260 gabbro
  GRect frame = GRect(
    w->col * FONT_W,
    w->row * FONT_H - 2,
    strlen(w->text_on) * (FONT_W + 4),
    FONT_H + 8
  );

  text_layers[which] = text_layer_create(frame);
  text_layer_set_text_color(text_layers[which], (flags & SETTING_INVERTED) > 0 ? GColorBlack : GColorWhite);
  text_layer_set_background_color(text_layers[which], GColorClear);
  text_layer_set_font(text_layers[which], font_off);
  Layer *window_layer = window_get_root_layer(window);
  layer_add_child(window_layer, text_layer_get_layer(text_layers[which]));
  prv_toggle_word(which, 0); // all are "off" initially
}

/**
 * Clear the watchface by destroying and recreating all text layers
 *
 */
static void prv_clear_watchface() {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  //  Destroy existing text layers
  for (unsigned i = 0; i < word_count; i++) {
    text_layer_destroy(text_layers[i]);
  }
  layer_destroy(minute_layer);

  // Set background color
  window_set_background_color(window, (flags & SETTING_INVERTED) > 0 ? GColorWhite : GColorBlack);

  // Create new text layers
  for (unsigned i = 0; i < word_count; i++) {
    prv_word_layer_init(i);
  }
  minute_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_set_update_proc(minute_layer, prv_minute_layer_update_callback);
  layer_add_child(window_layer, minute_layer);
}

/**
 * Load language pack from resource file.
 *
 * Each language pack includes all of the words and the positions needed to
 * display the current time in that language.
 */
static void prv_load_language_from_resource_file(void) {
  uint32_t resource_id = RESOURCE_ID_CHRONOCODE_EN_US;

  switch (settings.language) {
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
  for (i = 1; i < word_count; i++) {
    resource_load_byte_range(rh, (i - 1) * 33, buffer, 33);
    mywords[i].row = (buffer[0] & 0b11110000) >> 4;
    mywords[i].col = (buffer[0] & 0b00001111);
    memcpy(mywords[i].text_on, &buffer[1], 16);
    memcpy(mywords[i].text_off, &buffer[17], 16);
  }
  // Load interval logic
  j = (i - 1) * 33;
  for (i = 0; i < 13; i++) {
    resource_load_byte_range(rh, j + (i * 5), buffer, 5);
    intervals[i][0] = buffer[0];
    intervals[i][1] = buffer[1];
    intervals[i][2] = buffer[2];
    intervals[i][3] = buffer[3];
    intervals[i][4] = buffer[4];
  }
}

/**
 * Handle AppMessages
 *
 * @param iter The DictionaryIterator containing the AppMessage
 * @param context The context
 */
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  bool settings_changed = false;

  // All CAPS setting (Clay sends values as int32)
  Tuple *all_caps_tuple = dict_find(iter, MESSAGE_KEY_allCaps);
  if (all_caps_tuple) {
    settings.allCaps = all_caps_tuple->value->int32 == 1;
    settings_changed = true;
  }

  // Inverted setting
  Tuple *inverted_tuple = dict_find(iter, MESSAGE_KEY_inverted);
  if (inverted_tuple) {
    settings.inverted = inverted_tuple->value->int32 == 1;
    settings_changed = true;
  }

  // Two-minute dots setting
  Tuple *two_min_dots_tuple = dict_find(iter, MESSAGE_KEY_twoMinDots);
  if (two_min_dots_tuple) {
    settings.twoMinDots = two_min_dots_tuple->value->int32 == 1;
    settings_changed = true;
  }

  // Language setting (Clay sends select values as int32)
  Tuple *language_tuple = dict_find(iter, MESSAGE_KEY_language);
  if (language_tuple) {
    uint8_t new_lang = (uint8_t)language_tuple->value->int32;
    if (new_lang != settings.language) {
      settings.language = new_lang;
      prv_load_language_from_resource_file();
      settings_changed = true;
    }
  }

  if (settings_changed) {
    // Save the new settings
    prv_save_settings();

    // Redraw watchface
    prv_clear_watchface();
    prv_update_display_now();
  }
}

/**
 * Called when the window is pushed to the screen when it's not loaded.
 *
 * Create layout.
 *
 * @param window Pointer to Window object
 */
static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Load fonts
  font_on = fonts_load_custom_font(resource_get_handle(FONT_ON));
  font_off = fonts_load_custom_font(resource_get_handle(FONT_OFF));

  // Load language from resource file
  prv_load_language_from_resource_file();

  // Initialize text layers for displaying words
  memset(&text_layers, 0, sizeof(text_layers));
  for (unsigned i = 0; i < word_count; i++) {
    prv_word_layer_init(i);
  }

  // Initialize a graphics layer for the minute indicator
  minute_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_set_update_proc(minute_layer, prv_minute_layer_update_callback);
  layer_add_child(window_layer, minute_layer);
}

/**
 * Called when the window is de-initialized.
 *
 * Perform clean-up.
 *
 * @param window Pointer to Window object
 */
static void prv_window_unload(Window *window) {
  for (unsigned i = 0; i < (sizeof(text_layers) / sizeof(*text_layers)); i++) {
    text_layer_destroy(text_layers[i]);
  }
  layer_destroy(minute_layer);
  fonts_unload_custom_font(font_on);
  fonts_unload_custom_font(font_off);
}

/**
 * Initialize the app
 *
 */
static void prv_init(void) {
  prv_load_settings();

  // Listen for AppMessages
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);

  // Initialize window
  window = window_create();
  window_set_background_color(window, (flags & SETTING_INVERTED) > 0 ? GColorWhite : GColorBlack);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload
  });
  window_stack_push(window, true);

  // Update time immediately to avoid flash of "timeless" clock
  prv_update_display_now();

  // Subscribe to tick timer service to update watchface every minute
  tick_timer_service_subscribe(MINUTE_UNIT, prv_handle_minute_tick);
}

/**
 * De-initialize the app
 *
 */
static void prv_deinit(void) {
  prv_save_settings();
  tick_timer_service_unsubscribe();
  if (window) {
    window_destroy(window);
  }
}

/**
 * App entry point.
 *
 */
int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
