#include "stm32f4xx.h"
#include "clockconfig.h"
#include "NOKIA_5110.h"

#define RST_PORT GPIOD
#define RST_PIN 0
#define CS_PORT GPIOD
#define CS_PIN 1
#define DC_PORT GPIOD
#define DC_PIN 2

custom_libraries::clock_config system_clock;
custom_libraries::NOKIA_5110 NOKIA(SPI1,
                                    GPIOA,
                                    5,
                                    7,
                                    6,
                                    64,
                                    false,
                                    false,
                                    false,
                                    CS_PORT,
                                    CS_PIN,
                                    RST_PORT,
                                    RST_PIN,
                                    DC_PORT,
                                    DC_PIN);

int main(void) {
  
  system_clock.initialize();
  NOKIA.inverted_mode();

  NOKIA.print("MENU",30,0);

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  
  GPIOA->MODER |= GPIO_MODER_MODER7_0;
  GPIOA->MODER |= GPIO_MODER_MODER6_0;

  GPIOA->ODR |= GPIO_ODR_ODR_6;
  GPIOA->ODR &= ~GPIO_ODR_ODR_7;  

  while(1){
    for(volatile int i = 0; i < 2000000; i++){}
    GPIOA->ODR ^= (1<<6);
    GPIOA->ODR ^= (1<<7);

  }
}
