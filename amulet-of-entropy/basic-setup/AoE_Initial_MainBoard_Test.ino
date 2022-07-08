// Uses RP2040-Zero on-board LED for inital HW tests
//
// Turns LED blue for one second
// Black for one second
//
// Reads temperature sensor.  If the value read sanity checks:
// The LED is green for five seconds, else red for five seconds.
//
// Then cycles checking the four buttons.
//    LED blinks blue 1, 2, 3, or 4 times
//    to shows which of the four buttons is pressed.


#include <Adafruit_NeoPixel.h>

#define PIN       16 // IO pin for INTERNAL RGB LED
#define NUMPIXELS 1  //
 
//IO Pins
#define B1_pin    5
#define B2_pin    6
#define B3_pin    7
#define B4_pin    8
#define MPU_SDA   14
#define MPU_SCL   15
#define LED_pin   26
#define LDR_pin   27
#define Shot_pin  28
#define TMP_pin   29

int B1_val, B2_val, B3_val, B4_val;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();

  pinMode(B1_pin, INPUT_PULLUP);
  pinMode(B2_pin, INPUT_PULLUP);
  pinMode(B3_pin, INPUT_PULLUP);
  pinMode(B4_pin, INPUT_PULLUP);
  pinMode(LDR_pin, INPUT);
  pinMode(Shot_pin, INPUT);
  pinMode(TMP_pin, INPUT);
}

void loop() {
  //flash internal LED blue (signal startup)
  pixels.setPixelColor(0, pixels.Color(0, 0, 150));
  pixels.show();
  delay(1000);
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
  delay(1000);

  int temp=ReadTMP();
  if ((temp>130) || (temp<60))
  {
      //wrong temp read - make LED red for 5 second
      pixels.setPixelColor(0, pixels.Color(150, 0, 0));
      pixels.show();
      delay(5000);
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.show();
      delay(1000);
  } else {
      //OK temp read - make LED green for 5 second
      pixels.setPixelColor(0, pixels.Color(0, 150, 0));
      pixels.show();
      delay(5000);
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.show();
      delay(1000); 
  }

  while(1){   // FOREVER
  
    ReadButtons(); 
    if(B1_val){
      // blink blue ONCE for 1 sec
      pixels.setPixelColor(0, pixels.Color(0, 0, 150));
      pixels.show();
      delay(300);
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.show();
      delay(300); 
    }
    if(B2_val){
      // blink blue TWICE for 1 sec
      for(int c=0;c<2;c++){
        pixels.setPixelColor(0, pixels.Color(0, 0, 150));
        pixels.show();
        delay(300);
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));
        pixels.show();
        delay(300); 
      }
    }
    if(B3_val){
      // blink blue TWICE for 1 sec
      for(int c=0;c<3;c++){
        pixels.setPixelColor(0, pixels.Color(0, 0, 150));
        pixels.show();
        delay(300);
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));
        pixels.show();
        delay(300); 
      }
    }
    if(B4_val){
      // blink blue TWICE for 1 sec
      for(int c=0;c<4;c++){
        pixels.setPixelColor(0, pixels.Color(0, 0, 150));
        pixels.show();
        delay(300);
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));
        pixels.show();
        delay(300); 
      }
    }
    delay(2000);  // 5 seconds between button reads
  }
}


int ReadTMP() {
  int   analogValue = analogRead(TMP_pin);

  float volts = analogValue * (3.3 / 1023.0);
  float millivolts = volts * 1000;
  float tempC = (millivolts - 500) / 10;
  float tempF = (tempC * 1.8) + 32;
  
  return round(tempF);
}

void ReadButtons(void){
  B1_val = !(digitalRead(B1_pin));
  B2_val = !(digitalRead(B2_pin));
  B3_val = !(digitalRead(B3_pin));
  B4_val = !(digitalRead(B4_pin));   
}
