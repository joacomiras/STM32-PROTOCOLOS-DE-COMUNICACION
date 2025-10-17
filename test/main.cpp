// Sketch ESP32 (Esclavo UART)
#include <Arduino.h>

// Pines para el Serial2 del ESP32
#define RXD2 16 // Conectar al PA9 (TX) del STM32
#define TXD2 17 // Conectar al PA10 (RX) del STM32

void setup() {
    // 1. Iniciar monitor serie para la PC (Depuración)
    Serial.begin(115200);
    Serial.println("ESP32 (Esclavo UART) listo.");

    // 2. Iniciar Serial2 para comunicarse con el STM32
    // Asegurarse de que el baud rate sea 9600 (igual que el STM32)
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
    // 3. Revisar si hay un carácter disponible desde el STM32
    if (Serial2.available() > 0) {
        
        // 4. Leer el carácter recibido
        char c = Serial2.read();

        // 5. Imprimir el carácter en el monitor serie de la PC
        Serial.print("Carácter recibido: ");
        Serial.write(c); // Usar .write() es mejor para bytes
        Serial.println(); // Salto de línea

        // 6. Imprimir el valor HEXADECIMAL real (para depuración)
        Serial.print("Valor HEX: 0x");
        Serial.println(c, HEX); 
        Serial.println("-----------------"); // Separador
    }
}