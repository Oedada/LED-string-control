#include <FastLED.h>

#define LED_PIN  2
#define NUM_LEDS 96
#define COLOR_ORDER GRB
#define CHIPSET     WS2811

#define BRIGHTNESS 255
CRGB leds[NUM_LEDS];
byte bleds[NUM_LEDS];

class Cmd{
  public:
    byte c1 = 0; // первый байт команды
    byte c2 = 0;// второй байт команды
    byte cp1 = 0;
    byte cp2 = 0;
    byte a1;
    byte a2;
    byte a3;
    byte a4;
    byte a5;
    byte a6;
    char string[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // массив хранящий значения комманды и её аргументов первые два байта - команда, остальные шесть - её аргументы
    char* read(){
      if(Serial.available()>= 8){
        for(byte i = 0; i < 8; i++){
          string[i] = Serial.read();
        }
        c1 = string[0];
        c2 = string[1];
        a1 = string[2];
        a2 = string[3];
        a3 = string[4];
        a4 = string[5];
        a5 = string[6];
        a6 = string[7];
      }
      return string;
    }
    
    void print(){
      for(byte i = 0; i < 8; i++){
        Serial.println(string[i]);
      }
    }
};


void breath(float t, int r, int g, int b, int repeat) {
  int y = 100;
  long dl = (t * 2800) / y;
  for (int j = 0; j < repeat; j++) {
    for (int i = 0; i < y; i += 7) {
      FastLED.showColor(CRGB(r, g, b), i);
      FastLED.show();
      delay(dl);
    }
    for (int i = y; i > 0; i -= 7) {
      FastLED.showColor(CRGB(r, g, b), i);
      FastLED.show();
      delay(dl);
    }
  }
}


void fill(byte r, byte g, byte b){
  FastLED.showColor((r, b, b), BRIGHTNESS);
}


void blinding_lights(){
  for(byte i = 0; i < NUM_LEDS; i++){
    bleds[i] = random(0, 2);
    Serial.println(bleds[i]);
  }
  Serial.println("---ooo---");
  for(byte i = 0; i < NUM_LEDS; i++){
    Serial.println(bleds[i]);
    if(bleds[i] == 1){
      leds[i] = CRGB(0, 255, 0);
    }
    else{
      leds[i] = 0;
    }
  }
  delay(1000);
 FastLED.show();
}

class Rainbow {
  public:
    void gradient(bool dir) {
      if (dir) {
        for (byte i = 0; i < NUM_LEDS; i++) {
          byte hue = float(i) / NUM_LEDS * 255;
          leds[i] = CHSV(hue, 255, BRIGHTNESS);
        }
      }
      else {
        for (byte i = 0; i < NUM_LEDS; i++) {
          byte hue = float(NUM_LEDS - i) / NUM_LEDS * 255;
          leds[i] = CHSV(hue, 255, BRIGHTNESS);
        }
      }
      FastLED.show();
    }
    void fill(int t) {
      long dl = float(t) * 1000 / 255;
      uint32_t startt = millis();
      for (byte i = 0; i < 255; i++) {
        FastLED.showColor(CHSV(i, 255, BRIGHTNESS));
        delay(dl);
      }
    }
    void dynamic_gradient(bool dir){
      for(byte j = 0; j < 255; j++){
        for(byte i = 0; i < NUM_LEDS; i++){
          if(dir){
            leds[i] = CHSV(j + i, 255, BRIGHTNESS);
          }
          else{
            leds[i] = CHSV(255-(j + i), 255, BRIGHTNESS);
          }
        }
        FastLED.show();
        delay(10);
      }
    }
};

Rainbow allRainbow;
Cmd cmd;

void showAll() {
//  fill(0, 255, 0);
//  breath(1, 50, 130, 10, 5);
//  blinding_lights();
//  allRainbow.gradient(true);
//  allRainbow.fill(5);
  allRainbow.dynamic_gradient(0);
}

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  Serial.begin(9600);
  Serial.println("--------------New output--------------");
  FastLED.show();
}

void loop() {
//  showAll();
  cmd.read();
  delay(10);
  if(cmd.cp1 != cmd.c1 && cmd.cp2 != cmd.c2 && Serial.availableForWrite() > 0){
    cmd.print();
    cmd.cp1 = cmd.c1;
    cmd.cp2 = cmd.c2;
  }
  if(cmd.c1 == 48 && cmd.c2 == 54 && cmd.a1 == 48){
    allRainbow.dynamic_gradient(0);
  }
  else if(cmd.c1 == 48 && cmd.c2 == 54 && cmd.a1 == 49){
    allRainbow.dynamic_gradient(1);
  }
}
