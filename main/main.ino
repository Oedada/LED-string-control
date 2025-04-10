#include <FastLED.h>

#define LED_PIN  2
#define NUM_LEDS 96
#define COLOR_ORDER GRB
#define CHIPSET     WS2811

#define BRIGHTNESS 100
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
    void dynamic_gradient(){
      for(byte j = 0; j < 255; j++){
        for(byte i = 0; i < NUM_LEDS; i++){
          leds[i] = CHSV(j + i, 255, BRIGHTNESS);
        }
        FastLED.show();
        delay(10);
      }
    }
};


void showAll() {
    Rainbow allRainbow;
//  fill(0, 255, 155);
//  breath(1, 50, 130, 10, 5);
//  blinding_lights();
//  allRainbow.gradient(true);
//  allRainbow.fill(1);
  allRainbow.dynamic_gradient();
}


void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  Serial.begin(115200);
  Serial.println("--------------New output--------------");
  FastLED.show();
//  fill(255, 255, 255);
}

void loop() {
  showAll();
}
