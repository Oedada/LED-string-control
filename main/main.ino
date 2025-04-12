#include <FastLED.h>
#include <TimerOne.h>

#define LED_PIN  2
#define NUM_LEDS 96
#define COLOR_ORDER GRB
#define CHIPSET     WS2811

byte brightness = 255;
byte max_brightness = 100;
CRGB leds[NUM_LEDS];
byte bleds[NUM_LEDS];
char digits[16] = "0123456789ABCDEF";
long timing;
bool flag;

class Cmd {
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
    byte counter = 0;
    // массив хранящий значения комманды и её аргументов первые два байта - команда, остальные шесть - её аргументы

    byte ascii() {
      byte bt = Serial.read();
      for (byte i = 0; i < 16; i++) {
        if (bt == digits[i]) {
          return i;
        }
      }
      return 0;
    }

    char* read() {
      if (Serial.available() > 7) {
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

    void hexToRgb() {
      r = a1 * 16 + a2;
      g = a3 * 16 + a4;
      b = a5 * 16 + a6;
    }

};
Cmd cmd;



void breath() {
  if (cmd.counter < max_brightness && !flag) {
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), cmd.counter);
    cmd.counter++;
  }
  else if (cmd.counter > 0 && flag) {
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), cmd.counter);
    cmd.counter--;
  }
  else if (cmd.counter == max_brightness && !flag) {
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), cmd.counter);
    flag = 1;
  }
  else if (cmd.counter == 0 && flag) {
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), cmd.counter);
    flag = 0;
  }
}


void blinding_lights() {
  for (byte i = 0; i < NUM_LEDS; i++) {
    bleds[i] = random(0, 2);
    Serial.println(bleds[i]);
  }
  Serial.println("---ooo---");
  for (byte i = 0; i < NUM_LEDS; i++) {
    Serial.println(bleds[i]);
    if (bleds[i] == 1) {
      leds[i] = CRGB(0, 255, 0);
    }
    else {
      leds[i] = 0;
    }
  }
  delay(1000);
  FastLED.show();
}


class Rainbow {
  public:
    void gradient(bool dir) {
      for (byte i = 0; i < NUM_LEDS; i++) {
        byte hue;
        if (dir) {
          hue = float(i) / NUM_LEDS * 255;
        }
        else {
          hue = float(NUM_LEDS - i) / NUM_LEDS * 255;
        }
        leds[i] = CHSV(hue, 255, brightness);
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

    static void dynamic_gradient() {
      Serial.println("ok");
      if (cmd.a1 > 0) {
        for (byte i = 0; i < NUM_LEDS; i++) {
          byte hue = float(i + cmd.counter) / NUM_LEDS * 255;
          leds[i] = CHSV(hue, 255, brightness);
        }
      }
      else{
        for (byte i = 0; i < NUM_LEDS; i++) {
          byte hue = float(NUM_LEDS - (i + cmd.counter)) / NUM_LEDS * 255;
          leds[i] = CHSV(hue, 255, brightness);
        }
      }
      if(cmd.counter < 255){
        cmd.counter++;
      }
      else{
        cmd.counter = 0;
      }
      FastLED.show();
    }
};
Rainbow rainbow;


void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  Serial.begin(115200);
  Serial.println("--------------New output--------------");
  FastLED.show();
  Timer1.initialize();
}
// 99 - установить период в микросекундах тумноженных на десять(i know it's crazy)
// 98 - установить яркость
// 97 - установить максимальную яркость
// 01 - заливка ленты одним цветом
// 02 - режим дыхания
// 03 - динамический градиент
//
//
void loop() {
  cmd.read();
  // настройки
  if (cmd.c1 == 9 && cmd.c2 == 9) {
    if (cmd.cp1 != cmd.c1 || cmd.cp2 != cmd.c2) {
      Serial.println("warn");
      timing = (cmd.a1 * pow(16, 5) + cmd.a2 * pow(16, 4) + cmd.a3 * pow(16, 3) + cmd.a4 * pow(16, 2) + cmd.a5 * 16 + cmd.a6) * 10;
      cmd.cp1 = cmd.c1;
      cmd.cp2 = cmd.c2;
    }
  }

  else if (cmd.c1 == 9 && cmd.c2 == 8) {
    FastLED.setBrightness(cmd.a1 * 16 + cmd.a2);
    FastLED.show();
  }

  else if (cmd.c1 == 9 && cmd.c2 == 7) {
    if (cmd.cp1 != cmd.c1 || cmd.cp2 != cmd.c2) {
      Timer1.detachInterrupt();
      cmd.cp1 = cmd.c1;
      cmd.cp2 = cmd.c2;
    }
    max_brightness = cmd.a1 * 16 + cmd.a2;
  }
  // режимы
  // статические
  else if (cmd.c1 == 0 && cmd.c2 == 1) {
    if (cmd.cp1 != cmd.c1 || cmd.cp2 != cmd.c2) {
      Timer1.detachInterrupt();
      cmd.cp1 = cmd.c1;
      cmd.cp2 = cmd.c2;
    }
    cmd.hexToRgb();
    FastLED.showColor(CRGB(cmd.r, cmd.g, cmd.b), brightness);
  }

  else if (cmd.c1 == 0 && cmd.c2 == 4) {
    if (cmd.a1 > 0) {
      rainbow.gradient(1);
    }
    else {
      rainbow.gradient(0);
    }
  }
  // динамические
  else if (cmd.c1 == 0 && cmd.c2 == 2) {
    cmd.hexToRgb();
    if (cmd.cp1 != cmd.c1 || cmd.cp2 != cmd.c2) {
      Timer1.detachInterrupt();
      cmd.cp1 = cmd.c1;
      cmd.cp2 = cmd.c2;
      cmd.counter = 0;
      Timer1.attachInterrupt(breath, timing / (max_brightness * 2));
    }
  }

  else if (cmd.c1 == 0 && cmd.c2 == 3) {
    if (cmd.cp1 != cmd.c1 || cmd.cp2 != cmd.c2) {
      Timer1.stop();
      Timer1.detachInterrupt();
      cmd.cp1 = cmd.c1;
      cmd.cp2 = cmd.c2;
      cmd.counter = 0;
      Timer1.attachInterrupt(Rainbow.dynamic_gradient, 10000);
      Timer1.start();
    }
  }
}
