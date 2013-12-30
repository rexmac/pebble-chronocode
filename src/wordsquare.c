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

  graphics_context_set_fill_color(ctx, GColorWhite);
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

  text_layer_set_text(text_layers[which], on ? w->text_on : w->text_off);
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
  text_layer_set_text_color(text_layers[which], GColorWhite);
  text_layer_set_background_color(text_layers[which], GColorClear);
  text_layer_set_font(text_layers[which], font_off);
  Layer *window_layer = window_get_root_layer(window);
  layer_add_child(window_layer, text_layer_get_layer(text_layers[which]));
  toggle_word(which, 0); // all are "off" initially
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

  // Create a graphics layer for the entire background
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

static void init(void) {
  // Settings
  /*if (persist_exists(SETTINGS_KEY)) {
    persist_read_data(SETTINGS_KEY, sizeof(settings), &settings);
  } else {
    settings = DEFAULT_SETTINGS;
  }*/

  // Initialize fonts
  window = window_create();
  window_set_background_color(window, GColorBlack);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);

  // Update time immediately to avoid flash of "timeless" clock
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  update_time(t);

  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}

static void deinit(void) {
  tick_timer_service_unsubscribe();
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
