/*
  ESP 32 Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
  The ESP32 has an internal blue LED at D2 (GPIO 02)
*/

int LED = 2;
int RED_BUILTIN = 26;
int GREEN_BUILTIN = 25;

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RED_BUILTIN, OUTPUT);
  pinMode(GREEN_BUILTIN, OUTPUT);
}

void loop() 
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(RED_BUILTIN, HIGH);
  digitalWrite(GREEN_BUILTIN, HIGH);
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(RED_BUILTIN, LOW);
  digitalWrite(GREEN_BUILTIN, LOW);
  delay(1000);                       // wait for a second
}
