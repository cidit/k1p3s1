#include <Arduino.h>
#include <LibRobus.h>

const int ir_receive_in = 37;

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


bool IR_wake_check() {
    return (REMOTE_read() == 0x778812ED);
}

bool IR_start_check() {
    return (REMOTE_read() == 0xF70812ED);
}