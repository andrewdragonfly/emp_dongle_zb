/**
Dragonfly Battery dongle (340/50q/chip select A)
Compatible hardware version 1.0 and 1.1
Compatible Firmware version 0.2
**/

#ifndef PCA10056_H
#define PCA10056_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

// LEDs definitions
#define LEDS_NUMBER    5
#define LED_1                NRF_GPIO_PIN_MAP(1,7)
#define LED_2                NRF_GPIO_PIN_MAP(1,6)
#define LED_3                NRF_GPIO_PIN_MAP(1,4)
#define CONFIRMATION_PIN_42Q NRF_GPIO_PIN_MAP(1,1)//A
#define CONFIRMATION_PIN_50Q NRF_GPIO_PIN_MAP(1,3)//B

#define LED_START      LED_1

#define LEDS_ACTIVE_STATE 0

#define LEDS_LIST { LED_1, LED_2, LED_3, CONFIRMATION_PIN_50Q, CONFIRMATION_PIN_42Q }

#define LEDS_INV_MASK  LEDS_MASK

#define BSP_LED_0      13
#define BSP_LED_1      14
#define BSP_LED_2      15
#define BSP_LED_3      16
#define BSP_LED_4      17
#define BSP_LED_5      18

// buttons definitions
#define BUTTONS_NUMBER 1

#define BUTTON_1       NRF_GPIO_PIN_MAP(0,8) 

#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE 0

#define BUTTONS_LIST { BUTTON_1 }

#define BSP_BUTTON_0   BUTTON_1

//Uart
#define RX_PIN_NUMBER  20  
#define TX_PIN_NUMBER  23 

#define RX2_PIN_NUMBER  41  
#define TX2_PIN_NUMBER  11

#define HWFC           true



#ifdef __cplusplus
}
#endif

#endif // PCA10056_H
