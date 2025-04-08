#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel led(24, 2, NEO_GRB + NEO_KHZ800);
void setup() {
  Serial.begin(9600);
  led.begin();
  led.setBrightness(100);
  led.show();
}

void wave(float t, int r, int g, int b,int repeat){
  int dl = ((float) t / 510)*1000000;
  Serial.println(dl);
  for(int j = 0; j < repeat; j++){
    for(byte i = 0; i < 255; i++){
      led.fill(led.Color(r, g, b), 0, 24);
      led.show();
      Serial.println(r);
      delayMicroseconds(dl);
    }
    for(byte i = 255; i > 0; i--){
      r = constrain(r-i, 0, 255);
      g = constrain(g-i, 0, 255);
      b = constrain(b-i, 0, 255);
      led.fill(led.Color(r, g, b), 0, 24);
      led.show();
      delayMicroseconds(dl);
    }
  }
}

void loop() {
  wave(400, 255, 0, 0, 5);
  delay(5000);
}
