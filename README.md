# pebble-chronocode

**NOTE:** Requires Pebble Firmware v2.0 or greater.

## Description

ChronoCode (formerly known as WordSquare) displays the current time by emphasizing words "hidden" among a jumble of seemingly random letters. The time is displayed in sentence form, e.g. "It is five o'clock", "It is half past three", "It is a quarter to nine", etc. The watchface is configurable via the Pebble app on your smartphone. Options include inverted colors, all-caps mode, and 8 different languages.

### Extended Description

ChronoCode is a watchface app for the Pebble Smartwatch that fills the screen with a sequence of characters, which, at first glance, may appear to be completely random. However, all of the words required to display the current time appear somewhere in the jumbled mess of characters. The current time (to the nearest five minute interval) is designated by emphasizing the words needed to create a sentence that one might use to say the time, e.g. "It is eight o'clock" or "It is a quarter past three". For a more precise time, the watchface displays a small box/dot in one of the corners of the screen with each corner representing one minute before/after the current five minute interval. The top-left corner represents one minute, the top-right corner two minutes, the bottom-right corner three minutes and the bottom-left corner four minutes. Using these corner markers, it is possible to discern the time down to the nearest one minute interval.

The app is configurable via the Pebble smartphone app. See Options section below for more details.

Example watchfaces in various states:

[![Screenshot showing normal, "first run" state (time is 5:50)](https://s3.amazonaws.com/pebble.rexmac.com/chronocode/screenshot1.png)](https://s3.amazonaws.com/pebble.rexmac.com/chronocode/screenshot1.png)&nbsp;
[![Screenshot showing German language, inverted colors (time is 11:10)](https://s3.amazonaws.com/pebble.rexmac.com/chronocode/screenshot2.png)](https://s3.amazonaws.com/pebble.rexmac.com/chronocode/screenshot2.png)&nbsp;
[![Screenshot showing Swedish language, normal colors (time is 4:45)](https://s3.amazonaws.com/pebble.rexmac.com/chronocode/screenshot3.png)](https://s3.amazonaws.com/pebble.rexmac.com/chronocode/screenshot3.png)&nbsp;
[![Screenshot showing Spanish language, inverted colors (time is 4:45)](https://s3.amazonaws.com/pebble.rexmac.com/chronocode/screenshot4.png)](https://s3.amazonaws.com/pebble.rexmac.com/chronocode/screenshot4.png)

### [Download](http://github.com/rexmac/pebble-chronocode/releases)

## Install

To install this watchface onto your Pebble device using your phone's browser, go to the [downloads page](http://github.com/rexmac/pebble-chronocode/releases) and click the green `pebble-chronocode.pbw` button. Your phone should download and automatically install the file into your phone's Pebble app (or it may prompt you for which app to use to open the file, in which case you should select the Pebble app). You can then use the Pebble app on your phone to install the watchface to your Pebble watch.

## Use

The current time is displayed (to the most recent five-minute interval) in sentence form. Four dots are shown in the corners of the screen to represent 1 (top-left), 2 (top-right), 3 (bottom-left), and 4 (bottom-right) minutes past the five-minute inerval.

### Options

* All Caps
    Toggles "ALL CAPS" mode in which all letters are displayed in their capitalized (upper-case) state.

* Inverted
    Toggles inverted mode in which the colors of the display are inverted, i.e. black becomes white and white becomes black.

* ±2 Minute Dots
    Toggles "±2 minute dots" mode in which the watch changes behavior to display the nearest five-minute interval as opposed to the most recent five-minute interval. For example, 4:02 would still be displayed as "It is two o'clock," but 4:03 would be displayed as "It is five past two" as 4:03 is nearer to 4:05 than 4:00. In this mode, the four dots shown in the corners of the screen represent -1 (top-left), -2 (top-right), +1 (bottom-left), and +2 (bottom-right) minutes before or after the current five-minute interval. The earlier example of 4:02 would have the bottom-right dot displayed, while 4:03 would have the top-right dot displayed. This mode is a bit confusing at first if the user is accustomed to the "normal" display, but it is actually a more accurate display of the current time.

* Language
    One of eight languages can be chosen for the watchface display. Available languages include:
      - English (en_US)
      - Belgisch-Nederlands (nl_BE)
      - Dansk (da_DK)
      - Deutsch (de_DE)
      - Español (es_ES)
      - Français (fr_FR)
      - Italiano (it_IT)
      - Nederlands (nl_NL)
      - Português (pt_PT)
      - Svenska (sv_SE)


## Build

To build this watchface from source. Follow these steps:

1. Clone this repository in an appropriate directory. For example:

    `$ git clone https://github.com/rexmac/pebble-chronocode.git`

2. Build the project:

    `$ pebble build`

3. Install the compiled project to your Pebble:

    `$ pebble install --phone [your_phone_ip_here]`

For more information on building and installing Pebble apps from source, please see the official [Getting Started](https://developer.getpebble.com/2/getting-started/) guide.

## Bugs, Suggestions, Comments

Please use the [Github issue system](https://github.com/rexmac/pebble-chronocode/issues) to report bugs, request new features, or ask questions.

## Credits

Based on and inspired by:

* Wordsquare by [Hudson](https://bitbucket.org/hudson/pebble)
* wordsquare DE by [Robert Gies](http://forums.getpebble.com/discussion/4724/watch-face-wordquare-german)
* wordsquare_ES by [knopfler19](http://forums.getpebble.com/profile/10546/knopfler19)
* WordSquare-fr by ocuser01

Fonts used:

* SourceCodePro Black
* SourceCodePro Light

Both fonts are members of the Source font family from [Adobe](http://www.adobe.com/) and are licensed under the SIL Open Font License, Version 1.1. Please see [`resources/fonts/LICENSE.txt`](https://github.com/rexmac/pebble-chronocode/blob/master/resources/fonts/LICENSE.txt) for more information.

Icons:

* Menu icon - as far as I know, this icon was created by [Trammell Hudson](https://bitbucket.org/hudson/pebble/src/words)

## Changelog

Please see the [`CHANGELOG.md`](https://github.com/rexmac/pebble-chronocode/blob/master/CHANGELOG.md) file.

## License

Copyright (c) 2013, Rex McConnell. All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

* Neither the name of the {organization} nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

