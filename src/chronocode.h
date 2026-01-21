#pragma once
#include <pebble.h>

#define SETTINGS_KEY 42

#define FONT_H 18
#define FONT_W 12
#define FONT_ON  RESOURCE_ID_FONT_SOURCECODEPRO_BLACK_20

#ifdef PBL_COLOR
#define FONT_OFF RESOURCE_ID_FONT_SOURCECODEPRO_BLACK_20
#else
#define FONT_OFF RESOURCE_ID_FONT_SOURCECODEPRO_LIGHT_20
#endif

// A structure containing our settings
typedef struct ChronoCodeSettings {
  bool allCaps;
  bool inverted;
  bool twoMinDots;
  uint8_t language;
} __attribute__((__packed__)) ChronoCodeSettings;

// Settings (bit) flags
enum {
  SETTING_ALL_CAPS     = 1 << 0,
  SETTING_INVERTED     = 1 << 1,
  SETTING_TWO_MIN_DOTS = 1 << 2
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
  LANG_SV_SE = 7,
  LANG_NL_BE = 8,
  LANG_PT_PT = 9,
  LANG_NB_NO = 10
};
