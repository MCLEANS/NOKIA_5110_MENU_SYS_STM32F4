#include "stm32f4xx.h"
#include "clockconfig.h"
#include "NOKIA_5110.h"
#include "GPIO.h"

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
custom_libraries::_GPIO K0(GPIOE,4);
custom_libraries::_GPIO K1(GPIOE,3);

int main(void) {
  
  system_clock.initialize();
  //configure buttons K0 and K1 as inputs
  K0.pin_mode(custom_libraries::INPUT);
  K0.input_state(custom_libraries::PULL_UP);
  K1.pin_mode(custom_libraries::INPUT);
  K1.input_state(custom_libraries::PULL_UP);
  
  NOKIA.print("MENU",30,0);
  
  uint8_t x_value = 70;
  uint8_t y_value = 1;

  while(1){

    NOKIA.print("<<",x_value,y_value);

    if(!K0.digital_read()){
      y_value+=1;
      if(y_value >= 5){
        y_value = 5;
      }
    }

    NOKIA.clear();
  }
}
