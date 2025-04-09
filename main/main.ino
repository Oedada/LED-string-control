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

void breath(float t, int r, int g, int b, int repeat) {
  int y = 100;
  long dl = (t * 2800) / y;
  uint32_t start = millis();
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
  Serial.println(millis() - start);
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
};

void showAll() {
  Rainbow allRainbow;
  breath(1, 50, 130, 10, 5);
  //  allRainbow.staticRainbow(true);
  //  allRainbow.fill(1);
}

void loop() {
  showAll();
}
