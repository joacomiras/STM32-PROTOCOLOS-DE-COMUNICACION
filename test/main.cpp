#include <Arduino.h>


#define RXD2 16 // Conectar al PA9 (TX) del STM32
#define TXD2 17 // Conectar al PA10 (RX) del STM32

void setup() {
    // Serial del monitor serie
    Serial.begin(115200);
    Serial.println("ESP32 (Esclavo UART) listo.");

    // Serial2 porque el 1 es el monitor serie
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
    if (Serial2.available() > 0) {
        
        // Leer el carácter recibido
        char c = Serial2.read();

        Serial.print("Carácter recibido: ");
        Serial.write(c); // Usar .write() es mejor para bytes
    }
}
