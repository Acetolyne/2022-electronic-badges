//Required Libraries:
//
// Adafruit NeoPixel 
//
// GFX Library for Arduino (by Moon On Our Nation)
//    available by searching Manage Libraries
//

#include <Arduino_GFX_Library.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

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

#define LED_count 8

// pins 0-4 connect to the GC9A01 circular display like so:
Arduino_DataBus *bus = new Arduino_SWSPI(3, 4, 0, 1, -1); //DC, CS, SCK, MOSI, MISO
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 2, 0, true); //bus, RESET, rotation, IPS

Adafruit_NeoPixel RGB_LEDs(LED_count, LED_pin, NEO_GRB + NEO_KHZ800);

//MPU-6050 (GY-25 module) 3-axis MEMS gyro + 3-axis MEMS accelerometer
const int MPUadr=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ,CUR_color; //MPU Values

int B1_val, B2_val, B3_val, B4_val;
int LDR_val, TMP_val, CUR_input; 

int Shot_val; // Shot Noise from simple Random Noise Generator (RNG) circuit

void setup(void)
{
  analogReadResolution(12);  //increase analog input resolution to 12 bits  [0..4095]
  RGB_LEDs.begin();
  cycleLEDs();
  StartMPU();

  //button pins needs pullups since they close directly to ground (active low)
  pinMode(B1_pin, INPUT_PULLUP);
  pinMode(B2_pin, INPUT_PULLUP);
  pinMode(B3_pin, INPUT_PULLUP);
  pinMode(B4_pin, INPUT_PULLUP);
  pinMode(LDR_pin, INPUT);
  pinMode(Shot_pin, INPUT);
  pinMode(TMP_pin, INPUT);
  
  gfx->begin();
  gfx->fillScreen(BLACK);

  //draw a bunch of concentric circles - just for fun
  for(int r=1; r<120; r++)
    gfx->drawCircle(120, 120, r, random(0xffff));
  
  delay(2000);

  gfx->fillScreen(BLACK);
  gfx->drawCircle(120, 120, 110, RED);
  CUR_input = 0;
}

void loop()
{
  char strBuf[20];

  ReadButtons();  

  CUR_color = RGB_LEDs.Color(random(255),random(255),random(255));

  if(B1_val){
    //color LED 7
    RGB_LEDs.setPixelColor(7, RGB_LEDs.Color(40, 0, 40));  //purple
  } else {
    RGB_LEDs.setPixelColor(7, CUR_color);     //green
  }
  if(B2_val){
    RGB_LEDs.setPixelColor(6, RGB_LEDs.Color(40, 0, 40));  //purple
  } else {
    RGB_LEDs.setPixelColor(6, CUR_color);     //green
  }
  if(B3_val){
    //color LED 4
    RGB_LEDs.setPixelColor(4, RGB_LEDs.Color(40, 0, 40));  //purple
  } else {
    RGB_LEDs.setPixelColor(4, CUR_color);     //green
  }
  if(B4_val){
    //color LED 3
    RGB_LEDs.setPixelColor(3, RGB_LEDs.Color(40, 0, 40));  //purple
  } else {
    RGB_LEDs.setPixelColor(3, CUR_color);     //green
  }
  RGB_LEDs.setPixelColor(1, CUR_color);
  RGB_LEDs.setPixelColor(2, CUR_color);
  RGB_LEDs.setPixelColor(5, CUR_color);
  RGB_LEDs.setPixelColor(0, CUR_color);
  RGB_LEDs.show();

  ReadNoise();

  gfx->setTextSize(4);
  gfx->setTextColor(CUR_color, BLACK);

  gfx->setCursor(60, 100);
  sprintf(strBuf, "Defcon");
  gfx->println(strBuf);    

  gfx->setCursor(100, 140);
  sprintf(strBuf, "30");
  gfx->println(strBuf);
//   
//  gfx->setCursor(60, 100);
//  sprintf(strBuf, "Shot: %04d", Shot_val);
//  gfx->println(strBuf);
//
//  gfx->setCursor(60, 120);
//  sprintf(strBuf, "GyrX: %04d", (GyX&0x0FFF));  //lowest 12 bits
//  gfx->println(strBuf);
//
//  gfx->setCursor(60, 140);
//  sprintf(strBuf, "GyrY: %04d", (GyY&0x0FFF));  //lowest 12 bits
//  gfx->println(strBuf);
//
//  gfx->setCursor(60, 160);
//  sprintf(strBuf, "GyrZ: %04d", (GyZ&0x0FFF));  //lowest 12 bits
//  gfx->println(strBuf);
  
  delay(3000);
}

void ReadButtons(void){
  B1_val = !(digitalRead(B1_pin));
  B2_val = !(digitalRead(B2_pin));
  B3_val = !(digitalRead(B3_pin));
  B4_val = !(digitalRead(B4_pin));   
}

void ReadNoise(void)
{
  ReadMPU();
  LDR_val  = analogRead(LDR_pin);
  Shot_val = analogRead(Shot_pin);
  TMP_val = ReadTemp();
}

void cycleLEDs() {
  int i;
  for (i=0; i<8; i++){
    RGB_LEDs.setPixelColor(i, RGB_LEDs.Color(40, 0, 0));  //light red
    RGB_LEDs.show();
    delay(300);
  }
//  for (i=0; i<8; i++){
//    RGB_LEDs.setPixelColor(i, RGB_LEDs.Color(0,10,0));  //light green
//    RGB_LEDs.show();
//  }
}

void StartMPU(){
  Wire1.setSDA(MPU_SDA);
  Wire1.setSCL(MPU_SCL);
  Wire1.begin();

  Wire1.beginTransmission(MPUadr);
  Wire1.write(0x6B); 
  Wire1.write(0);    
  Wire1.endTransmission(true);
}

void ReadMPU(){
  Wire1.beginTransmission(MPUadr);
  Wire1.write(0x3B);  
  Wire1.endTransmission(false);
  Wire1.requestFrom(MPUadr,12,true);  
  AcX=Wire1.read()<<8|Wire1.read();    
  AcY=Wire1.read()<<8|Wire1.read();  
  AcZ=Wire1.read()<<8|Wire1.read();  
  GyX=Wire1.read()<<8|Wire1.read();  
  GyY=Wire1.read()<<8|Wire1.read();  
  GyZ=Wire1.read()<<8|Wire1.read();  
}

int ReadTemp() {
  int   analogValue = analogRead(TMP_pin);
  return analogValue;
/*
  // this returns raw 12 bits for most resolution [0..4095]
  // since that is best as a noise source
  // but for demo, you could convert it to degrees C or F
  float volts = analogValue * (3.3 / 4095.0);
  float millivolts = volts * 1000;
  float tempC = (millivolts - 500) / 10;
  float tempF = (tempC * 1.8) + 32;
  return round(tempF);
*/
}
