#pragma once
#include <Arduino.h>

namespace robot_state {
    
    enum RobotState {
        VEILLE,
        EN_JEU,
    };

    RobotState current_state = VEILLE;

    void start() {
        current_state = EN_JEU;
    }

    void begin() {
        
    }

    void update() {
        
    }

}