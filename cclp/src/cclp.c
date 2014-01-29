/**
 * CCLP - ChronoCode Language Packer
 *
 * Parses language pack files (currently written as C files), and writes them
 * out to binary files ready to be read as a raw Pebble resource file.
 *
 * Very rough and bare bones at the moment, but it gets the job done.
 *
 * @todo Use JSON files to define language packs?
 * @license New BSD License (please see LICENSE file)
 * @repo https://github.com/rexmac/pebble-chronocode
 * @author Rex McConnell <rex@rexmac.com>
 */
#include <stdio.h>
#include <stdint.h>

typedef struct {
  uint8_t x;
  uint8_t y;
  char text_on[13];
  char text_off[13];
} word_t;

//#include "chronocode.da_DK.c"
//#include "chronocode.de_DE.c"
//#include "chronocode.en_US.c"
//#include "chronocode.es_ES.c"
//#include "chronocode.fr_FR.c"
//#include "chronocode.it_IT.c"
//#include "chronocode.nl_BE.c"
//#include "chronocode.nl_NL.c"
#include "chronocode.pt_PT.c"
//#include "chronocode.sv_SE.c"

int main() {
  FILE *fp;
  fp = fopen("packed", "wb");
  uint8_t i;
  uint8_t n;
  const word_t *w;
  for(i = 1; i < 54; i++) {
    w = &words[i];
    n = (w->x << 4) | w->y;
    fwrite(&n, sizeof(n), 1, fp);
    fwrite(w->text_on, sizeof(w->text_on[0]), sizeof(w->text_on)/sizeof(w->text_on[0]), fp);
    fwrite(w->text_off, sizeof(w->text_off[0]), sizeof(w->text_off)/sizeof(w->text_off[0]), fp);
  }
  // Begin writing interval logic - we're now at byte 1431 (0x597)
  for(i = 0; i < 13; i++) {
    fwrite(&intervals[i][0], 1, 1, fp);
    fwrite(&intervals[i][1], 1, 1, fp);
    fwrite(&intervals[i][2], 1, 1, fp);
    fwrite(&intervals[i][3], 1, 1, fp);
    fwrite(&intervals[i][4], 1, 1, fp);
  }
  fclose(fp);
  return 0;
}

