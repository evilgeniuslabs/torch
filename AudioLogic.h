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

int noiseCorrection[bandCount] = {
  //  -55, -50, -45, -55, -40, -55, -50,
  0, 0, 0, 0, 0, 0, 0
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
    delayMicroseconds(30);

    levelLeft = analogRead(MSGEQ7_LEFT_PIN);
    levelRight = analogRead(MSGEQ7_RIGHT_PIN);
    digitalWrite(MSGEQ7_STROBE_PIN, HIGH);

    levelLeft += noiseCorrection[band];
    levelRight += noiseCorrection[band];

//    if (levelLeft < 0) levelLeft = 0;
//    if (levelLeft > 1023) levelLeft = 1023;
//
//    if (levelRight < 0) levelRight = 0;
//    if (levelRight > 1023) levelRight = 1023;

    levelsLeft[band] = levelLeft;
    levelsRight[band] = levelRight;

    //    if (levelLeft >= peaksLeft[band]) {
    peaksLeft[band] = levelLeft;
    //    }
    //    else if (peaksLeft[band] > 0) {
    //      peaksLeft[band] = peaksLeft[band] - peakDecay;
    //      if(peaksLeft[band] < 0) peaksLeft[band] = 0;
    //    }
    //
    //    if (levelRight >= peaksRight[band]) {
    peaksRight[band] = levelRight;
    //    }
    //    else if (peaksRight[band] > 0) {
    //      peaksRight[band] = peaksRight[band] - peakDecay;
    //      if(peaksRight[band] < 0) peaksRight[band] = 0;
    //    }
  }
}
