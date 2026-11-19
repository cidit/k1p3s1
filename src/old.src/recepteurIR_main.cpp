#include <Arduino.h>
#include <LibRobus.h>

const int ir_receive_pin = 37;
bool robot_start = false;
bool robot_wake_up = true;

void beep(int times) 
{
    for (int i = 0; i < times; i++)
    {
        AX_BuzzerON();
        delay(100);
        AX_BuzzerOFF();
        delay(100);
    }
}

void IR_control_check() 
{
    switch (REMOTE_read()) 
    {
        case 0xF70812ED :
            Serial.println("Start command received");
            beep(2);
            robot_start = true;
            break;
        case 0x778812ED :
            Serial.println("Wake command received");
            beep(3);
            robot_wake_up = true;
            break;
    }
}

bool can_robot_move()
{
    IR_control_check();
    return robot_start and robot_wake_up;
}




void setup()
{
    BoardInit();
    Serial.println("Mode lecture");
    beep(5);
}

void loop()
{
    can_robot_move();
}