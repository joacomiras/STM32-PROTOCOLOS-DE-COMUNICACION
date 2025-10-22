#include <Arduino.h>

#define RXD2 16 // Conectar al PA9 (TX) del STM32
#define TXD2 17 // Conectar al PA10 (RX) del STM32

void setup() {

    Serial.begin(115200);
    delay(2000); 
    Serial.println("ESP32 (Esclavo UART) listo.");


    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);


}

void loop() {
    if (Serial2.available() > 0) {
        char c = Serial2.read();
        Serial.print("Carácter recibido: ");
        Serial.write(c);
        Serial.println();
    }
}
