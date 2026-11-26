#pragma once
#include <Arduino.h>

namespace robot_state {
    
    enum RobotState {
        VEILLE,
        EN_JEU,
        FIN_JEU,
    };

    RobotState current_state = VEILLE;
    uint32_t game_start_time = 0;

    void game_start() {
        current_state = EN_JEU;
        game_start_time = millis();
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
    }


    void begin() {
        
    }

    void update() {
        
    }

}