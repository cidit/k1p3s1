#include <Arduino.h>
#include <LibRobus.h>
// #include <features/EXEMPLE_SPEED_FEATURE.h>
#include <features/robot_state.h>
// #include <features/button_code.h>
#include <features/turret_aiming.h>

void setup()
{
  BoardInit();
  Serial.begin(115200);
  // my_example_speed_feature::begin();

  // robot_state::begin();
  turret_aiming::begin();

  // turret_aiming::rotate_to(-120);
}

auto angle_sum = 0;

void loop()
{
  turret_aiming::update();
  // if (turret_aiming::_state == turret_aiming::DONE) {
  //   //tire
  //   Serial.println("Start turning");
  //   turret_aiming::random_turret_turn(120.0);
  // }

  

  if (millis() % 1000 == 0 && turret_aiming::_state == turret_aiming::DONE)
  {
    // angle_sum -= 120;
    Serial.println("whack");
    turret_aiming::random_turret_turn(120.0);
  }
}
