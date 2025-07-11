#include <Arduino.h>

// Pines del ESP32 conectados al teclado
const int pin40 = 4;                      // Pin común del teclado
const int filas[] = {25, 26, 27};         // GPIOs disponibles y válidos (ajusta tus conexiones físicas)
const char* nombres[] = {"SW1", "SW2", "SW3"};

const unsigned long umbralTiempo = 800;  // microsegundos mínimos para validación

unsigned long tiempoInicio[3] = {0, 0, 0};
bool posibleCoincidencia[3] = {false, false, false};

void setup() {
  Serial.begin(115200);
  pinMode(pin40, INPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(filas[i], INPUT);
  }
}

void loop() {
  bool estado40 = digitalRead(pin40);

  for (int i = 0; i < 3; i++) {
    bool estadoFila = digitalRead(filas[i]);

    if (estado40 == LOW && estadoFila == LOW) {
      if (!posibleCoincidencia[i]) {
        posibleCoincidencia[i] = true;
        tiempoInicio[i] = micros();
      } else {
        unsigned long duracion = micros() - tiempoInicio[i];
        if (duracion > umbralTiempo) {
          Serial.print("¡Coincidencia válida en ");
          Serial.print(nombres[i]);
          Serial.println("!");
          delay(150);
          posibleCoincidencia[i] = false;
        }
      }
    } else {
      posibleCoincidencia[i] = false;
    }
  }
}