/**
 * Pebble WordSquare - watchface for Pebble watches
 *
 * The watchface fills the screen with a sequence of characters, which, at
 * first glance, may appear to be completely random. However, all of the words
 * required to display the current time appear somewhere in the jumbled mess
 * of characters. The current time (to the nearest five minute interval) is
 * designated by highlighting the words needed to create a sentence that one
 * might use to say the time, e.g. "It is eight o'clock" or "It is a quarter
 * past three". For a more precise time, the watchface displays a small box/dot
 * in one of the corners of the screen with each corner representing one minute
 * before/after the current five minute interval. The top-left corner
 * represents one minute, the top-right corner two minutes, the bottom-right
 * corner three minutes and the bottom-left corner four minutes. Using these
 * corner markers, it is possible to discern the time down to the nearest one
 * minute interval.
 *
 * Example watchfaces in various states:
 *
 *   itoisapebble    IToISapebble    IToISapebble    IToISapebble
 *   atquarterten    atquarterten    atquarterTEN    AtQUARTERten
 *   ktwentynfive    ktwentynfive    ktwentynfive    ktwentynfive
 *   halfopasttof    halfopasttof    halfopastTOf    halfoPASTtof
 *   sevenfivetwo    sevenFIVEtwo    sevenfivetwo    sevenfivetwo
 *   onesixeleven    onesixeleven    oneSIXeleven    onesixeleven
 *   teneninefour    teneninefour    teneninefour    teneninefour
 *   eighttwelves    eighttwelves    eighttwelves    eighttwelves
 *   threehoclock    threexOCLOCK    threehoclock    THREEhoclock
 *       (1)             (2)             (3)             (4)
 *
 *   (1) All words in the "off" state. Never displayed. Only shown here for
 *       reference.
 *   (2) "It is five o'clock"
 *   (3) "It is ten to six"
 *   (4) "It is a quarter past three"
 *
 * The watchface uses a different font for active words which makes discerning
 * the current time even easier.
 *
 * Based on Word clock by Trammell Hudson (https://bitbucket.org/hudson/pebble/src/words),
 * which is based on qlock2.
 *
 * Copyright (c) 2013, Rex McConnell. All rights reserved.
 *
 * @license BSD 3-Clause License (Revised) (@see LICENSE)
 * @author Rex McConnell <rex@rexmac.com>
 */
#include <pebble.h>

#include <wordsquare.h>
#include <wordsquare.@LANG@.h>

static Window *window; /**< The Pebble window */
static TextLayer *text_layers[WORD_COUNT]; /**< Array of text layers for displaying the words */
static GFont font_on;  /**< The font used for words that are inactive or "off" */
static GFont font_off; /**< The font used for words that are active or "on" */
static AppSync settings_sync; /**< Keeps settings in sync between phone and watch */
static uint8_t settings_sync_buffer[64]; /**< Buffer used by settings sync */
static uint8_t settings; /**< Current settings (as bit flags) */

// Settings (bit) flags
enum {
  SETTING_ALL_CAPS = 1,
  SETTING_INVERTED = 2
};

// Language IDs (not being used...yet)
enum language {
  EN_US = 0,
  DA_DK = 1,
  DE_DE = 2,
  ES_ES = 3,
  FR_FR = 4,
  IT_IT = 5,
  NL_NL = 6,
  SV_SE = 7
};

// Settings AppSync keys; correspond to appKeys in appinfo.json
enum {
  SETTING_SYNC_KEY_ALL_CAPS = 0, // TUPLE_CSTRING
  SETTING_SYNC_KEY_INVERTED = 1, // TUPLE_CSTRING
  SETTING_SYNC_KEY_LANGUAGE = 2, // TUPLE_CSTRING
};

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
 * Toggle the on/off state of a single word.
 *
 * @param which The index of the word to toggle
 * @param on    Desired on/off state of the word
 */
static void toggle_word(int which, int on) {
  const word_t * const w = &words[which];

  text_layer_set_text(text_layers[which], on ? w->text_on : ((settings & SETTING_ALL_CAPS) > 0 ? w->text_on : w->text_off));
  text_layer_set_font(text_layers[which], on ? font_on : font_off);
}

/**
 * Initialize TextLayer for displaying a single word.
 *
 * @param which The index of the word to be displayed by the new layer
 */
