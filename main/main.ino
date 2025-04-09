#include <FastLED.h>

#define LED_PIN  2
#define NUM_LEDS 30
#define COLOR_ORDER GRB
#define CHIPSET     WS2811

#define BRIGHTNESS 120
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  Serial.begin(115200);
  Serial.println("--------------New output--------------");
  FastLED.show();
}

void breath(float t, int r, int g, int b,int repeat){
  uint32_t startt = millis();
  int y = 255;
  long dl = t/(y*2);
  for(int j = 0; j < repeat; j++){
    for(int i = 0; i < y; i += 5){
      FastLED.delay(dl);
      FastLED.showColor(CRGB(r, g, b), i);
      FastLED.show();
    }
    for(int i = y; i > 0; i -= 5){
      FastLED.delay(dl);
      FastLED.showColor(CRGB(r, g, b), i);
      FastLED.show();
    }
  }
  Serial.println(millis() - startt);
}

void staticRainbow(bool dir){
    if(dir){
      for(byte i = 0; i < NUM_LEDS; i++){
        byte hue = float(i)/NUM_LEDS*191;
        leds[i] = CHSV(hue, 255, BRIGHTNESS);
      }
    }
    else{
      for(byte i = 0; i < NUM_LEDS; i++){
        byte hue = float(NUM_LEDS-i)/NUM_LEDS*191;
        leds[i] = CHSV(hue, 255, BRIGHTNESS);
      }
    }
    FastLED.show();
}

//void showAll(){
//  breath(1, 50, 130, 10, 5);
//  staticRainbow(true);
//}

void loop() {
  staticRainbow(0);
}
