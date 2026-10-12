#include <Arduino.h>
#include <LibRobus.h>

// mettre dans tous les fichiers.h
#include <Arduino.h>

void differential_read(int pin_ambiant, int pin_5khz, int interval, int sample_size, int sample_time) // Non-utilisée
{
    float sum_ambiant = 0;
    float sum_5khz = 0;
    float next_sum_ambiant = 0;
    float next_sum_5khz = 0;

    for (int i = 0; i < sample_size; i++)
    {
        int reading_ambiant = analogRead(pin_ambiant);
        int reading_5khz = analogRead(pin_5khz);
        sum_ambiant += reading_ambiant;
        sum_5khz += reading_5khz;
        delay(sample_time);
    }
    float average_reading_ambiant = sum_ambiant / sample_size;
    float average_reading_5khz = sum_5khz / sample_size;

    delay(interval);

    for (int i = 0; i < sample_size; i++)
    {
        int next_reading_ambiant = analogRead(pin_ambiant);
        int next_reading_5khz = analogRead(pin_5khz);
        next_sum_ambiant += next_reading_ambiant;
        next_sum_5khz += next_reading_5khz;
        delay(sample_time);
    }
    float average_next_reading_ambiant = next_sum_ambiant / sample_size;
    float average_next_reading_5khz = next_sum_5khz / sample_size;

    float diff_ambiant = float(average_next_reading_ambiant - average_reading_ambiant) / interval;
    float diff_5khz = float(average_next_reading_5khz - average_reading_5khz) / interval;

    Serial.println(String(">") +
                   "avgA:" + String(average_reading_ambiant) + "," +
                   "avgNA:" + String(average_next_reading_ambiant) + "," +
                   "avg5:" + String(average_reading_5khz) + "," +
                   "avgN5:" + String(average_next_reading_5khz) + "," +
                   "dA:" + String(diff_ambiant) + "," +
                   "d5:" + String(diff_5khz));
}

float difference_read(int pin_ambiant, int pin_5khz, int interval, int sample_size, int sample_time)
{
    /**
     * @brief Calcule la différence entre les points de deux fonctions d'entrées analog IN
     * @details Fait la moyenne mobile simple de deux séries de points, puis les soustrait
     * @param pin_ambiant La pin analog IN connectée à la sortie du circuit pour le bruit ambiant
     * @param pin_5khz La pin analog IN connectée à la sortie du circuit pour l'amplificateur de 5 kHz 
     * @param interval Le temps d'attente entre deux appels de la fonction
     * @param sample_size La quantité de lectures analog pour faire la moyenne des points
     * @param sample_time Le temps d'attente entre deux séries de lectures analog
     * @return La différence entre les aires sous la courbe des deux fonctions d'entrées analog IN (float)
     */
    float sum_ambiant = 0;
    float sum_5khz = 0;

    for (int i = 0; i < sample_size; i++) 
    {
        int reading_ambiant = analogRead(pin_ambiant);
        int reading_5khz = analogRead(pin_5khz);
        sum_ambiant += reading_ambiant;
        sum_5khz += reading_5khz;
        delay(sample_time);
    }

    float average_reading_ambiant = sum_ambiant / sample_size; 
    float average_reading_5khz = sum_5khz / sample_size;

    float integral_ambiant = average_reading_ambiant * interval;
    float integral_5khz = average_reading_5khz * interval;

    float integral_difference = average_reading_5khz - average_reading_ambiant;

    delay(interval);

    return integral_difference;
    
/*     Serial.println
    (
    String(">") +
    "avgA:" + String(average_reading_ambiant) + "," +
    "avg5:" + String(average_reading_5khz) + "," +
    "iA:" + String(integral_ambiant) + "," +
    "i5:" + String(integral_5khz) + "," + 
    "id:" + String(integral_difference)
    );  */

}

bool is_sifflet(int pin_ambiant, int pin_5khz, int patience, float integral_thr) 
{
    /**
     * @brief Vérifie si un sifflet a été entendu
     * @details Additionne difference_read() à une somme sur un certain nombre d'itérations, en regardant à chaque fois si cette somme atteint un certain integral_thr
     * @param pin_ambiant La pin analog IN connectée à la sortie du circuit pour le bruit ambiant
     * @param pin_5khz La pin analog IN connectée à la sortie du circuit pour l'amplificateur de 5 kHz
     * @param patience Le nombre de mises à jour permises à integral_sum pour atteindre integral_thr
     * @param integral_thr Le minimum requis pour que la fonction retourne True
     * @return Un booléen
     */
    float integral_sum = 0.0;
    for (int i = 0; i < patience; i++) {
        integral_sum += difference_read(pin_ambiant, pin_5khz, 100, 5, 5);
        //Serial.print(String(integral_sum));
        if (integral_sum > integral_thr) {
            return true;
        }
    }
    Serial.print("\n");
    return false;
}

int pin_ambiant = A0;
int pin_5khz = A3;
bool heard_sifflet = false;

void setup()
{
    Serial.begin(115200);
    pinMode(pin_ambiant, INPUT);
    pinMode(pin_5khz, INPUT);
}



void loop()
{
    if (is_sifflet(A0, A3, 5, 0)) { // Vérifie sur un temps de (patience * interval + sample_time * sample_size) ms si le sifflet à été entendu
        heard_sifflet = true;
    }

    if (heard_sifflet) {
        // Tous les comportements du robot après avoir entendu le sifflet devront être dans ce if statement
        Serial.println("hi");
    }
}
