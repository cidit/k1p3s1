#pragma once

#include <Arduino.h>
#include <features/detect_ball.h>

namespace robot_state
{

    enum RobotState
    {
        VEILLE,
        EN_JEU,
        FIN_JEU,
    };

    RobotState current_state = VEILLE;
    uint32_t game_start_time = 0;
    uint32_t score = 0;

    void game_start()
    {
        Serial.println("Que la partie commence!");
        current_state = EN_JEU;
        game_start_time = millis();
        score = 0;
    }

    uint32_t time_elapsed()
    {
        if (current_state == EN_JEU)
        {
            return millis() - game_start_time;
        }
        else
        {
            return 0;
        }
    }

    void game_stop()
    {

        Serial.println("Partie terminée");
        Serial.print("Pointage:\t");
        score = detect_ball::_time_last_ball / 1000;
        Serial.println(score);
        current_state = FIN_JEU;
    }

    void game_sleep()
    {
        Serial.println("Stop! Le robot entre en veille");
        current_state = VEILLE;
    }

    void begin()
    {
        Serial.println("FEATURE READY : ROBOT STATE");
    }

    void update()
    {
    }

}