// crash course c++

// mettre dans tous les fichiers.h
#pragma once
#include <Arduino.h>

// fonction binaire (bool) avec nom
bool listen_5khz(int pin_son_ambiant, int pin_son_5khz, int nmb_sample, int sample_time){

    // lecture des deux pins du circuit
    // analogRead(pin_son_ambiant)
    // analogRead(pin_son_5khz)
    
    int sum_son_ambiant = 0;
    int sum_son_5khz = 0;

    for (int i=0; i<nmb_sample; i++){
        // Donner une variable pour la lecture des pins
        // mettre ; à la fin du statement, dans ce cas-ci : variable = qqchose
        int son_ambiant = analogRead(pin_son_ambiant);
        int son_5khz = analogRead(pin_son_5khz);
        sum_son_ambiant = sum_son_ambiant + son_ambiant;
        sum_son_5khz = sum_son_5khz + son_5khz;

        delay(sample_time);
    }
    
    int average_son_ambiant = sum_son_ambiant/nmb_sample;
    int average_son_5khz = sum_son_5khz/nmb_sample;

    /*
    int i = 0;
    while (i<10) {
        // do stuff
        i++;
    }
    */

    // fonction if et conditions pour retourner un oui ou non à la fct bool
    if (average_son_5khz > average_son_ambiant){
        return true;
    }
    else {
        return false;
    } 
}

