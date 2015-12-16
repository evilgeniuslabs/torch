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

void moveDown() {
  for (int y = MATRIX_HEIGHT - 1; y > 0; y--) {
    for (int x = 0; x < MATRIX_WIDTH; x++) {
      leds[XY(x, y)] = leds[XY(x, y - 1)];
    }
  }
}
