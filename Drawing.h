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
