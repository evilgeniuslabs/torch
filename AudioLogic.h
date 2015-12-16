/*
 * Torch: https://github.com/evilgeniuslabs/torch
 * Copyright (c) 2015 Jason Coon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#define MSGEQ7_STROBE_PIN 2
#define MSGEQ7_RESET_PIN  3
#define MSGEQ7_LEFT_PIN   A0
#define MSGEQ7_RIGHT_PIN  A1

const uint8_t bandCount = 7;

int levelsLeft[bandCount];
int peaksLeft[bandCount];

int levelsRight[bandCount];
int peaksRight[bandCount];

static const uint8_t peakDecay = (1024 / MATRIX_HEIGHT) / 4;
bool drawPeaks = true;

int noiseCorrection[bandCount * 2] = {
  -40, -40, -40, -40, -40, -40, -40, 
  -40, -40, -40, -40, -40, -40, -40, 
};

uint8_t bandOffset = 3;

uint8_t horizontalPixelsPerBand = MATRIX_WIDTH / (bandCount * 2);

uint8_t levelsPerVerticalPixel = 63; // 1024 / MATRIX_HEIGHT;

uint8_t levelsPerHue = 1024 / 256;

void initializeAudio() {
  pinMode(MSGEQ7_LEFT_PIN, INPUT);
  pinMode(MSGEQ7_RIGHT_PIN, INPUT);
  pinMode(MSGEQ7_RESET_PIN, OUTPUT);
  pinMode(MSGEQ7_STROBE_PIN, OUTPUT);
  digitalWrite(MSGEQ7_RESET_PIN, LOW);
  digitalWrite(MSGEQ7_STROBE_PIN, HIGH);
}

void readAudio() {
  digitalWrite(MSGEQ7_RESET_PIN, HIGH);
  digitalWrite(MSGEQ7_RESET_PIN, LOW);

  int levelLeft;
  int levelRight;

  for (uint8_t band = 0; band < bandCount; band++) {
    digitalWrite(MSGEQ7_STROBE_PIN, LOW);
    delayMicroseconds(20);

    levelLeft = analogRead(MSGEQ7_LEFT_PIN);
    levelRight = analogRead(MSGEQ7_RIGHT_PIN);
    digitalWrite(MSGEQ7_STROBE_PIN, HIGH);

    levelLeft += noiseCorrection[band];
    levelRight += noiseCorrection[band + bandCount];

    if(levelLeft < 0) levelLeft = 0;
    if(levelLeft > 1024) levelLeft = 1024;
    
    if(levelRight < 0) levelRight = 0;
    if(levelRight > 1024) levelRight = 1024;
    
    levelsLeft[band] = levelLeft;
    levelsRight[band] = levelRight;
    
    if (levelLeft >= peaksLeft[band]) {
      peaksLeft[band] = levelLeft;
    }
    else if (peaksLeft[band] > 0) {
      peaksLeft[band] = peaksLeft[band] - peakDecay;
      if(peaksLeft[band] < 0) peaksLeft[band] = 0;
    }

    if (levelRight >= peaksRight[band]) {
      peaksRight[band] = levelRight;
    }
    else if (peaksRight[band] > 0) {
      peaksRight[band] = peaksRight[band] - peakDecay;
      if(peaksRight[band] < 0) peaksRight[band] = 0;
    }
  }
}

