## 3.0.0 (2026-01-19)

### BREAKING CHANGES:

  - Migrate from legacy AppSync to @rebble/clay configuration framework
  - Settings configuration now uses Clay JSON format instead of HTML-based settings page
  - Existing user settings will be reset to defaults on first launch

### Features:

  - Modern Clay-based settings UI with improved UX
  - Consistent function naming with `prv_` prefix for static functions
  - Add `prv_update_display_now()` utility function to reduce code duplication

### Documentation:

  - Update README with current project information
  - Update screenshots

## 2.2.4 (2014-04-13)

### Features:

  - Added Norsk (nb_NO) translation

## 2.2.3 (2014-03-31)

### Bugfixes:

  - Refer to next hour when 25 or 30 minutes past the hour for Swedish watch face.

## 2.2.2 (2014-02-07)

### Bugfixes:

  - Refer to next hour when 25 minutes to next hour for French translation.
  - Refer to next hour when two minutes or less to next hour and ±2 Minute Dots option is enabled.

## 2.2.1 (2014-02-04)

### Bugfixes:

  - Refer to next hour when half (or more) past the current hour.

## 2.2.0 (2014-01-27)

### New Features:

  - Added Belgisch-Nederlands (nl_BE) translation
  - Added Português (pt_PT) translation
  - Now storing translation strings and logic in individual resource files, which results in an over 75% decrease in RAM usage and a few kilobytes less in package (PBW file) size.
  - Added "±2 minute dots" setting.

## 2.1.2 (2014-01-20)

### Bugfixes:

  - Replaced ZEHN with TIEN in Dutch(Nederlands) translation.

## 2.1.1 (2014-01-13)

### Bugfixes:

  - Fixed some JS bugs causing improper handling of app settings.

## 2.1.0 (2014-01-09)

### New Features:

  - Configurable via Pebble smartphone app. Options include:
    - Inverted colors - black text on white screen rather than white text on black screen
    - All-caps mode - All letters capitalized
    - Language - One of eight languages can be chosen for the watchface display.

## 2.0.1 (2013-12-31)

### Bugfixes:

  - Fixed overlay issue with "TJUGO" in Swedish/Svenska watchface.

## 2.0.0 (2013-12-30)

Initial release
