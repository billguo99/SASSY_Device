/*
 * A simple example to interface with rdm6300 rfid reader.
 *
 * Connect the rdm6300 to VCC=5V, GND=GND, TX=any GPIO (this case GPIO-04)
 * Note that the rdm6300's TX line is 3.3V level,
 * so it's save to use with both AVR* and ESP* microcontrollers.
 *
 * Arad Eizen (https://github.com/arduino12).
 */

#include "rdm6300.h"

#define RDM6300_RX_PIN 4
#define READ_LED_PIN 2

Rdm6300 rdm6300;
int iCount = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(READ_LED_PIN, OUTPUT);
  digitalWrite(READ_LED_PIN, LOW);
  
  rdm6300.begin(RDM6300_RX_PIN);

  Serial.println("\nPlace RFID tag near the rdm6300...");
}

void loop()
{
  /* if non-zero tag_id, update() returns true- a new tag is near! */
  if (rdm6300.update()){
    iCount = iCount + 1;
    Serial.print("Card no: ");
    Serial.print(iCount);
    Serial.print("\n");
    uint32_t tagID = rdm6300.get_tag_id();
    Serial.println(tagID, HEX);
    Serial.println(tagID, DEC);
  }

  digitalWrite(READ_LED_PIN, rdm6300.is_tag_near());

  delay(10);
}