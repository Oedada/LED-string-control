#include <FastLED.h>

#define LED_PIN  2
#define NUM_LEDS 96
#define COLOR_ORDER GRB
#define CHIPSET     WS2811

#define BRIGHTNESS 255
CRGB leds[NUM_LEDS];
byte bleds[NUM_LEDS];



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


class Cmd{
  public:
    byte c1 = 0; // первый байт команды
    byte c2 = 0;// второй байт команды
    byte cp1 = 0;
    byte cp2 = 0;
    byte a1 = 0;
    byte a2 = 0;
    byte a3 = 0;
    byte a4 = 0;
    byte a5 = 0;
    byte a6 = 0;
    byte r = 0;
    byte g = 0;
    byte b = 0;
    char string[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // массив хранящий значения комманды и её аргументов первые два байта - команда, остальные шесть - её аргументы

    byte ascii(byte bt){
      if(bt == 48) return 0;
      else if(bt == 49) return 1;
      else if(bt == 50) return 2;
      else if(bt == 51) return 3;
      else if(bt == 52) return 4;
      else if(bt == 53) return 5;
      else if(bt == 54) return 6;
      else if(bt == 55) return 7;
      else if(bt == 56) return 8;
      else if(bt == 57) return 9;
      else if(bt == 65) return 10;
      else if(bt == 66) return 11;
      else if(bt == 67) return 12;
      else if(bt == 68) return 13;
      else if(bt == 69) return 14;
      else if(bt == 70) return 15;
    }
    
    char* read(){
//      if(Serial.available()==8){
//        for(byte i = 0; i < 8; i++){
//          Serial.print(Serial.read());
//        }
//        Serial.println();
//      }
      if(Serial.available()== 8){
        for(byte i = 0; i < 8; i++){
          string[i] = Serial.read();
        }
      }
      c1 = ascii(string[0]);
      c2 = ascii(string[1]);
      a1 = ascii(string[2]);
      a2 = ascii(string[3]);
      a3 = ascii(string[4]);
      a4 = ascii(string[5]);
      a5 = ascii(string[6]);
      a6 = ascii(string[7]);
    }
    
    void print(){
      for(byte i = 0; i < 8; i++){
        Serial.print(string[i]);
      }
    }
    
    void hexToRgb(){
      r = ascii(a1)*16 + ascii(a2);
      g = ascii(a3)*16 + ascii(a4);
      b = ascii(a5)*16 + ascii(a6);
    }
    
};


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
  Serial.begin(115200);
  Serial.println("--------------New output--------------");
  FastLED.show();
}

void loop() {
  cmd.read();
//  if(cmd.cp1 != cmd.c1 || cmd.cp2 != cmd.c2){
//    cmd.print();
//    cmd.cp1 = cmd.c1;
//    cmd.cp2 = cmd.c2;
//  }
//  Serial.println("uuu");
//  Serial.println(cmd.c1);
//  Serial.println(cmd.c2);
  if(cmd.c1 == 0 && cmd.c2 == 1){
    cmd.hexToRgb();
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), BRIGHTNESS);
  }
  
  if(cmd.c1 == 0 && cmd.c2 == 6 && cmd.a1 == 0){
    allRainbow.dynamic_gradient(0);
  }
  else if(cmd.c1 == 0 && cmd.c2 == 6 && cmd.a1 == 1){
    allRainbow.dynamic_gradient(1);
  }

}
