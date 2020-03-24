#include <Button2.h>

#include <ESPRotary.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Adafruit_FeatherOLED.h>
#include <Adafruit_FeatherOLED_WiFi.h>

#define ROTARY_PIN1  14
#define ROTARY_PIN2 12
#define ROTARY_BUTTON 15
// OLED FeatherWing buttons map to different pins depending on board:
#if defined(ESP8266)
  #define BUTTON_A  0
  #define BUTTON_B 16
  #define BUTTON_C  2
#elif defined(ESP32)
  #define BUTTON_A 15
  #define BUTTON_B 32
  #define BUTTON_C 14
#endif

#define RELAY_PIN 3

Button2 b = Button2(ROTARY_BUTTON);
Button2 c = Button2(BUTTON_C);
ESPRotary r = ESPRotary(ROTARY_PIN1, ROTARY_PIN2, 3);

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
int o_pulses=1;
int o_sleep=10;
int o_duration=5;
int o_power=10;
int btn_delay=100;
char line1[20];
int o_mode=0;
long rotary_position=-999;
bool i_triggered;
int i_pulseidx=0;
int when_triggered=0;
const int m_pulses = 0;
const int m_duration = 1;
const int m_sleep = 2;
const int m_power = 3;

 
void setup() {
  Serial.begin(9600);
  Serial.println("setup begun");
  i_triggered=false;
  r.setChangedHandler(checkRotary);
  c.setTapHandler(doubleClick);
  b.setTapHandler(tap);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.display();
  
}

 
void loop() {
  r.loop();
  b.loop();
  c.loop();
  relayCheck();
  delay(1);
  yield();
  makeDisplay();
  
}
void doubleClick(Button2& b) {
    Serial.println("DoubleClick!  Firing!");
    when_triggered=millis();
    i_triggered=true;
    i_pulseidx=0;
}
void tap(Button2& b){
    Serial.println("Single-Click");
    if (o_mode >= 3) 
      o_mode=0;
    else
      o_mode++;
}
void checkRotary(ESPRotary &r) {
  Serial.println("detected rotation");
  long newPos = r.getPosition();
  long delta = newPos-rotary_position;
  if (delta != 0) {
    rotary_position=newPos;
  switch (o_mode) {
    case m_pulses:
    {
      o_pulses=o_pulses + delta;
      if (o_pulses > 5) o_pulses=5;
      if (o_pulses < 1) o_pulses=1;
      break;
    }
    case m_duration:
    {
      o_duration=o_duration + delta;
      if (o_duration > 1000) o_pulses=1000;
      if (o_duration < 1) o_pulses=1;
      break;
    }
    case m_sleep:
    {
      o_sleep=o_sleep + delta;
      if (o_sleep > 1000) o_sleep=1000;
      if (o_sleep < 1) o_sleep=1;
      break;
    }
    case m_power:
    {
      o_power=o_power + delta;
      if (o_power > 100) o_power=100;
      if (o_power < 1) o_power=1;
      break;      
    }
  }  
    
  }
}

void relayCheck() {
  int now=millis();
  if (i_pulseidx < o_pulses) {
    if (now-when_triggered >= o_duration+o_sleep) {
      when_triggered=millis();
      i_pulseidx++;
      if (i_pulseidx <= o_pulses) 
        i_triggered=true;
      else
        i_triggered=false;
  }
  }

  if (i_triggered) 
    if (now-when_triggered > o_duration) {
      i_triggered=false;
      digitalWrite(RELAY_PIN, LOW);
    }
    else
      digitalWrite(RELAY_PIN, HIGH);
  else
    digitalWrite(RELAY_PIN, LOW);
}

void makeDisplay() {
  char n_line1[20];
  switch (o_mode) {
    case m_pulses:
    {
      sprintf(n_line1, "Pulses: %1d", o_pulses);
      break;
    }
    case m_duration:
    {
      sprintf(n_line1, "Dur: %2d", o_duration);
      break;
    }
    case m_sleep:
    {
      sprintf(n_line1, "Sleep: %2d", o_sleep);
      break;
    }
    case m_power:
    {
      sprintf(n_line1, "Power: %3d", o_power);
      break;
    }
  }  
  if (strcmp(n_line1,line1) != 0) {
    strcpy(line1,n_line1);
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(line1);
    display.display();
  }
}
