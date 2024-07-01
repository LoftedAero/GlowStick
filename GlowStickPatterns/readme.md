## About
This program uses:
- An Excel spreadsheet and macro to capture LED color patterns for the wings and tail of the Glow Stick
- An Arduino sketch to store and control these patterns using the Arduino Nano control board
- The Adafruit NeoPixel and Adafruit NeoMatrix libraries to drive the WS2815 LEDs as matrices
- R/C channels to select between multiple patterns and control overall LED brightness

## Usage
1) Place the Excel workbook, Arduino sketch, and patterns.h file all in the same directory.
2) Open the Excel workbook. You may need to enable macros and re-open the file depending on your system settings.
3) Add or remove copies of the sheet to match the number of lighting patterns you'd like to load. For example, six sheets will produce six patterns you can switch between.
4) Fill the cells representing the wings and tail surfaces with your desired colors. Use black fill for LEDs you'd like to turn off.
5) Click the "Generate" button on sheet 1. The macro will save the patterns to a file called "patterns.h" in the directory this spreadsheet is saved. 
6) Open the GlowStickPatterns.ino file in Arduino IDE and flash it to the device using the appropriate board and COM port settings.

## Key Details
The following elements of the code can be modified as needed:

### Pin Mapping
```arduino
#define INPUT_PIN 2
#define BRGHT_PIN 3
#define LTAIL_PIN 4
#define RTAIL_PIN 5
#define LWING_PIN 11
#define RWING_PIN 12
 ```
These definitions control the mapping of pins on the Arduino Nano to the various signals. The `INPUT_PIN` and `BRGHT_PIN` are the pins connected to R/C receiver channels to control the selected pattern and the overall brightness, respectively. The rest are the pins connected to the input of each LED array on the wings and tail surfaces. These correspond to the wiring diagram on the previous project page by default, but you can change them if you've wired your hardware differently. 

### Input Ranges
```arduino
unsigned int PWM_MIN = 1000;
unsigned int PWM_MAX = 2000;
unsigned int BRIGHT_MIN = 0;
unsigned int BRIGHT_MAX = 255;
 ```
These definitions control the minimum and maximum PWM input expected from your R/C receiver and the minimum and maximum brightness corresponding to that range. You can adjust either range as needed.

### Enables and Defaults
```arduino
bool INPUT_ENABLE = 1;
bool BRIGHT_ENABLE = 1;
unsigned int pattern = 1;
unsigned int brightness = 50;
 ```
You can set `INPUT_ENABLE` or `BRIGHT_ENABLE` to 0 if you don't want to use R/C channels to control pattern or brightness. If you do so, the value of `pattern` controls the pattern that's displayed at a fixed value of `brightness`.

## Video Tutorial

[![Video Tutorial](https://i9.ytimg.com/vi/yK6Lo7ZanPA/mqdefault.jpg?v=6682389d&sqp=CLSJibQG&rs=AOn4CLBkqVBLoqrbleEGo5i64_KvNYz85A)](https://youtu.be/yK6Lo7ZanPA "Video Tutorial")

