#include <Arduino.h>
#include <LibRobus.h>
#include <IRremote.hpp>

const int ir_receive_pin = 37;
const uint16_t target_address = 0x012B;

void setup()
{
    Serial.begin(115200);
    IrReceiver.begin(ir_receive_pin, ENABLE_LED_FEEDBACK);
}

void loop()
{
    if (IrReceiver.decode())
    {
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
}