// Portions of this code are adapted from "NoiseSmearing" by Stefan Petrick: https://gist.github.com/StefanPetrick/9ee2f677dbff64e3ba7a

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

