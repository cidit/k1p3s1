#include <Arduino.h>
#include <librobus.h>

void setup() 
{
  BoardInit();
  
}
int count;
void loop() 
{
    count = ENCODER_Read(1);
    Serial.println(count);
    delay(100);
}