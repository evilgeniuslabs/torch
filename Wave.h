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

uint16_t wave() {
  static byte rotation = 3;

  static const uint8_t scaleWidth = 256 / MATRIX_WIDTH;
  static const uint8_t scaleHeight = 256 / MATRIX_HEIGHT;

  static const uint8_t maxX = MATRIX_WIDTH - 1;
  static const uint8_t maxY = MATRIX_HEIGHT - 1;

  static uint8_t waveCount = 1;
  static uint8_t hue = 1;
  static uint8_t theta = 0;

//  EVERY_N_SECONDS(10) {
//    rotation = random(0, 1);
//    waveCount = random(1, 3);
//  }

  int n = 0;

  switch (rotation) {
    case 0:
      for (int x = 0; x < MATRIX_WIDTH; x++) {
        n = quadwave8(x * 2 + theta) / scaleHeight;
        leds[XY(x, n)] = ColorFromPalette(palette, x + hue);
        if (waveCount == 2)
          leds[XY(x, maxY - n)] = ColorFromPalette(palette, x + hue);
      }
      break;

    case 1:
      for (int y = 0; y < MATRIX_HEIGHT; y++) {
        n = quadwave8(y * 2 + theta) / scaleWidth;
        leds[XY(n, y)] = ColorFromPalette(palette, y + hue);
        if (waveCount == 2)
          leds[XY(maxX - n, y)] = ColorFromPalette(palette, y + hue);
      }
      break;

    case 2:
      for (int x = 0; x < MATRIX_WIDTH; x++) {
        n = quadwave8(x * 2 - theta) / scaleHeight;
        leds[XY(x, n)] = ColorFromPalette(palette, x + hue);
        if (waveCount == 2)
          leds[XY(x, maxY - n)] = ColorFromPalette(palette, x + hue);
      }
      break;

    case 3:
      for (int y = 0; y < MATRIX_HEIGHT; y++) {
        n = quadwave8(y * 2 - theta) / scaleWidth;
        leds[XY(n, y)] = ColorFromPalette(palette, y + hue);
        if (waveCount == 2)
          leds[XY(maxX - n, y)] = ColorFromPalette(palette, y + hue);
      }
      break;
  }

  dimAll(254);

  EVERY_N_MILLISECONDS(10) {
    theta++;
    hue++;
  }

  return 0;
}
