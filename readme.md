## About
This repository contains sample code for controlling RGB LEDs on the "Glow Stick" 3D printable R/C airplane from [Lofted Aero](www.loftedaero.com)

## Included Examples
Currently included programs are:

### GlowStickPatterns
This program uses an Excel template to create full-color lighting patterns for the wings and tail surfaces. An Arduino sketch stores and control these patterns on the aircraft, accepting R/C inputs to toggle between multiple schemes and control overall brightness.

## Hardware Configuration
Currently supported hardware configurations are:

### LEDs
[WS2815 12V addressible LED strips](https://www.amazon.com/gp/product/B07LG4VLKV/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&th=1). 12V is used rather than 5V to keep current draw reasonable in the full 568-LED configuration.

### Control Board
Arduino Nano or equivalent, wired as shown: 
