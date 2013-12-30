# pebble-wordsquare

**NOTE:** Requires Pebble Firmware v2.0 or greater.

Watchface app for the Pebble Smartwatch that fills the screen with a sequence
of characters, which, at first glance, may appear to be completely random.
However, all of the words required to display the current time appear
somewhere in the jumbled mess of characters. The current time (to the nearest
five minute interval) is designated by highlighting the words needed to create
a sentence that one might use to say the time, e.g. "It is eight o'clock" or
"It is a quarter past three". For a more precise time, the watchface displays a
small box/dot in one of the corners of the screen with each corner representing
one minute before/after the current five minute interval. The top-left corner
represents one minute, the top-right corner two minutes, the bottom-right
corner three minutes and the bottom-left corner four minutes. Using these
corner markers, it is possible to discern the time down to the nearest one
minute interval.

Example watchfaces in various states:

[![Screenshot showing all words in the inactive state](http://rexmac.com/projects/pebble/pebble-wordsquare-sample-1.png)](http://rexmac.com/projects/pebble/pebble-wordsquare-sample-1.png)
[![Screenshot showing display state for 5:00](http://rexmac.com/projects/pebble/pebble-wordsquare-sample-2.png)](http://rexmac.com/projects/pebble/pebble-wordsquare-sample-2.png)
[![Screenshot showing display state for 5:50](http://rexmac.com/projects/pebble/pebble-wordsquare-sample-3.png)](http://rexmac.com/projects/pebble/pebble-wordsquare-sample-3.png)
[![Screenshot showing display state for 3:15](http://rexmac.com/projects/pebble/pebble-wordsquare-sample-4.png)](http://rexmac.com/projects/pebble/pebble-wordsquare-sample-4.png)

### [Download](http://github.com/rexmac/pebble-wordsquare/releases)

## Install

To install this watchface onto your Pebble device using your phone's browser, go to the [downloads page](http://github.com/rexmac/pebble-wordsquare/releases) and click the green `pebble-wordsquare.xx_XX.pbw` button (where `xx_XX` is the language of the watchface that you would like). Your phone should download and automatically install the file into your phone's Pebble app (or it may prompt you for which app to use to open the file, in which case you should select the Pebble app). You can then use the Pebble app on your phone to install the watchface to your Pebble watch.

## Build

To build this watchface from source. Follow these steps:

1. Clone this repository in an appropriate directory. For example:

    `$ git clone https://github.com/rexmac/pebble-wordsquare.git`

2. Build the project:

    `$ make [lang=xx_XX]`

    By default, the app will be built with the American English typeface (`en_US`). By using the optional `lang` option when calling `make`, it is possible it build the watchface with a different language/locale. Supported languages include:
      * Dansk (da_DK)
      * Deutsch (de_DE)
      * English (en_US)
      * Español (es_ES)
      * Français (fr_FR)
      * Italiano (it_IT)
      * Nederlands (nl_NL)
      * Русский язык (ru_RU) - work-in-progress
      * Svenska (sv_SE)

3. Install the compiled project to your Pebble:

    `$ pebble install --phone [your_phone_ip_here]`

For more information on building and installing Pebble apps from source, please see the official [Getting Started](https://developer.getpebble.com/2/getting-started/) guide.

## Bugs, Suggestions, Comments

Please use the [Github issue system](https://github.com/rexmac/pebble-wordsquare/issues) to report bugs, request new features, or ask questions.

## Credits

Based on and inspired by:

* Wordsquare by [Hudson](https://bitbucket.org/hudson/pebble/src/words)
* wordsquare_ES by [knopfler19](http://forums.getpebble.com/profile/10546/knopfler19)
* WordSquare-fr by ocuser01

Fonts used:

* SourceCodePro Black
* SourceCodePro Light

Both fonts are members of the Source font family from [Adobe](http://www.adobe.com/) and are licensed under the SIL Open Font License, Version 1.1. Please see [`resources/fonts/LICENSE.txt`](https://github.com/rexmac/pebble-wordsquare/blob/master/resources/fonts/LICENSE.txt) for more information.

Icons:

* Menu icon - as far as I know, this icon was created by [Trammell Hudson](https://bitbucket.org/hudson/pebble/src/words)

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

