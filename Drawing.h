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

void drawCircle(int16_t x0, int16_t y0, uint16_t radius, const CRGB& color)
{
  int a = radius, b = 0;
  int radiusError = 1 - a;

  if (radius == 0) {
    leds[XY(x0, y0)] = color;
    return;
  }

  while (a >= b)
  {
    leds[XY(a + x0, b + y0)] = color;
    leds[XY(b + x0, a + y0)] = color;
    leds[XY(-a + x0, b + y0)] = color;
    leds[XY(-b + x0, a + y0)] = color;
    leds[XY(-a + x0, -b + y0)] = color;
    leds[XY(-b + x0, -a + y0)] = color;
    leds[XY(a + x0, -b + y0)] = color;
    leds[XY(b + x0, -a + y0)] = color;

    b++;
    if (radiusError < 0)
      radiusError += 2 * b + 1;
    else
    {
      a--;
      radiusError += 2 * (b - a + 1);
    }
  }
}

void drawFastVLine(uint16_t x, uint16_t y0, uint16_t y1, const CRGB& color) {
  uint16_t i;

  for (i = y0; i <= y1; i++) {
    leds[XY(x, i)] = color;
  }
}
