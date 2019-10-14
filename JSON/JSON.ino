#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
 
const char* ssid = "M0UNTA1N";
const char* password =  "t9ngxuek";

WiFiUDP ntpUDP;
// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP);
 
void setup() {
 
  Serial.begin(115200);
  delay(4000);   //Delay needed before calling the WiFi.begin

  WiFi.begin(ssid, password); 
 
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  timeClient.begin();
  timeClient.setTimeOffset(2*3600);
  Serial.println("Connected to the WiFi network");
 
}
 
void loop() {
 
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    
   HTTPClient http;   
   const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(4);
    DynamicJsonDocument doc(capacity);
    
    doc["device"] = "http://165.22.68.223/api/devices/device_1/";
    doc["activity"] = "http://165.22.68.223/api/activities/1/";
    JsonObject card = doc.createNestedObject("card");
    card["card_id"] = "test007";
    timeClient.update();
    Serial.println(timeClient.getFormattedTime());
    //2019-09-25T17:25:58.320131+02:00
    //YYYY-MM-DDThh:mm[:ss[.uuuuuu]][+HH:MM|-HH:MM|Z]."]
    doc["scan_time"] = timeClient.getFormattedTime();
    String message;
    serializeJson(doc, message);
    Serial.println();
    delay(10000);
   http.begin("http://165.22.68.223/api/scans/");  //Specify destination for HTTP request
   http.addHeader("Content-Type", "application/json");             //Specify content-type header
   http.addHeader("Authorization", "Basic c2Fzc3lhZG1pbjpzYXNzeTEyMzQ1");
 
   int httpResponseCode = http.POST(message);   //Send the actual POST request
 
   if(httpResponseCode>0){
 
    String response = http.getString();                       //Get the response to the request
 
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
 
   }else{
 
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
 
   }
 
   http.end();  //Free resources
 
 }else{
 
    Serial.println("Error in WiFi connection");   
 
 }
 
  delay(10000);  //Send a request every 10 seconds
 
}
