#define MAX_DIMENSION ((MATRIX_WIDTH > MATRIX_HEIGHT) ? MATRIX_WIDTH : MATRIX_HEIGHT)

// The 16 bit version of our coordinates
static uint16_t noisex;
static uint16_t noisey;
static uint16_t noisez;

// We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
// use the z-axis for "time".  speed determines how fast time moves forward.  Try
// 1 for a very slow moving effect, or 60 for something that ends up looking like
// water.
uint16_t noisespeedx = 1;
uint16_t noisespeedy = 1;
uint16_t noisespeedz = 1;

// Scale determines how far apart the pixels in our noise matrix are.  Try
// changing these values around to see how it affects the motion of the display.  The
// higher the value of scale, the more "zoomed out" the noise iwll be.  A value
// of 1 will be so zoomed in, you'll mostly see solid colors.
uint16_t noisescale = 30; // scale is set dynamically once we've started up

// This is the array that we keep our computed noise values in
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];

uint8_t colorLoop = 0;
// This function generates a random palette that's a gradient
// between four different colors.  The first is a dim hue, the second is 
// a bright hue, the third is a bright pastel, and the last is 
// another bright hue.  This gives some visual bright/dark variation
// which is more interesting than just a gradient of different hues.
void SetupRandomPalette()
{
  gPalette = CRGBPalette16( 
                      CHSV( random8(), 255, 32), 
                      CHSV( random8(), 255, 255), 
                      CHSV( random8(), 128, 255), 
                      CHSV( random8(), 255, 255)); 
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( gPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  gPalette[0] = CRGB::White;
  gPalette[4] = CRGB::White;
  gPalette[8] = CRGB::White;
  gPalette[12] = CRGB::White;

}

// This function sets up a palette of black and blue stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndBlueStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( gPalette, 16, CRGB::Black);

  for(uint8_t i = 0; i < 6; i++) {
    gPalette[i] = CRGB::Blue;
  }
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;
  
  gPalette = CRGBPalette16( 
    green,  green,  black,  black,
    purple, purple, black,  black,
    green,  green,  black,  black,
    purple, purple, black,  black );
}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.

boolean initialized = false;

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void fillnoise8() {

  if(!initialized) {
    initialized = true;
    // Initialize our coordinates to some random values
    noisex = random16();
    noisey = random16();
    noisez = random16();
  }
  
  // If we're runing at a low "speed", some 8-bit artifacts become visible
  // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
  // The amount of data smoothing we're doing depends on "speed".
  uint8_t dataSmoothing = 0;
  uint16_t lowestNoise = noisespeedx < noisespeedy ? noisespeedx : noisespeedy;
  lowestNoise = lowestNoise < noisespeedz ? lowestNoise : noisespeedz;
  if( lowestNoise < 8) {
    dataSmoothing = 200 - (lowestNoise * 4);
  }
  
  for(int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = noisescale * i;
    for(int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = noisescale * j;
      
      uint8_t data = inoise8(noisex + ioffset, noisey + joffset, noisez);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      data = qsub8(data,16);
      data = qadd8(data,scale8(data,39));

      if( dataSmoothing ) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }
      
      noise[i][j] = data;
    }
  }
  
  noisex += noisespeedx;
  noisey += noisespeedy;
  noisez += noisespeedz;
}

void mapNoiseToLEDsUsingPalette(uint8_t hueReduce = 0)
{
  static uint8_t ihue=0;
  
  for(int i = 0; i < MATRIX_WIDTH; i++) {
    for(int j = 0; j < MATRIX_HEIGHT; j++) {
      // We use the value at the (i,j) coordinate in the noise
      // array for our brightness, and the flipped value from (j,i)
      // for our pixel's index into the color palette.

      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];

      // if this palette is a 'loop', add a slowly-changing base value
      if( colorLoop) { 
        index += ihue;
      }

      // brighten up, as the color palette itself often contains the 
      // light/dark dynamic range desired
      if( bri > 127 ) {
        bri = 255;
      } else {
        bri = dim8_raw( bri * 2);
      }

      if(hueReduce > 0) {
        if(index < hueReduce) index = 0;
        else index -= hueReduce;
      }

      CRGB color = ColorFromPalette( gPalette, index, bri);
      uint16_t n = XY(i, j);

      leds[n] = color;
    }
  }
  
  ihue+=1;
}

uint16_t drawNoise(uint8_t hueReduce = 0) {
  // generate noise data
  fillnoise8();
  
  // convert the noise data to colors in the LED array
  // using the current palette
  mapNoiseToLEDsUsingPalette(hueReduce);

  return 10;
}

uint16_t rainbowNoise() {
  gPalette = RainbowColors_p;
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 30;
  colorLoop = 0; 
  return drawNoise();
}

uint16_t rainbowStripeNoise() {
  gPalette = RainbowStripeColors_p;
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 20;
  colorLoop = 0; 
  return drawNoise();
}

uint16_t partyNoise() {
  gPalette = PartyColors_p;
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 30;
  colorLoop = 0; 
  return drawNoise();
}

uint16_t forestNoise() {
  gPalette = ForestColors_p;
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 120;
  colorLoop = 0; 
  return drawNoise();
}

uint16_t cloudNoise() {
  gPalette = CloudColors_p;
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 30;
  colorLoop = 0; 
  return drawNoise();
}

uint16_t fireNoise() {
  gPalette = HeatColors_p;
  noisespeedx = 8; // 24;
  noisespeedy = 0;
  noisespeedz = 8;
  noisescale = 50;
  colorLoop = 0; 
  return drawNoise(60);
}

uint16_t lavaNoise() {
  gPalette = LavaColors_p;
  noisespeedx = 32;
  noisespeedy = 0;
  noisespeedz = 16;
  noisescale = 50;
  colorLoop = 0; 
  return drawNoise();
}

uint16_t oceanNoise() {
  gPalette = OceanColors_p;
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 90;
  colorLoop = 0; 
  return drawNoise();
}

uint16_t blackAndWhiteNoise() {
  SetupBlackAndWhiteStripedPalette();
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 30;
  colorLoop = 0; 
  return drawNoise();
}

uint16_t blackAndBlueNoise() {
  SetupBlackAndBlueStripedPalette();
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 30;
  colorLoop = 0; 
  return drawNoise();
}

