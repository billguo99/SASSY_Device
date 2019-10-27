#include "WiFi.h"
#include <HTTPClient.h>
// OLED screen headers
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "rdm6300.h"
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define RDM6300_RX_PIN 4
#define BLUE_LED_PIN 2
#define PushButton1 32
#define PushButton2 33
#define PushButton3 14
#define PushButton4 12
#define PB1_LED 26
#define GREEN_LED 25

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Rdm6300 rdm6300;
int iCount = 0;
//uint32_t admin = 13145459;

const char* ssid = "M0UNTA1N";
const char* password =  "t9ngxuek";

WiFiUDP ntpUDP;
// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(PushButton1, INPUT);
  pinMode(PushButton2, INPUT);
  pinMode(PushButton3, INPUT);
  pinMode(PushButton4, INPUT);
  pinMode(PB1_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  digitalWrite(BLUE_LED_PIN, HIGH);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  delay(2000);
  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(PB1_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("SASSY!");
  display.display();
  display.println("Device is offline.");
  display.display();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  timeClient.begin();
  timeClient.setTimeOffset(2*3600);
  Serial.println("Connected to the WiFi network");
  display.println("Device is online.");
  display.display();
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  rdm6300.begin(RDM6300_RX_PIN);
  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(PB1_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("Waiting for admin...");
  display.display();
  uint32_t adminID = 0x0;
  uint32_t cardID = 0x0;
  uint32_t card2ID = 0x0;
  bool bAdmin = false;
  HTTPClient http;
  
  String sUsername;
  String payload;
  String sHTTPGET;
  while(!bAdmin){
    //Serial.println(tagID);
    //Serial.println(admin);
    if (rdm6300.update()){
      digitalWrite(BLUE_LED_PIN, rdm6300.is_tag_near());
      String sHTTPAdminCheck = "http://165.22.68.223/api/get_staff_activities/";
      adminID = rdm6300.get_tag_id();
      String sAdminID = String(adminID);
      sHTTPAdminCheck = sHTTPAdminCheck + sAdminID;
      Serial.println(sHTTPAdminCheck);
      http.begin(sHTTPAdminCheck); //Specify the URL
      int httpCode = http.GET();                                        //Make the request
      
      if (httpCode > 0) { //Check for the returning code
          payload = http.getString();
          sHTTPGET = payload;
          Serial.println(payload);
          int iMessage = payload.indexOf("message");
          int iUsername = payload.indexOf("username");
          String sMessage = payload.substring(iMessage+10,iUsername-3);
          Serial.println(sMessage);
          if(httpCode == 200){
            bAdmin = true;
            int iActivities = payload.indexOf("activities");
            sUsername = payload.substring(iUsername+11,iActivities-3);
          }else if(httpCode == 404){
            Serial.println("Not admin or no events.");
          }else{
            Serial.print("HTTP Code: ");
            Serial.print(httpCode);
            Serial.println();
          }
      }
      else{
          Serial.println("Error on HTTP request");
          display.println("Error on HTTP request");
          display.display();
      }
    Serial.println(bAdmin);
    }
    digitalWrite(BLUE_LED_PIN, rdm6300.is_tag_near());
    delay(200);
  }
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("Welcome ");
  display.print(sUsername);
  display.println("!");
  display.display();
  rdm6300.end();
  delay(2000);
  digitalWrite(BLUE_LED_PIN, LOW);
  // List events
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("Your events:");
  display.display();
  int iLastID = payload.lastIndexOf("\"id\"");
  int iLastCreated = payload.lastIndexOf("\"created_by\"");
  //Serial.println(payload);
  //Serial.println(iLastID);
  //Serial.println(iLastCreated);
  String sNoEvents = payload.substring(iLastID+5,iLastCreated-1);
  //Serial.println(sNoEvents);
  int iNoEvents = sNoEvents.toInt();
  Serial.println("Number of Events:");
  Serial.println(iNoEvents);
  for(int i=0;i<iNoEvents;i++){
    //Serial.println(payload);
    int iName = payload.indexOf("\"name\"");
    payload = payload.substring(iName+8);
    int iDescription = payload.indexOf("description");
    String sName = payload.substring(0, iDescription-3);
    //Serial.println(sName);
    //Serial.println(payload);
    int iStartTime = payload.indexOf("start_time");
    String sDescription = payload.substring(iDescription+14, iStartTime-3);
    payload = payload.substring(iStartTime);
    display.print(i+1);
    display.print(". ");
    display.print(sName);
    display.println();
    display.display();
  }
  Serial.println("Done with listing events.");
  http.end(); //Free the resources
  delay(2000);
  int iCount = 1;
  display.print(iCount);
  display.display();
  int PushButton1_State = digitalRead(PushButton1);
  while(PushButton1_State==LOW){
    //Serial.println("Waiting for PB1.");
    PushButton1_State = digitalRead(PushButton1);
    int PushButton3_State = digitalRead(PushButton3);
    if(PushButton3_State==HIGH){
      iCount = iCount+1;
      Serial.println(iCount);
      display.print(iCount);
      display.display();
    }
    int PushButton4_State = digitalRead(PushButton4);
    if(PushButton4_State==HIGH){
      iCount = iCount-1;
      Serial.println(iCount);
      display.print(iCount);
      display.display();
    }
    delay(200);
  }
  digitalWrite(PB1_LED, HIGH);
  String sSearch = "\"id\":";
  sSearch += String(iCount);
  //Serial.println(sSearch);
  sHTTPGET = sHTTPGET.substring(sHTTPGET.indexOf(sSearch));
  String sName = sHTTPGET.substring(sHTTPGET.indexOf("\"name\"")+8,sHTTPGET.indexOf("\"description\"")-2);
  //Serial.println(sName);
  String sDescription = sHTTPGET.substring(sHTTPGET.indexOf("\"description\"")+15,sHTTPGET.indexOf("\"start_time\"")-2);
  //Serial.println(sDescription);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("Event ");
  display.print(iCount);
  display.print(" selected.");
  display.println();
  // Display Name and Description of Event Selected
  display.print(sName);
  display.print(" - ");
  display.print(sDescription);
  display.println();
  display.println("Waiting for card...");
  display.display();
  delay(2000);
  int PushButton2_State = digitalRead(PushButton2);
  rdm6300.begin(RDM6300_RX_PIN);
  while(PushButton2_State!=HIGH){
    if (rdm6300.update()){
      digitalWrite(BLUE_LED_PIN, rdm6300.is_tag_near());
      cardID = rdm6300.get_tag_id();
      Serial.println(cardID);
      if (cardID == card2ID){
        
      }else{
        const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(8);
        DynamicJsonDocument doc(capacity);
        doc["device"] = "http://165.22.68.223/api/devices/device_1/";
        doc["activity"] = "http://165.22.68.223/api/activities/" + String(iCount) + "/";
        
        JsonObject card = doc.createNestedObject("card");
        card["card_id"] = String(cardID);
        //doc["card"] = card;
        timeClient.update();
        //Serial.println(timeClient.getFormattedDate());
        //2019-09-25T17:25:58.320131+02:00
        //YYYY-MM-DDThh:mm[:ss[.uuuuuu]][+HH:MM|-HH:MM|Z]."]
        doc["scan_time"] = timeClient.getFormattedDate();
        String message;
        serializeJson(doc, message);
        //Serial.println(message);
        
        http.begin("http://165.22.68.223/api/scans/");  //Specify destination for HTTP request
        http.addHeader("Content-Type", "application/json");             //Specify content-type header
        http.addHeader("Authorization", "Basic c2Fzc3lhZG1pbjpzYXNzeTEyMzQ1");
  
        int httpResponseCode = http.POST(message);   //Send the actual POST request
   
        if(httpResponseCode>0){
          if(httpResponseCode==201){
            String response = http.getString();                       //Get the response to the request
            Serial.println(httpResponseCode);   //Print return code
            Serial.println(response);           //Print request answer
            display.clearDisplay();
            display.setCursor(0, 10);
            display.println(cardID);
            display.println("is scanned.");
            display.display();
            digitalWrite(GREEN_LED, HIGH);
            delay(2000);
          }else{
            String response = http.getString();                       //Get the response to the request
            Serial.println(httpResponseCode);   //Print return code
            Serial.println(response);           //Print request answer
            display.clearDisplay();
            display.setCursor(0, 10);
            display.println("Error with POST request");
            display.display();
            delay(1000);
          }
        }else{
          Serial.print("Error on sending POST: ");
          Serial.println(httpResponseCode);
          display.clearDisplay();
          display.setCursor(0, 10);
          display.println("Error with POST request");
          display.display();
          delay(1000);
        }
        card2ID = cardID;
        digitalWrite(GREEN_LED, LOW);
        display.clearDisplay();
        display.setCursor(0, 10);
        display.print(sName);
        display.print(" - ");
        display.print(sDescription);
        display.println();
        display.println("Waiting for card...");
        display.println();
        display.println();
        display.println();
        display.println("Press PB2 to finish.");
        display.display();
        digitalWrite(BLUE_LED_PIN, rdm6300.is_tag_near());
        delay(1000);
      }
      
      
    }
    PushButton2_State = digitalRead(PushButton2);
    digitalWrite(BLUE_LED_PIN, rdm6300.is_tag_near());
  }
  rdm6300.end();
  http.end(); //Free the resources
}
