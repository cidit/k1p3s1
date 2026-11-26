#include <Arduino.h>
#include <LibRobus.h>
//#include <features/EXEMPLE_SPEED_FEATURE.h>
//#include <features/robot_state.h>
//#include <features/button_code.h>
#include <features/detect_ball.h>

void setup() {
  BoardInit();
  Serial.begin(115200);

  detect_ball::begin();
}

void loop() {
  detect_ball::update();
}

