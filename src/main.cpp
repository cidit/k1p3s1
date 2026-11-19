#include <Arduino.h>
#include <LibRobus.h>
#include <features/EXEMPLE_SPEED_FEATURE.h>

void setup() {
  BoardInit();
  Serial.begin(115200);
  my_example_speed_feature::begin();
}

void loop() {
  my_example_speed_feature::update();
}

