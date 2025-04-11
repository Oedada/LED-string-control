#include <FastLED.h>
#include <TimerOne.h>

#define LED_PIN  2
#define NUM_LEDS 6
#define COLOR_ORDER GRB
#define CHIPSET     WS2811

#define BRIGHTNESS 100
#define MAX_BRIGHTNESS 100
CRGB leds[NUM_LEDS];
byte bleds[NUM_LEDS];
char digits[16] = "0123456789ABCDEF";
long timing;
bool flag;
byte counter;

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
    // массив хранящий значения комманды и её аргументов первые два байта - команда, остальные шесть - её аргументы

    byte ascii(){
      byte bt = Serial.read();
      for(byte i = 0; i <16; i++){
        if (bt == digits[i]){
          return i;
        }
      }
      return 0;
    }
    
    char* read(){
      if(Serial.available()== 8){
        c1 = ascii();
        c2 = ascii();
        a1 = ascii();
        a2 = ascii();
        a3 = ascii();
        a4 = ascii();
        a5 = ascii();
        a6 = ascii();
      }
    }
    
//    void print(){
//      for(byte i = 0; i < 8; i++){
//        Serial.print(string[i]);
//      }
//    }
    
    void hexToRgb(){
      r = a1*16 + a2;
      g = a3*16 + a4;
      b = a5*16 + a6;
    }
    
};
Cmd cmd;



void breath() {
  if(counter < 255 && !flag){
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), counter);
    FastLED.show();
    counter++;
  }
  else if(counter > 0 && flag){
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), counter);
    FastLED.show();
    counter--;
  }
  else if(counter == 255 && !flag){
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), counter);
    FastLED.show();
    flag = 1;
  }
  else if(counter == 0 && flag){
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), counter);
    FastLED.show();
    flag = 0;
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
  Timer1.initialize();
}

void loop() {
  cmd.read();
  if(cmd.c1 == 9 && cmd.c2 == 9){
    timing = (cmd.a1*(16**5)+cmd.a2*(16**4)+cmd.a3*(16**3)+cmd.a4*(16**2)+cmd.a5*16+cmd.a6)*10;
  }
  else if(cmd.c1 == 0 && cmd.c2 == 1){
    cmd.hexToRgb();
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), BRIGHTNESS);
  }
  else if(cmd.c1 == 0 && cmd.c2 == 2){
    cmd.hexToRgb();
    Timer1.detachInterrupt()
    Timer1.attachInterrupt(breath, timing/(MAX_BRIGHTNESS*2));
  }
  else if(cmd.c1 == 0 && cmd.c2 == 6 && cmd.a1 == 0){
    allRainbow.dynamic_gradient(0);
  }
  else if(cmd.c1 == 0 && cmd.c2 == 6 && cmd.a1 == 1){
    allRainbow.dynamic_gradient(1);
  }

}
