// Demo built-in RGB LED of RP2040-Zero
// Fade between red, green, and blue in a cycle

#include <Adafruit_NeoPixel.h>

#define PIN       16 // IO pin for RGB LED
#define NUMPIXELS 1  // 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
}

void loop() {
  int i;
  for (i=0; i<=150; i+=10){
    pixels.setPixelColor(0, pixels.Color(150-i, i, 0));
    pixels.show();
    delay(50);
  }
  for (i=0; i<=150; i+=10){
    pixels.setPixelColor(0, pixels.Color(0, 150-i, i));
    pixels.show();
    delay(50);
  }
  for (i=0; i<=150; i+=10){
    pixels.setPixelColor(0, pixels.Color(i, 0, 150-i));
    pixels.show();
    delay(50);
  }
}
