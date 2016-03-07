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
    CRGB colorRight = ColorFromPalette(palette, levelRight / levelsPerHue);

    uint8_t x = bandIndex + bandOffset;
    if (x >= MATRIX_WIDTH)
      x -= MATRIX_WIDTH;

    drawFastVLine(x, (MATRIX_HEIGHT - 1) - levelLeft / levelsPerVerticalPixel, MATRIX_HEIGHT - 1, colorLeft);
    drawFastVLine(x + bandCount, (MATRIX_HEIGHT - 1) - levelRight / levelsPerVerticalPixel, MATRIX_HEIGHT - 1, colorRight);
  }

  return 1;
}

uint16_t analyzerColumnsSolid() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  for (uint8_t bandIndex = 0; bandIndex < bandCount; bandIndex++) {
    int levelLeft = levelsLeft[bandIndex];
    int levelRight = levelsRight[bandIndex];

    if (drawPeaks) {
      levelLeft = peaksLeft[bandIndex];
      levelRight = peaksRight[bandIndex];
    }

    CRGB colorLeft = ColorFromPalette(palette, gHue);
    CRGB colorRight = ColorFromPalette(palette, gHue);

    uint8_t x = bandIndex + bandOffset;
    if (x >= MATRIX_WIDTH)
      x -= MATRIX_WIDTH;

    drawFastVLine(x, (MATRIX_HEIGHT - 1) - levelLeft / levelsPerVerticalPixel, MATRIX_HEIGHT - 1, colorLeft);
    drawFastVLine(x + bandCount, (MATRIX_HEIGHT - 1) - levelRight / levelsPerVerticalPixel, MATRIX_HEIGHT - 1, colorRight);
  }

  return 1;
}

uint16_t analyzerPixels() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  for (uint8_t bandIndex = 0; bandIndex < bandCount; bandIndex++) {
    int levelLeft = levelsLeft[bandIndex];
    int levelRight = levelsRight[bandIndex];

    if (drawPeaks) {
      levelLeft = peaksLeft[bandIndex];
      levelRight = peaksRight[bandIndex];
    }

    CRGB colorLeft = ColorFromPalette(palette, levelLeft / levelsPerHue);
    CRGB colorRight = ColorFromPalette(palette, levelRight / levelsPerHue);

    uint8_t x = bandIndex + bandOffset;
    if (x >= MATRIX_WIDTH)
      x -= MATRIX_WIDTH;

    leds[XY(x, (MATRIX_HEIGHT - 1) - levelLeft / levelsPerVerticalPixel)] = colorLeft;
    leds[XY(x + bandCount, (MATRIX_HEIGHT - 1) - levelLeft / levelsPerVerticalPixel)] = colorRight;
  }

  return 0;
}

uint16_t fallingSpectrogram() {
  moveDown();

  for (uint8_t bandIndex = 0; bandIndex < bandCount; bandIndex++) {
    int levelLeft = levelsLeft[bandIndex];
    int levelRight = levelsRight[bandIndex];

    if (drawPeaks) {
      levelLeft = peaksLeft[bandIndex];
      levelRight = peaksRight[bandIndex];
    }

    if (levelLeft <= 8) levelLeft = 0;
    if (levelRight <= 8) levelRight = 0;

    CRGB colorLeft;
    CRGB colorRight;

    if (currentPaletteIndex < 2) { // invert the first two palettes
      colorLeft = ColorFromPalette(palette, 205 - (levelLeft / levelsPerHue - 205));
      colorRight = ColorFromPalette(palette, 205 - (levelLeft / levelsPerHue - 205));
    }
    else {
      colorLeft = ColorFromPalette(palette, levelLeft / levelsPerHue);
      colorRight = ColorFromPalette(palette, levelRight / levelsPerHue);
    }

    uint8_t x = bandIndex + bandOffset;
    if (x >= MATRIX_WIDTH)
      x -= MATRIX_WIDTH;

    leds[XY(x, 0)] = colorLeft;
    leds[XY(x + bandCount, 0)] = colorRight;
  }

  return 0;
}

uint16_t audioFire() {
  moveUp();

  for (uint8_t bandIndex = 0; bandIndex < bandCount; bandIndex++) {
    int levelLeft = levelsLeft[bandIndex];
    int levelRight = levelsRight[bandIndex];

    if (drawPeaks) {
      levelLeft = peaksLeft[bandIndex];
      levelRight = peaksRight[bandIndex];
    }

    if (levelLeft <= 8) levelLeft = 0;
    if (levelRight <= 8) levelRight = 0;

    CRGB colorLeft = ColorFromPalette(HeatColors_p, levelLeft / 5);
    CRGB colorRight = ColorFromPalette(HeatColors_p, levelRight / 5);

    uint8_t x = bandIndex + bandOffset;
    if (x >= MATRIX_WIDTH)
      x -= MATRIX_WIDTH;

    leds[XY(x, MATRIX_HEIGHT - 1)] = colorLeft;
    leds[XY(x + bandCount, MATRIX_HEIGHT - 1)] = colorRight;
  }

  return 0;
}

