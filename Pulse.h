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

uint16_t pulse() {
//  palette = RainbowColors_p;

  static uint8_t hue = 0;
  static uint8_t centerX = 0;
  static uint8_t centerY = 0;
  static uint8_t step = 0;

  static const uint8_t maxSteps = 16;
  static const float fadeRate = 0.8;

  dimAll(235);

  if (step == 0) {
    centerX = random(32);
    centerY = random(32);
    hue = random(256); // 170;

    drawCircle(centerX, centerY, step, ColorFromPalette(palette, hue));
    step++;
  }
  else {
    if (step < maxSteps) {
      // initial pulse
      drawCircle(centerX, centerY, step, ColorFromPalette(palette, hue, pow(fadeRate, step - 2) * 255));

      // secondary pulse
      if (step > 3) {
        drawCircle(centerX, centerY, step - 3, ColorFromPalette(palette, hue, pow(fadeRate, step - 2) * 255));
      }
      step++;
    }
    else {
      step = 0;
    }
  }

  return 30;
}
