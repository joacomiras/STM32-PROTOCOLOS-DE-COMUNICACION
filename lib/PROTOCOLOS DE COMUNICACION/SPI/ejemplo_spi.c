#include "spi.h" // Incluimos nuestra librería genérica de SPI

// --- Constantes para la comunicación ---

// Comando estándar para leer el JEDEC ID de una memoria flash
#define SPI_COMMAND_READ_ID   0x9F 
// Byte que enviamos cuando solo nos interesa recibir datos
#define SPI_READ_BYTE        0xFF 

// Función de retardo simple para el bucle
void delay_ms(volatile uint32_t ms) {
    ms *= 8000; // Valor aproximado para un reloj de 72MHz
    while(ms--);
}

int main(void) {
    // 1. Inicializa el periférico SPI1 en modo maestro
    spi_iniciar(SPI1);

    // Variables para almacenar los datos recibidos del esclavo
    unsigned char manufacturer_id;
    unsigned char device_id_high;
    unsigned char device_id_low;

    // Bucle infinito para leer el ID del dispositivo esclavo continuamente
    while (1) {
        // 2. Inicia la comunicación poniendo la línea SS (Slave Select) en BAJO
        spi_seleccionar_esclavo(SPI1);

        // 3. ENVÍO: Enviamos el comando 0x9F para solicitar el ID.
        //    El dato que devuelve el esclavo durante este envío no nos interesa, así que lo ignoramos.
        spi_transferir_byte(SPI1, SPI_COMMAND_READ_ID);

        // 4. RECEPCIÓN: Ahora leemos los 3 bytes de respuesta del esclavo.
        //    Para recibir, debemos enviar "bytes falsos" (READ) para generar los pulsos de reloj.
        
        // Leemos el primer byte (ID del fabricante)
        manufacturer_id = spi_transferir_byte(SPI1, SPI_READ_BYTE);

        // Leemos el segundo byte (Parte alta del ID del dispositivo)
        device_id_high = spi_transferir_byte(SPI1, SPI_READ_BYTE);

        // Leemos el tercer byte (Parte baja del ID del dispositivo)
        device_id_low = spi_transferir_byte(SPI1, SPI_READ_BYTE);

        // 5. Finaliza la comunicación poniendo la línea SS en ALTO
        spi_deseleccionar_esclavo(SPI1);
        
        // Esperamos un momento antes de volver a leer
        delay_ms(1000);
    }

    return 0;
}