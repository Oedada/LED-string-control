#include <Adafruit_NeoPixel.h>
#define countPixel 24
#define pinString 2
Adafruit_NeoPixel led(countPixel, pinString, NEO_GRB + NEO_KHZ800);
void setup() {
  Serial.begin(9600);
  Serial.println("--------------New output--------------");
  led.begin();
  led.setBrightness(100);
  led.show();
}

void breath(float t, int r, int g, int b,int repeat){
  int dl = ((float(t-0.561))/ 510.0)*1000000;
  for(int j = 0; j < repeat; j++){
    for(byte i = 0; i < 255; i++){
      led.fill(led.Color(r, g, b), 0, 24);
      led.setBrightness(i);
      led.show();
      delayMicroseconds(dl);
    }
    for(byte i = 255; i > 0; i--){
      led.fill(led.Color(r, g, b), 0, 24);
      led.setBrightness(i);
      led.show();
      delayMicroseconds(dl);
    }
  }
  led.clear();
}

void staticRainbow(bool dir){
  if(dir){
    for(byte i = 0; i < countPixel; i++){
      long hue = long((float(i)/24)*65535);
      led.setPixelColor(i, led.ColorHSV(hue, 255, 255));
    }
    led.show();
  }
  else{
    for(byte i = 0; i < countPixel; i++){
      long hue = long((float(countPixel-1-i)/24)*65535);
      Serial.print(i); Serial.print(" ");
      Serial.println(hue);
      led.setPixelColor(i, led.ColorHSV(hue, 255, 255));
    }
    led.show();
  }
}

void showAll(){
  breath(1, 50, 130, 10, 5);
  staticRainbow(true);
}

void loop() {
//  showAll();
  staticRainbow(false);
  delay(5000);
}
