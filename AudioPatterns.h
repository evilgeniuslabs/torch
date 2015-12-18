/*
 * Torch: https://github.com/evilgeniuslabs/torch
 * Copyright (C) 2015 Jason Coon
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

uint16_t analyzerColumns() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  for (uint8_t bandIndex = 0; bandIndex < bandCount; bandIndex++) {
    int levelLeft = levelsLeft[bandIndex];
    int levelRight = levelsRight[bandIndex];

    if (drawPeaks) {
      levelLeft = peaksLeft[bandIndex];
      levelRight = peaksRight[bandIndex];
    }

    CRGB colorLeft = ColorFromPalette(palette, levelLeft / levelsPerHue); // CRGB colorLeft = ColorFromPalette(palette, bandIndex * (256 / bandCount));
//    CRGB colorRight = ColorFromPalette(palette, levelRight / levelsPerHue);

    uint8_t x = bandIndex + bandOffset;
    if (x >= MATRIX_WIDTH)
      x -= MATRIX_WIDTH;

    drawFastVLine(x, (MATRIX_HEIGHT - 1) - levelLeft / levelsPerVerticalPixel, MATRIX_HEIGHT - 1, colorLeft);
//    drawFastVLine(x + bandCount, (MATRIX_HEIGHT - 1) - levelRight / levelsPerVerticalPixel, MATRIX_HEIGHT - 1, colorRight);
  }

  return 1;
}

uint16_t analyzerPixels() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  for (uint8_t bandIndex = 0; bandIndex < bandCount; bandIndex++) {
    int levelLeft = levelsLeft[bandIndex];
//    int levelRight = levelsRight[bandIndex];

    if (drawPeaks) {
      levelLeft = peaksLeft[bandIndex];
//      levelRight = peaksRight[bandIndex];
    }

    CRGB colorLeft = ColorFromPalette(palette, levelLeft / levelsPerHue);
//    CRGB colorRight = ColorFromPalette(palette, levelRight / levelsPerHue);

    uint8_t x = bandIndex + bandOffset;
    if (x >= MATRIX_WIDTH)
      x -= MATRIX_WIDTH;

    leds[XY(x, (MATRIX_HEIGHT - 1) - levelLeft / levelsPerVerticalPixel)] = colorLeft;
//    leds[XY(x + bandCount, (MATRIX_HEIGHT - 1) - levelLeft / levelsPerVerticalPixel)] = colorRight;
  }

  return 0;
}

uint16_t fallingSpectrogram() {
  moveDown();

  for (uint8_t bandIndex = 0; bandIndex < bandCount; bandIndex++) {
    int levelLeft = levelsLeft[bandIndex];
//    int levelRight = levelsRight[bandIndex];

    if (drawPeaks) {
      levelLeft = peaksLeft[bandIndex];
//      levelRight = peaksRight[bandIndex];
    }

    if (levelLeft <= 8) levelLeft = 0;
//    if (levelRight <= 8) levelRight = 0;

    CRGB colorLeft = ColorFromPalette(palette, levelLeft / levelsPerHue);
//    CRGB colorRight = ColorFromPalette(palette, levelRight / levelsPerHue);

    if (currentPaletteIndex < 2) { // invert the first two palettes
      colorLeft = ColorFromPalette(palette, 205 - (levelLeft / 4 - 205));
//      colorRight = ColorFromPalette(palette, 205 - (levelLeft / 4 - 205));
    }
    else {
      colorLeft = ColorFromPalette(palette, levelLeft / 4);
//      colorRight = ColorFromPalette(palette, levelRight / 4);
    }

    uint8_t x = bandIndex + bandOffset;
    if (x >= MATRIX_WIDTH)
      x -= MATRIX_WIDTH;

    leds[XY(x, 0)] = colorLeft;
//    leds[XY(x + bandCount, 0)] = colorRight;
  }

  return 0;
}

uint16_t audioNoise() {
  // generate noise data

  noisespeedx = 0;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 30;
  colorLoop = 0;

  static int lastPeak = 0;

  if(peaksLeft[0] >= lastPeak) {
    noisespeedy = peaksLeft[0] / 32;
  }

  noisespeedz = peaksLeft[0] / 128;

  lastPeak = peaksLeft[0];

  fillnoise8();

  for (int i = 0; i < MATRIX_WIDTH; i++) {
    for (int j = 0; j < MATRIX_HEIGHT; j++) {
      uint8_t index = noise[i][j];
      uint8_t bri =   noise[j][i];

      // brighten up, as the color palette itself often contains the
      // light/dark dynamic range desired
      if ( bri > 127 ) {
        bri = 255;
      } else {
        bri = dim8_raw( bri * 2);
      }

      CRGB color = ColorFromPalette( palette, index, bri);
      uint16_t n = XY(i, j);

      leds[n] = color;
    }
  }

  return 0;
}