uint16_t rainbowAudioNoise() {
  static int lastPeak0 = 0;

  noisespeedx = 0;

  if (peaksLeft[0] >= lastPeak0) {
    noisespeedx = peaksLeft[0] / 57;
  }

  lastPeak0 = peaksLeft[0];

  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 30;
  colorLoop = 0;
  return drawNoise(RainbowColors_p);
}

uint16_t rainbowStripeAudioNoise() {
  static int lastPeak0 = 0;

  noisespeedy = 0;

  if (peaksLeft[0] >= lastPeak0) {
    noisespeedy = peaksLeft[0] / 57;
  }

  lastPeak0 = peaksLeft[0];

  noisespeedx = 0;
  noisespeedz = 0;
  noisescale = 20;
  colorLoop = 0;
  return drawNoise(RainbowStripeColors_p);
}

uint16_t partyAudioNoise() {
  static int lastPeak0 = 0;

  noisespeedx = 0;

  if (peaksLeft[0] >= lastPeak0) {
    noisespeedx = peaksLeft[0] / 57;
  }

  lastPeak0 = peaksLeft[0];

  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 30;
  colorLoop = 0;
  return drawNoise(PartyColors_p);
}

uint16_t forestAudioNoise() {
  static int lastPeak0 = 0;

  noisespeedx = 0;

  if (peaksLeft[0] >= lastPeak0) {
    noisespeedx = peaksLeft[0] / 57;
  }

  lastPeak0 = peaksLeft[0];

  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 120;
  colorLoop = 0;
  return drawNoise(ForestColors_p);
}

uint16_t cloudAudioNoise() {
  static int lastPeak0 = 0;

  noisespeedx = 0;

  if (peaksLeft[0] >= lastPeak0) {
    noisespeedx = peaksLeft[0] / 57;
  }

  lastPeak0 = peaksLeft[0];

  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 30;
  colorLoop = 0;
  return drawNoise(CloudColors_p);
}

uint16_t fireAudioNoise() {
  static int lastPeak0 = 0;
  static int lastPeak6 = 0;

  noisespeedx = 0;
  noisespeedz = 0;

  if (peaksLeft[0] >= lastPeak0) {
    noisespeedx = peaksLeft[0] / 32;
  }

  if (peaksLeft[6] >= lastPeak6) {
    noisespeedz = peaksLeft[6] / 128;
  }

  lastPeak0 = peaksLeft[0];
  lastPeak6 = peaksLeft[6];

  noisespeedy = 0;
  noisescale = 50;
  colorLoop = 0;

  return drawNoise(HeatColors_p, 60);
}

uint16_t lavaAudioNoise() {
  static int lastPeak0 = 0;
  static int lastPeak6 = 0;

  noisespeedy = 0;
  noisespeedz = 0;

  if (peaksLeft[0] >= lastPeak0) {
    noisespeedy = peaksLeft[0] / 32;
  }

  if (peaksLeft[6] >= lastPeak6) {
    noisespeedz = peaksLeft[6] / 128;
  }

  lastPeak0 = peaksLeft[0];
  lastPeak6 = peaksLeft[6];

  noisespeedx = 0;
  noisescale = 50;
  colorLoop = 0;
  return drawNoise(LavaColors_p);
}

uint16_t oceanAudioNoise() {
  static int lastPeak0 = 0;

  noisespeedy = 0;

  if (peaksLeft[0] >= lastPeak0) {
    noisespeedy = peaksLeft[0] / 57;
  }

  lastPeak0 = peaksLeft[0];

  noisespeedx = 0;
  noisespeedz = 0;
  noisescale = 90;
  colorLoop = 0;
  return drawNoise(OceanColors_p);
}

uint16_t blackAndWhiteAudioNoise() {
  SetupBlackAndWhiteStripedPalette();
  static int lastPeak0 = 0;

  noisespeedy = 0;

  if (peaksLeft[0] >= lastPeak0) {
    noisespeedy = peaksLeft[0] / 128;
  }

  lastPeak0 = peaksLeft[0];

  noisespeedx = 0;
  noisespeedz = 0;
  noisescale = 15;
  colorLoop = 0;
  return drawNoise(blackAndWhiteStripedPalette);
}

uint16_t blackAndBlueAudioNoise() {
  SetupBlackAndBlueStripedPalette();
  static int lastPeak0 = 0;

  noisespeedx = 0;

  if (peaksLeft[0] >= lastPeak0) {
    noisespeedx = peaksLeft[0] / 57;
  }

  lastPeak0 = peaksLeft[0];

  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 45;
  colorLoop = 0;
  return drawNoise(blackAndBlueStripedPalette);
}
