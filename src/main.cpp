#include <Arduino.h>
#include <LibRobus.h>
// #include <features/EXEMPLE_SPEED_FEATURE.h>
#include <features/robot_state.h>
// #include <features/button_code.h>
#include <features/turret_aiming.h>
#include <features/detect_ball.h>

uint32_t _time_max = 5000; //Temps max entre les balles pour mettre fin au jeu

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

  // Si aucune balle n'est détectée depuis le début du jeu, le moment où
  // la dernière balle a été détectée corrrespond au moment de début du jeu
  if (detect_ball::_time_last_ball < robot_state::game_start_time){
    detect_ball::_time_last_ball = robot_state::game_start_time;
  }

  // Si aucune balle n'est détectée, vérifie si le temps maximal
  // est écoulé.
  if ((robot_state::time_elapsed() - detect_ball::_time_last_ball) > _time_max)
  {
      robot_state::current_state = robot_state::FIN_JEU;
      robot_state::game_stop();
  }
}