#include <Arduino.h>
#include <LibRobus.h>

// #include <features/EXEMPLE_SPEED_FEATURE.h>
#include <features/robot_state.h>
#include <features/button_code.h>
#include <features/random_turret_aiming.h>
#include <features/detect_ball.h>

uint32_t _time_max = 5000; // Temps max entre les balles pour mettre fin au jeu

void setup()
{
  BoardInit();
  // my_example_speed_feature::begin();


  random_turret_aiming::begin();
  button_code::begin(13);
  detect_ball::begin();
}

auto angle_sum = 0;

void loop()
{
  button_code::update();
  if (button_code::_state == button_code::DONE and button_code::code != 0)
  {
    switch (button_code::code)
    {
    case 1:
      robot_state::game_start();
      break;
    case 2:
      robot_state::game_stop();
      break;
    }
    button_code::code = 0;
  }

  switch (robot_state::current_state)
  {
  case robot_state::VEILLE:
    // mettre ce que l'on permet au robot de faire en veille ici

    break;
  case robot_state::EN_JEU:
    // mettre ce que l'on permet au robot de faire en jeu ici
    random_turret_aiming::update();
    break;
  case robot_state::FIN_JEU:
    // mettre ce que l'on permet au robot de faire en fin de jeu ici

    break;
  }
  // turret_aiming::update();
  // // if (turret_aiming::_state == turret_aiming::DONE) {
  // //   //tire
  // //   Serial.println("Start turning");
  // //   turret_aiming::random_turret_turn(120.0);
  // // }

  // if (millis() % 1000 == 0 && turret_aiming::_state == turret_aiming::DONE)
  // {
  //   // angle_sum -= 120;
  //   Serial.println("whack");
  //   turret_aiming::random_turret_turn(120.0);
  // }

  // // Si aucune balle n'est détectée depuis le début du jeu, le moment où
  // // la dernière balle a été détectée corrrespond au moment de début du jeu
  // if (detect_ball::_time_last_ball < robot_state::game_start_time){
  //   detect_ball::_time_last_ball = robot_state::game_start_time;
  // }

  // // Si aucune balle n'est détectée, vérifie si le temps maximal
  // // est écoulé.
  // if ((robot_state::time_elapsed() - detect_ball::_time_last_ball) > _time_max)
  // {
  //     robot_state::current_state = robot_state::FIN_JEU;
  //     robot_state::game_stop();
  // }
}