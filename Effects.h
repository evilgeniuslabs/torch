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

// give it a linear tail to the right
void streamRight(byte scale, int fromX = 0, int toX = MATRIX_WIDTH, int fromY = 0, int toY = MATRIX_HEIGHT)
{
  for (int x = fromX + 1; x < toX; x++) {
    for (int y = fromY; y < toY; y++) {
      leds[XY(x, y)] += leds[XY(x - 1, y)];
      leds[XY(x, y)].nscale8(scale);
    }
  }
  for (int y = fromY; y < toY; y++)
    leds[XY(0, y)].nscale8(scale);
}

// give it a linear tail to the left
void streamLeft(byte scale, int fromX = MATRIX_WIDTH, int toX = 0, int fromY = 0, int toY = MATRIX_HEIGHT)
{
  for (int x = toX; x < fromX; x++) {
    for (int y = fromY; y < toY; y++) {
      leds[XY(x, y)] += leds[XY(x + 1, y)];
      leds[XY(x, y)].nscale8(scale);
    }
  }
  for (int y = fromY; y < toY; y++)
    leds[XY(0, y)].nscale8(scale);
}

// give it a linear tail downwards
void streamDown(byte scale)
{
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    for (int y = 1; y < MATRIX_HEIGHT; y++) {
      leds[XY(x, y)] += leds[XY(x, y - 1)];
      leds[XY(x, y)].nscale8(scale);
    }
  }
  for (int x = 0; x < MATRIX_WIDTH; x++)
    leds[XY(x, 0)].nscale8(scale);
}

// give it a linear tail upwards
void streamUp(byte scale)
{
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    for (int y = MATRIX_HEIGHT - 2; y >= 0; y--) {
      leds[XY(x, y)] += leds[XY(x, y + 1)];
      leds[XY(x, y)].nscale8(scale);
    }
  }
  for (int x = 0; x < MATRIX_WIDTH; x++)
    leds[XY(x, MATRIX_HEIGHT - 1)].nscale8(scale);
}

// give it a linear tail up and to the left
void streamUpAndLeft(byte scale)
{
  for (int x = 0; x < MATRIX_WIDTH - 1; x++) {
    for (int y = MATRIX_HEIGHT - 2; y >= 0; y--) {
      leds[XY(x, y)] += leds[XY(x + 1, y + 1)];
      leds[XY(x, y)].nscale8(scale);
    }
  }
  for (int x = 0; x < MATRIX_WIDTH; x++)
    leds[XY(x, MATRIX_HEIGHT - 1)].nscale8(scale);
  for (int y = 0; y < MATRIX_HEIGHT; y++)
    leds[XY(MATRIX_WIDTH - 1, y)].nscale8(scale);
}

// give it a linear tail up and to the right
void streamUpAndRight(byte scale)
{
  for (int x = 0; x < MATRIX_WIDTH - 1; x++) {
    for (int y = MATRIX_HEIGHT - 2; y >= 0; y--) {
      leds[XY(x + 1, y)] += leds[XY(x, y + 1)];
      leds[XY(x, y)].nscale8(scale);
    }
  }
  // fade the bottom row
  for (int x = 0; x < MATRIX_WIDTH; x++)
    leds[XY(x, MATRIX_HEIGHT - 1)].nscale8(scale);

  // fade the right column
  for (int y = 0; y < MATRIX_HEIGHT; y++)
    leds[XY(MATRIX_WIDTH - 1, y)].nscale8(scale);
}

void moveUp()
{
  for (int y = 0; y < MATRIX_HEIGHT - 1; y++) {
    for (int x = 0; x < MATRIX_WIDTH; x++) {
      leds[XY(x, y)] = leds[XY(x, y + 1)];
    }
  }
}

void moveDown() {
  for (int y = MATRIX_HEIGHT - 1; y > 0; y--) {
    for (int x = 0; x < MATRIX_WIDTH; x++) {
      leds[XY(x, y)] = leds[XY(x, y - 1)];
    }
  }
}
