#include <Arduino.h>
#include <LibRobus.h>
//#include <IRremote.hpp>
//#include "fonctions.h"

 void setup(){
    Serial.begin(9600);

 }
void loop() 
{
    Serial.println("allo frank");
    delay(100);
}
    
/* const int ir_receive_pin = 37;
const uint16_t target_address = 0x012B;

void setup()
{
    BoardInit();
    //IrReceiver.begin(ir_receive_pin, ENABLE_LED_FEEDBACK);
    pinMode(22,OUTPUT);
    digitalWrite(22,HIGH);
    Serial.println("Mode lecture");
}

void loop()
{
     if (IrReceiver.decode())
    {
        Serial.println("Commande recue");
        if (IrReceiver.decodedIRData.protocol == NEC and
            IrReceiver.decodedIRData.address == target_address)
            {
                Serial.println("Successful read");
                switch (IrReceiver.decodedIRData.command)
                {
                    case 0xC001:
                        Serial.println("ROBUS start order received");
                        //Mettre protocole de départ ici
                    case 0xC002:
                        Serial.println("ROBUS wake-up order received");
                        //Metre protocole de réveil ici
                    default:
                        Serial.println("Unknown command received: 0x" + String(IrReceiver.decodedIRData.command, HEX));
                }
            }
    } 
   uint32_t state = REMOTE_read();
   if (state == 0)
   {
    Serial.println(state);
   } else {
        Serial.print("Recu : 0x");
    Serial.println(state, HEX);
    delay(1000);
   }
   
} */