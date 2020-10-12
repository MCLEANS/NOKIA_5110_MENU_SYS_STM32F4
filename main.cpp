#include "stm32f4xx.h"
#include "clockconfig.h"
#include "NOKIA_5110.h"
#include "GPIO.h"
#include "EXTI.h"

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
custom_libraries::_EXTI K0(GPIOE,4,custom_libraries::FALLING);
custom_libraries::_EXTI K1(GPIOE,3,custom_libraries::FALLING);

uint8_t x_value = 70;
uint8_t y_value = 1;

uint8_t line_counter = 1;
uint8_t page = 1;

bool clear_screen = false;

extern "C" void EXTI3_IRQHandler(void){
  if(EXTI->PR & EXTI_PR_PR3){
    EXTI->PR |= EXTI_PR_PR3;
    y_value += 1;
    line_counter++;
    if(y_value > 4) y_value = 5;
    clear_screen = true;
  }
}

extern "C" void EXTI4_IRQHandler(void){
  if(EXTI->PR & EXTI_PR_PR4){
    EXTI->PR |= EXTI_PR_PR4;
    y_value -= 1;
    line_counter --;
    if(y_value < 2) y_value = 1;
    clear_screen = true;
  }
}

void first_menu_page(void){
  NOKIA.print("1. FILES",0,1);
  NOKIA.print("2. DATA",0,2);
  NOKIA.print("3. ARCHIVE",0,3);
  NOKIA.print("4. PRINT",0,4);
  NOKIA.print("5. DELETE",0,5);
}

void second_menu_page(void){
  NOKIA.print("6. FILES",0,1);
  NOKIA.print("7. DATA",0,2);
  NOKIA.print("8. ARCHIVE",0,3);
  NOKIA.print("9. PRINT",0,4);
  NOKIA.print("10. DELETE",0,5);  
}

int main(void) {
  
  system_clock.initialize();
 
   //Initialize interrupt pins
  K0.initialize();
  K1.initialize();

  //Set interrupt priority and enable
  NVIC_SetPriority(EXTI3_IRQn,0x02);
  NVIC_EnableIRQ(EXTI3_IRQn);
  NVIC_SetPriority(EXTI4_IRQn,0x02);
  NVIC_EnableIRQ(EXTI4_IRQn);

  while(1){
    NOKIA.print("MENU",20,0);
    if(line_counter > 10) line_counter = 10;
    if(line_counter < 1) line_counter = 1;
    if(line_counter <= 5) page = 1;
    if(line_counter > 5 and line_counter <= 10) page = 2;
    if(page == 1) first_menu_page();
    if(page == 2) second_menu_page();
    NOKIA.print("<<",x_value,y_value);

    if(clear_screen){
      NOKIA.clear();
      clear_screen = false;
    }
    
  }
}
