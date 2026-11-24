# CONVENTIONS

## Structure de code `.h` (dans le dossier `include`)

```cpp
#pragma once
#include <Arduino.h>

namespace <LE NOM DU FICHIER HEADER> {

}
```

## Organisation du dossier `include`

- `features`: Dossier dans lequel on implémente le code en terme de fonctionnalités du robot.
- `commons`: Dossier dans lequel on peut mettre le code commun qui ne rentre pas nécéssairement dans une seule feature.

## Structure des features

Dans `include/features/ma_feature.h`

```cpp
#pragma once
#include <Arduino.h>

namespace ma_feature {

    uint32_t _delais_interne = 1000;

    void faire_quelque_chose() {
        // ...
    }

    void begin() {
        // X    Serial.begin(9600);
        Serial.println("bonjour `setup` depuis `ma_feature`!");
    }

    void update() {
        if (millis() % _delais_interne == 0) {
            // ...
        }
    }
}
```

Dans `include/features/mon_autre_feature.h`

```cpp
#pragma once
#include <Arduino.h>
#include "features/ma_feature.h"

namespace mon_autre_feature {

    // ...

    void begin() {
        // ...
    }

    void update() {
        ma_feature::faire_quelque_chose();
        // X    ma_feature::_delais_interne = 4000;
    }
}
```

Dans `src/main.cpp`

```cpp
#include <Arduino.h>
#include "features/ma_feature.h"
#include "features/mon_autre_feature.h"

void setup() {
    Serial.begin(115200);
    ma_feature::begin();
    mon_autre_feature::begin();
}

void loop() {
    ma_feature::update();
    mon_autre_feature::update();
}