static void word_layer_init(int which) {
  const word_t * const w = &words[which];

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
 *
 *
 */
static void clear_watchface() {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  for (unsigned i = 0 ; i < WORD_COUNT ; i++) {
    text_layer_destroy(text_layers[i]);
  }
  layer_destroy(minute_layer);
  window_set_background_color(window, (settings & SETTING_INVERTED) > 0 ? GColorWhite : GColorBlack);

  for (unsigned i = 0 ; i < WORD_COUNT ; i++) {
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
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Settings Sync Error: %d", app_message_error);
}

/**
 * Called when a settings tuple has changed.
 *
 * @todo only update if new_tuple != old_tuple?
 *
 * @see https://developer.getpebble.com/2/api-reference/group___app_sync.html#ga448af36883189f6345cc7d5cd8a3cc29
 */
static void settings_sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Settings Sync Tuple Changed!");

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Settings Before: %u", settings);
  switch (key) {
    case SETTING_SYNC_KEY_ALL_CAPS:
      APP_LOG(APP_LOG_LEVEL_DEBUG, "setting_all_caps: %u %u %s", new_tuple->type, new_tuple->length, new_tuple->value->cstring);
      if (strcmp(new_tuple->value->cstring, "0") == 0) settings = settings & ~SETTING_ALL_CAPS;
      else settings = settings | SETTING_ALL_CAPS;
      break;
    case SETTING_SYNC_KEY_INVERTED:
      APP_LOG(APP_LOG_LEVEL_DEBUG, "setting_inverted: %u %u %s", new_tuple->type, new_tuple->length, new_tuple->value->cstring);
      if (strcmp(new_tuple->value->cstring, "0") == 0) settings = settings & ~SETTING_INVERTED;
      else settings = settings | SETTING_INVERTED;
      break;
    case SETTING_SYNC_KEY_LANGUAGE:
      APP_LOG(APP_LOG_LEVEL_DEBUG, "setting_language: %u %u %s", new_tuple->type, new_tuple->length, new_tuple->value->cstring);
      break;
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Settings After: %u", settings);

  // Redraw watchface
  clear_watchface();
  /*layer_mark_dirty(minute_layer);
  for (unsigned i = 0 ; i < WORD_COUNT ; i++) {
    text_layer_set_text_color(text_layers[i], (settings & SETTING_INVERTED) > 0 ? GColorBlack : GColorWhite);
    //layer_mark_dirty(text_layers[which]);
  }*/
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
  for (unsigned i = 0 ; i < WORD_COUNT ; i++) {
    word_layer_init(i);
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Word layer init: %u", sizeof(words[LAYER_FILLER]));
  toggle_word(LAYER_FILLER, 1);

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
  for (unsigned i = 0 ; i < WORD_COUNT ; i++) {
    text_layer_destroy(text_layers[i]);
  }
  layer_destroy(minute_layer);
}

/**
 * Initialize the app
 *
 */
static void init(void) {
  // Load settings and init sync with JS app on phone
  settings = 0;
  Tuplet initial_settings[] = {
    //TupletInteger(SETTING_SYNC_KEY_ALL_CAPS, (uint8_t) 0),
    //TupletInteger(SETTING_SYNC_KEY_INVERTED, (uint8_t) 0),
    //TupletInteger(SETTING_SYNC_KEY_LANGUAGE, (uint8_t) 0)
    TupletCString(SETTING_SYNC_KEY_ALL_CAPS, "0"),
    TupletCString(SETTING_SYNC_KEY_INVERTED, "0"),
    TupletCString(SETTING_SYNC_KEY_LANGUAGE, "0")
  };
  app_sync_init(&settings_sync, settings_sync_buffer, sizeof(settings_sync_buffer), initial_settings, ARRAY_LENGTH(initial_settings),
    settings_sync_tuple_changed_callback, settings_sync_error_callback, NULL
  );
  app_message_open(64, 64);

  // Initialize fonts
  window = window_create();
  window_set_background_color(window, (settings & SETTING_INVERTED) > 0 ? GColorWhite : GColorBlack);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);

  // Update time immediately to avoid flash of "timeless" clock
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  update_time(t);

  // Subscribe to tick timer service to update watchface every minute
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
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

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
