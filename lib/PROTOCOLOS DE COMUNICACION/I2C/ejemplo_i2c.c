#include "i2c.h"

// Dirección de un sensor de ejemplo
#define SENSOR_ADD 0x88 // Dirección de 7 bits

// Dirección del registro interno del sensor que queremos leer (ej: registro de temperatura)
#define SENSOR_REG_TEMP 0x07

int main(void) {
    // Inicializar el bus I2C1
    i2c_iniciar(I2C1);
    
    unsigned char temp_byte_alto;
    unsigned char temp_byte_bajo;

    while(1) {
        // --- Paso 1: Escribir la dirección del registro que queremos leer ---
        i2c_start(I2C1);
        i2c_enviar_direccion(I2C1, SENSOR_ADD); // Dirección del sensor en modo escritura
        i2c_enviar_byte(I2C1, SENSOR_REG_TEMP);  // Indicar qué registro interno leer
        i2c_stop(I2C1); // Detener (algunos sensores necesitan un reinicio)

        // --- Paso 2: Leer los 2 bytes de datos de ese registro ---
        i2c_start(I2C1); // Iniciar de nuevo la comunicación
        i2c_enviar_direccion(I2C1, SENSOR_ADD | 0x01); // Dirección del sensor en MODO LECTURA

        // Leer el primer byte y enviar ACK (queremos más)
        temp_byte_alto = i2c_recibir_byte_ack(I2C1);

        // Leer el segundo (y último) byte y enviar NACK (no queremos más)
        temp_byte_bajo = i2c_recibir_byte_nack(I2C1);
        
        // Finalizar la transacción
        i2c_stop(I2C1);

        // Delay para no saturar el bus
        for(volatile int i=0; i<1000000; i++);
    }
}