#include "stm32f103xb.h"
#include <stdint.h>
#include "usart.h"


// FUNCION DE CONFIGURACIÓN DEL RELOJ (Clock @ 72MHz) ---

void SysClock_Config_72MHz(void) {
    
    // Habilitar el cristal externo (HSE)
    RCC->CR |= RCC_CR_HSEON; 
    
    while (!(RCC->CR & RCC_CR_HSERDY));

    // latencia de la memoria FLASH
    // (Necesario para > 48 MHz)
    FLASH->ACR |= FLASH_ACR_PRFTBE;       // Habilitar el prefetch buffer
    FLASH->ACR &= ~(FLASH_ACR_LATENCY);   // Limpiar los bits de latencia
    FLASH->ACR |= FLASH_ACR_LATENCY_2;    // Configurar 2 wait states (WS)

    // Configurar los divisores de los buses
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    // PCLK2  = HCLK / 1 = 72 MHz (Bus de USART1, SPI1, GPIOs)
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
    // PCLK1 = HCLK / 2 = 36 MHz (Bus de USART2, I2C1)
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

    RCC->CFGR &= ~(RCC_CFGR_PLLMULL | RCC_CFGR_PLLSRC); // Limpiar bits
    RCC->CFGR |= RCC_CFGR_PLLMULL9; // Multiplicador x9 (8 MHz * 9 = 72 MHz)
    RCC->CFGR |= RCC_CFGR_PLLSRC;   // Usar HSE (8 MHz) como fuente del PLL

    RCC->CR |= RCC_CR_PLLON;
    
    while (!(RCC->CR & RCC_CR_PLLRDY));

    // Cambiar clk del sistema (SYSCLK) al PLL que se configuró arriba para que sea mar rapido
    RCC->CFGR &= ~(RCC_CFGR_SW); // Limpiar bits
    RCC->CFGR |= RCC_CFGR_SW_PLL; // Seleccionar PLL
    
    // Esperar a que el cambio se complete
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

int main(void) {

    // Configurar el reloj a 72MHz
    SysClock_Config_72MHz(); 

    usart_init(USART1, 115200);
    usart_send_char(USART1, 'A');

    while(1) {
    }
    return 0;
}