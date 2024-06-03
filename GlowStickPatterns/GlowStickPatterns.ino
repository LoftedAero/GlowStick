#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

// Include pixel map
#include "patterns.h"

// Pin mapping
#define INPUT_PIN 2
#define BRGHT_PIN 3
#define LTAIL_PIN 4
#define RTAIL_PIN 5
#define LWING_PIN 11
#define RWING_PIN 12

// Matrix parameters
#define WING_W 29
#define WING_H 8
#define TAIL_W 9
#define TAIL_H 4

// Input ranges
unsigned long INPUT_PWM;
unsigned long BRGHT_PWM;
unsigned int PWM_MIN = 1000;
unsigned int PWM_MAX = 2000;
unsigned int BRIGHT_MIN = 0;
unsigned int BRIGHT_MAX = 255;

// Other variables
bool INPUT_ENABLE = 1;
bool BRIGHT_ENABLE = 1;
unsigned int pattern = 1;
unsigned int brightness = 50;
unsigned int current_pattern = 0;
unsigned int current_brightness = 50;
unsigned int brightness_deadzone = 10;
int thresholds[PCOUNT - 1];
unsigned long loopPeriod = 200;
unsigned long lastLoop;

void setup() {
//  Serial.begin(115200);
//  delay(1000);
//  Serial.println("Serial port initialized");

  // Configure R/C inputs
#ifdef INPUT_PIN
  pinMode(INPUT_PIN, INPUT);
#endif

#ifdef BRGHT_PIN
  pinMode(BRGHT_PIN, INPUT);
#endif

  // Set up pattern switching via R/C input if multiple patterns are present
  if (PCOUNT > 1) {
    int stepSize = (PWM_MAX - PWM_MIN) / (PCOUNT);

    // Generate threshold values for pattern switching
    for (int i = 0; i < (PCOUNT - 1); i++) {
      thresholds[i] = PWM_MIN + ((i + 1) * stepSize);
    }

    // If only one pattern is present, disable switching
  } else {
    INPUT_ENABLE = 0;
  }
}


void loop() {
  // If pattern switching is enabled, poll the input pin and use an R/C channel value to toggle between patterns
  if (INPUT_ENABLE) {
    INPUT_PWM = constrain(pulseIn(INPUT_PIN, HIGH), PWM_MIN, PWM_MAX);
    if (INPUT_PWM > thresholds[PCOUNT - 2]) {
      pattern = PCOUNT;
    } else {
      for (int i = 0; i < (PCOUNT - 1); i++) {
        if (INPUT_PWM < thresholds[i]) {
          pattern = i + 1;
          break;
        }
      }
    }
  }

  // If brightness control is enabled, poll the brightness pin and use an R/C channel to control brightness
  if (BRIGHT_ENABLE) {
    BRGHT_PWM = constrain(pulseIn(BRGHT_PIN, HIGH), PWM_MIN, PWM_MAX);
    brightness = map(BRGHT_PWM, PWM_MIN, PWM_MAX, BRIGHT_MIN, BRIGHT_MAX);
    // Discard brightness values that seem invalid
    if ((brightness < 0) || (brightness > BRIGHT_MAX)) {
      brightness = current_brightness;
    }
  }

  // If there's been a change, trigger the arrays to update. This also takes effect once on first boot for cases where switching is disabled.
  if ((pattern != current_pattern) || (abs(brightness - current_brightness) > brightness_deadzone)) {
    displayBitmap(leftTailBitmaps[pattern - 1], brightness, TAIL_W, TAIL_H, LTAIL_PIN, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);
    displayBitmap(rightTailBitmaps[pattern - 1], brightness, TAIL_W, TAIL_H, RTAIL_PIN, NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);
    displayBitmap(leftWingBitmaps[pattern - 1], brightness, WING_W, WING_H, LWING_PIN, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);
    displayBitmap(rightWingBitmaps[pattern - 1], brightness, WING_W, WING_H, RWING_PIN, NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);
    current_pattern = pattern;
    current_brightness = brightness;
  }

  // Enforce constant loop period
  while (millis() - lastLoop < loopPeriod) {
  }
  lastLoop = millis();
}

void displayBitmap(const unsigned short *bitmap, unsigned int brightness, uint16_t width, uint16_t height, uint8_t pin, uint8_t layout, uint16_t type) {
  // Create the matrix object dynamically
  Adafruit_NeoMatrix *matrix = new Adafruit_NeoMatrix(width, height, pin, layout, type);

  // Begin operation
  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(brightness);

  // Display bitmap
  for (uint16_t i = 0; i < height; i++) {
    for (uint16_t j = 0; j < width; j++) {
      uint16_t color = pgm_read_word(bitmap + i * width + j);
      uint8_t r = color & 0x00F;
      uint8_t g = (color & 0x0F0) >> 4;
      uint8_t b = (color & 0xF00) >> 8;
      // Convert from 4/4/4 to 5/6/5
      b = map(b, 0, 15, 0, 31);
      g = map(g, 0, 15, 0, 63);
      r = map(r, 0, 15, 0, 31);
      matrix->drawPixel(j, i, matrix->Color(r << 3, g << 2, b << 3));
    }
  }

  // Show matrix
  matrix->show();

  // Delete the matrix object to free memory
  delete matrix;
  matrix = nullptr;
}
