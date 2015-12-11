struct Fairy {
  uint8_t x = 0;
  uint8_t y = 0;
  uint8_t hue = 0;
  uint8_t val = 0;
};

const uint8_t fairyCount = 40;

Fairy fairies[fairyCount];

uint16_t drawFairies() {
  static boolean started = false;

  if(!started) {
    started = true;
      
    // set all the fairies to random positions
    for (uint8_t i = 0; i < fairyCount; i++) {
      fairies[i].x = random(0, MATRIX_WIDTH);
      fairies[i].y = random(0, MATRIX_HEIGHT);
      fairies[i].hue = random(0, 256);
      fairies[i].val = random(0, 256);
    }
  }
  
  fadeToBlackBy(leds, NUM_LEDS, 196);

  EVERY_N_MILLISECONDS(60) {
    // update fairies
    for (uint8_t i = 0; i < fairyCount; i++) {
      if(random(0, 100) > 98) {
        if(random(0, 2) == 0) {
          fairies[i].y += random(0, 3) - 1;
        }
        else {
          fairies[i].x += random(0, 3) - 1;
        }
    
        fairies[i].y = fairies[i].y % MATRIX_HEIGHT;
        fairies[i].x = fairies[i].x % MATRIX_WIDTH;
      }

      fairies[i].hue++;
      fairies[i].val = beatsin8(8, 0, 255, 0, (255 / fairyCount) * i);
    }
  }

  // draw fairies
  CRGB w(85,85,85), W(CRGB::White);
  gPalette = CRGBPalette16( W,W,W,W, w,w,w,w, w,w,w,w, w,w,w,w );
  
  for (uint8_t i = 0; i < fairyCount; i++) {
    leds[XY(fairies[i].x, fairies[i].y)] = ColorFromPalette(gPalette, fairies[i].hue, fairies[i].val); // CHSV(fairies[i].hue, 255, 255);
  }

  return 0;
}
