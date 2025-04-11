#include <FastLED.h>
#include <TimerOne.h>

#define LED_PIN  2
#define NUM_LEDS 96
#define COLOR_ORDER GRB
#define CHIPSET     WS2811

byte brightness = 100;
byte max_brightness = 100;
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
      if(Serial.available()> 7){
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
  if(counter < max_brightness && !flag){
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), counter);
    counter++;
  }
  else if(counter > 0 && flag){
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), counter);
    counter--;
  }
  else if(counter == max_brightness && !flag){
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), counter);
    flag = 1;
  }
  else if(counter == 0 && flag){
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), counter);
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
          leds[i] = CHSV(hue, 255, brightness);
        }
      }
      else {
        for (byte i = 0; i < NUM_LEDS; i++) {
          byte hue = float(NUM_LEDS - i) / NUM_LEDS * 255;
          leds[i] = CHSV(hue, 255, brightness);
        }
      }
      FastLED.show();
    }
    void fill(int t) {
      long dl = float(t) * 1000 / 255;
      uint32_t startt = millis();
      for (byte i = 0; i < 255; i++) {
        FastLED.showColor(CHSV(i, 255, brightness));
        delay(dl);
      }
    }
    static void dynamic_gradient(){
        for(byte i = 0; i < NUM_LEDS; i++){
          if(cmd.a1 > 0){
            leds[i] = CHSV(counter + i, 255, brightness);
          }
          else{
            leds[i] = CHSV(255-(counter + i), 255, brightness);
          }
        }
        FastLED.show();
      }
};
Rainbow rainbow;


void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  Serial.begin(4800);
  Serial.println("--------------New output--------------");
  FastLED.show();
  Timer1.initialize();
}
// 99 - установить период в микросекундах тумноженных на десять(i now it's crazy)
// 98 - установить яркость
// 97 - установить максимальную яркость
void loop() {
  cmd.read();
  if(cmd.c1 == 9 && cmd.c2 == 9){
    Serial.print(cmd.c1); Serial.println(cmd.c2);
    timing = (cmd.a1*pow(16, 5)+cmd.a2*pow(16, 4)+cmd.a3*pow(16, 3)+cmd.a4*pow(16, 2)+cmd.a5*16+cmd.a6)*10;
  }
  else if(cmd.c1 == 9 && cmd.c2 == 8){
    Serial.print(cmd.c1); Serial.println(cmd.c2);
    FastLED.setBrightness(cmd.a1*16+cmd.a2);
    FastLED.show();
  }
  else if(cmd.c1 == 9 && cmd.c2 == 7){
    if(cmd.cp1 != cmd.c1 || cmd.cp2 != cmd.c2){
      Timer1.detachInterrupt();
      cmd.cp1 = cmd.c1;
      cmd.cp2 = cmd.c2; 
    }
    max_brightness = cmd.a1*16+cmd.a2;
  }
  else if(cmd.c1 == 0 && cmd.c2 == 1){
    Serial.print(cmd.c1); Serial.println(cmd.c2);
    if(cmd.cp1 != cmd.c1 || cmd.cp2 != cmd.c2){
      Timer1.detachInterrupt();
      cmd.cp1 = cmd.c1;
      cmd.cp2 = cmd.c2; 
    }
    cmd.hexToRgb();
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), brightness);
  }
  else if(cmd.c1 == 0 && cmd.c2 == 2){
    Serial.print(cmd.c1); Serial.println(cmd.c2);
    cmd.hexToRgb();
    if(cmd.cp1 != cmd.c1 || cmd.cp2 != cmd.c2){
      Timer1.detachInterrupt();
      cmd.cp1 = cmd.c1;
      cmd.cp2 = cmd.c2; 
    }
    Timer1.attachInterrupt(breath, timing/(max_brightness*2));
  }
  else if(cmd.c1 == 0 && cmd.c2 == 6){
    Serial.print(cmd.c1); Serial.println(cmd.c2);
    if(cmd.cp1 != cmd.c1 || cmd.cp2 != cmd.c2){
      Timer1.detachInterrupt();
      cmd.cp1 = cmd.c1;
      cmd.cp2 = cmd.c2; 
    }
    Timer1.attachInterrupt(rainbow.dynamic_gradient, timing/255);
  }
}
