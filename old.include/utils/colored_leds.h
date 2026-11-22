#include <Arduino.h>
#include <LibRobus.h>

enum {rouge, orange, jaune, vert, bleu, nb_leds};

struct ColoredLED
{
    int pin;
    unsigned long time_set;
    bool is_on;
    
    ColoredLED(int led_pin)
    {
        pin = led_pin;
        time_set = 0;
        is_on = false;
    }

    void begin()
    {
        pinMode(pin, OUTPUT);
    }

    void on()
    {
        digitalWrite(pin, HIGH);
        time_set = millis();
        is_on = true;
    }

    void off()
    {
        digitalWrite(pin, LOW);
        is_on = false;
    }

    bool is_time()
    {
        return is_on and (millis() - time_set > 10 * 1000);
    }
};

void start_colored_leds(ColoredLED colored_leds[])
{
        for (int i = 0; i < nb_leds; i++)
    {
        colored_leds[i].begin();
    }
}

void check_colored_leds(ColoredLED colored_leds[])
{
    for (int i = 0; i < nb_leds; i++)
{
    if (colored_leds[i].is_time())
    {
        colored_leds[i].off();
    }
}
}