#include "WiFi.h"
// OLED screen headers
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "rdm6300.h"

#define RDM6300_RX_PIN 4
#define BLUE_LED_PIN 2
#define PushButton1 15
#define PushButton2 27
#define PB1_LED 26

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Rdm6300 rdm6300;
int iCount = 0;
uint32_t admin = 13145459;

const char* ssid = "M0UNTA1N";
const char* password =  "t9ngxuek";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(PushButton1, INPUT);
  pinMode(PushButton2, INPUT);
  pinMode(PB1_LED, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  digitalWrite(BLUE_LED_PIN, HIGH);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  delay(2000);
  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(PB1_LED, LOW);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("SASSY!");
  display.display();
  //display.println("Device is offline.");
  //display.display();
  //WiFi.begin(ssid, password);
  //while (WiFi.status() != WL_CONNECTED) {
    //delay(500);
    //Serial.println("Connecting to WiFi..");
  //}
  
  //Serial.println("Connected to the WiFi network");
  display.println("Device is connected (online).");
  display.display();
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  rdm6300.begin(RDM6300_RX_PIN);
  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(PB1_LED, LOW);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("Waiting for admin...");
  display.display();
  uint32_t adminID = 0x0;
  uint32_t cardID = 0x0;
  bool bAdmin = false;
  while(!bAdmin){
    //Serial.println(tagID);
    //Serial.println(admin);
    if (rdm6300.update()){
    adminID = rdm6300.get_tag_id();
    //Serial.println(tagID, HEX);
    //Serial.println(tagID, DEC);
    if(adminID==admin){
      bAdmin = true;
    }
    //Serial.println(tagID);
    //Serial.println(admin);
    }
    digitalWrite(BLUE_LED_PIN, rdm6300.is_tag_near());
  }

  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("Welcome admin!");
  display.display();
  rdm6300.end();
  delay(2000);
  digitalWrite(BLUE_LED_PIN, LOW);
  int PushButton1_State = digitalRead(PushButton1);
  // List events
  display.println("Your events:");
  display.println("1. Test 1 - CSC3022H");
  display.println("2. Prac 4 - EEE4018F");
  display.display();
  delay(2000);
  while(PushButton1_State==LOW){
    //Serial.println("Waiting for PB1.");
    PushButton1_State = digitalRead(PushButton1);
  }
  digitalWrite(PB1_LED, HIGH);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("Event 1 selected.");
  display.println("Test 1 - CSC3022H");
  display.println("Waiting for card...");
  display.display();
  delay(2000);
  int PushButton2_State = digitalRead(PushButton2);
  rdm6300.begin(RDM6300_RX_PIN);
  while(PushButton2_State!=HIGH){
    if (rdm6300.update()){
      digitalWrite(BLUE_LED_PIN, rdm6300.is_tag_near());
      cardID = rdm6300.get_tag_id();
      if(adminID==cardID){
        continue;
      }
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println(cardID);
      display.println("is scanned.");
      display.display();
      delay(2000);
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("Waiting for card to be scanned.");
      display.println("Press PB2 to finish event.");
      display.display();
      digitalWrite(BLUE_LED_PIN, rdm6300.is_tag_near());
      delay(1000);
    }
    PushButton2_State = digitalRead(PushButton2);
    adminID = cardID;
    digitalWrite(BLUE_LED_PIN, rdm6300.is_tag_near());
  }
  rdm6300.end();
}
