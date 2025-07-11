#include <Arduino.h>

const int pin40 = 4;
const int filas[] = {25, 26, 27};
const char* nombres[] = {"SW1", "SW2", "SW3"};

const unsigned long umbralTiempo = 800;  // μs mínimos para validación

unsigned long tiempoInicio[3] = {0, 0, 0};
bool posibleCoincidencia[3] = {false, false, false};
bool botonYaDetectado[3] = {false, false, false};  // Nueva bandera

void setup() {
  Serial.begin(115200);
  pinMode(pin40, INPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(filas[i], INPUT);
  }
}

//LOOP
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
        if (duracion > umbralTiempo && !botonYaDetectado[i]) {
          Serial.print("¡Coincidencia válida en ");
          Serial.print(nombres[i]);
          Serial.println("!");
          botonYaDetectado[i] = true;  // Solo imprime una vez
          posibleCoincidencia[i] = false;
        }
      }
    } else {
      // Reset si se suelta el botón
      posibleCoincidencia[i] = false;
      botonYaDetectado[i] = false;
    }
  }
}