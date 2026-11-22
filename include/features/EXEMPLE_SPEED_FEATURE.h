/*

Pour faciliter l'organisation du code celui-ci doit être divisé en features 
chapeautées par un seul membre de l'équipe qui en prendra pleinement 
possession. 

Pour permettre l'intégration, il est important de se mettre d'accord sur
certaines conventions. 

Voici un exemple de comment je propose d'implémenter nos feature.

Une feature est un comportement du programme final. Cela veut dire que 
chaques features a une fonction d'initialisation (comme setup) et une fonction 
d'action (comme loop). Pour les distinguer du setup et du loop d'arduino, ces
fonctions seront appellées begin et update, respectivement. 

Habituellement, comme dans python, ce code serait définit dans un module à part.
Comme C n'offre pas ce type de fonctionnalités, nos features seront définies 
sous des namespaces comme fait ci-dessous.

...

*/


#pragma once
#include <Arduino.h>

// Important: Gardez `begin` et `update` à la fin pour aider les 
// gens à se repérer dans votre code!
namespace my_example_speed_feature
{

    // Les variables globales peuvent êtres lues par tout le monde. 
    int public_variable; // (VARIABLE TOUCHE)
    
    // Utilisez un `_` au début de votre nom de variable pour indiquer aux
    // autres qu'ils ne devraient pas y toucher (VARIABLE PAS-TOUCHE)
    int _private_variable;

    // Cela va de même pour les noms de fonctions.
    float read_current_speed() {
        return 21;
    }

    void _perform_secret_action() {
        _private_variable = 1000000;
    }

    /**
     * `begin` sert à remplacer `setup` pour VOTRE feature. Elle sera 
     * appellée à la fin du `setup` dans le main principal.
     * 
     * Considérations importantes:
     * Le code ici doit initialiser ses propres resources seulement car 
     * les resources partagées comme le `Serial.begin` et le `BoardInit` 
     * de librobus seront déjà initialisées au début de setup().
     */
    void begin() {

        // PAS BON, Serial.begin sera déja fait dans le setup!
        // Serial.begin(69420); 

        // BON, on assume que la resource est initialisée.
        Serial.println("bonjour `setup` depuis `my_speed_feature`!");
        
    }

    /**
     * `update` sert à remplacer `loop` pour VOTRE feature. Elle sera 
     * appellée dans le `loop` du main principal.
     * 
     * Considéreations importantes:
     * Puisque nos features doivent pouvoir s'executer de manière asynchrone,
     * il faut à tout prix éviter d'utiliser `delay` puisque cette fonction
     * bloque l'execution du code au complet! 
     * Voir `documentation/ASYNC_VS_PARALLEL.md` pour plus d'information.
     */
    void update() {

        // Au lieu d'utiliser un `delay` pour bloquer le code, C'est beaucoup mieux 
        // d'utiliser `millis` pour attendre un moment précis avant d'éxécuter une
        // partie du code qui doit être exécutée périodiquement. 
        if (millis() % 200 == 0) {
            Serial.println("bonjour `loop` depuis `my_speed_feature`!");
        }
    }

} 
