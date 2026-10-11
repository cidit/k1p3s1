#include <Arduino.h>
#include <LibRobus.h>

// crash course c++

// mettre dans tous les fichiers.h
#pragma once
#include <Arduino.h>

// fonction binaire (bool) avec nom
bool listen_5khz(int pin_son_ambiant, int pin_son_5khz, int nmb_sample, int sample_time, float sensitivity){

    // lecture des deux pins du circuit
    // analogRead(pin_son_ambiant)
    // analogRead(pin_son_5khz)
    
    float sum_son_ambiant = 0;
    float sum_son_5khz = 0;

    for (int i=0; i<nmb_sample; i++){
        // Donner une variable pour la lecture des pins
        // mettre ; à la fin du statement, dans ce cas-ci : variable = qqchose
        int son_ambiant = analogRead(pin_son_ambiant);
        int son_5khz = analogRead(pin_son_5khz);
        sum_son_ambiant = sum_son_ambiant + son_ambiant;
        sum_son_5khz = sum_son_5khz + son_5khz;
        
        delay(sample_time);
    }
    
    float average_son_ambiant = sum_son_ambiant/nmb_sample;
    float average_son_5khz = sum_son_5khz/nmb_sample;

    // fonction if et conditions pour retourner un oui ou non à la fct bool
    if (average_son_5khz > average_son_ambiant){
        return true;
    }
    else {
        return false;
    } 
}



void setup() {
    Serial.begin(115200);

}

void loop() {
    listen_5khz(A0, A3, 100, 50);

}

