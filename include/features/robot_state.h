#pragma once
#include <Arduino.h>
#include <features/detect_ball.h>

namespace robot_state {
    
    enum RobotState {
        VEILLE,
        EN_JEU,
        FIN_JEU,
    };

    RobotState current_state = VEILLE;
    uint32_t game_start_time = 0;
    uint32_t score = 0;

    void game_start() {
        current_state = EN_JEU;
        game_start_time = millis();
        score = 0;
    }

    uint32_t time_elapsed(){
        if (current_state == EN_JEU){
            return millis()-game_start_time;
        }
        else{
            return 0;
        }
    }

    void game_stop() {
        current_state = FIN_JEU;
        score = detect_ball::_time_last_ball/1000;
        Serial.println("Partie terminée");
        Serial.println("Pointage:\t");
        Serial.print(score);
    }


    void begin() {
        
    }

    void update() {
        
    }

}