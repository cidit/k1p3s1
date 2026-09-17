#include <Arduino.h>

void setup() 
{
  // Initialise le port de communication et attend pour l'ouvrir:
  Serial.begin(9600);
  // Ce délai permet de s'assurer que le moniteur serie (Serial Monitor) soit disponible
  delay(1500);
}

void loop() 
{
  Serial.println("Bonjour le monde!");

  delay(100);
}