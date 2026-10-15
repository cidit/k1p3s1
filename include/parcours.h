#include "fonctions.h"
#include <LibRobus.h>

//if (x % 2) x is odd
int direction = 0; // 0 = N; 1 = O; 2 = S; 3 = E
int position[2] = {0,1};
int murs[10][3][4] = {
    {{0,1,1,0}, {1,0,1,0}, {0,0,1,1}},
    {{0,1,0,1}, {1,1,1,1}, {0,1,0,1}},
    {{0,1,0,0}, {1,0,1,0}, {0,0,0,1}},
    {{0,1,0,1}, {1,1,1,1}, {0,1,0,1}},
    {{0,1,0,0}, {0,0,1,0}, {0,0,0,1}},
    {{0,1,0,1}, {0,1,0,1}, {0,1,0,1}},
    {{0,1,0,0}, {1,0,0,0}, {0,0,0,1}},
    {{0,1,0,1}, {1,1,1,1}, {0,1,0,1}},
    {{0,1,0,0}, {0,0,0,0}, {0,0,0,1}},
    {{1,1,0,0}, {1,1,1,1}, {1,0,0,1}},
};

void turn(int direction, int futur_dir){
    int nb_turn = direction - futur_dir;
    if (abs(nb_turn) != nb_turn){
        if (nb_turn == 1){
            turn_right();
        }
        else if (nb_turn == 2){
            turn_right();
            turn_right();
        }
        else if (nb_turn == 3){
            turn_left();
        }
    }
    else if (abs(nb_turn) == nb_turn){
        if (nb_turn == 1){
            turn_left();
        }
        else if (nb_turn == 2){
            turn_left();
            turn_left();
        }
        else if (nb_turn == 3){
            turn_right();
        }
    }
}

void position_update(int pos[2], int direction){
    if (direction == 0){
        position[0] = pos[0] + 1;
    }
    else if (direction == 1){
        position[1] = pos[1] - 1;
    }
    else if (direction == 2){
        position[0] = pos[0] - 1;
    }
    else if (direction == 3){
        position[1] = pos[1] + 1;
    }
}

bool check_walls(int pos[2], int direction_check){
    if (murs[pos[0]][pos[1]][direction_check] == 1){
        return 1;
    }
    else if (murs[pos[0]][pos[1]][direction_check]){
        return 0;
    }
}


void try_parcours(){
    while (position[0] != 10) {
        if (check_walls(position, direction) == 0) {
            if (check_open()) {
                forwardInMetersTrapeze(0.5);
                position_update(position, direction);
            }
        }
        else if (direction != 0 && check_walls(position, 0) == 0) {
            turn(direction, 0);
            direction = 0;
            if (check_open()) {
                forwardInMetersTrapeze(0.5);
                position_update(position, direction);
            }
        }
        else if (direction != 1 && check_walls(position, 1) == 0) {
            turn(direction, 1);
            direction = 1;
            if (check_open()) {
                forwardInMetersTrapeze(0.5);
                position_update(position, direction);
            }
        }
        else if (direction != 3 && check_walls(position, 3) == 0) {
            turn(direction, 3);
            direction = 3;
            if (check_open()) {
                forwardInMetersTrapeze(0.5);
                position_update(position, direction);
            }
        }
        else if (direction != 2 && check_walls(position, 2) == 0) {
            turn(direction, 2);
            direction = 2;
            if (check_open()) {
                forwardInMetersTrapeze(0.5);
                position_update(position, direction);
            }
        }        
    }
    if (position[0] == 10){
        turn(direction, 0);
        direction = 0;
        forwardInMetersTrapeze(0.5);
        position[0] = position[0] + 1;
        if (position[1] == 0){
            turn_right();
            forwardInMetersTrapeze(0.5);
            turn_right();
        }
        else if (position[1] == 2){
            turn_left();
            forwardInMetersTrapeze(0.5);
            turn_left();
        }
        forwardInMetersTrapeze(5.5);
    }
}
