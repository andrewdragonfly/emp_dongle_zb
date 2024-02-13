#ifndef DF_CHIP_CHECK
#define DF_CHIP_CHECK

void confirmation_init (){//turn IO low
  nrf_gpio_pin_write(CONFIRMATION_PIN_50Q,0x0);
  nrf_gpio_pin_write(CONFIRMATION_PIN_42Q,0x0);
}



void chip_check(){
  char resp;
  confirmation_init();
  nrf_gpio_pin_write(CONFIRMATION_PIN_50Q,0xFFFFFFFF);

  mem_write_complete();
}


#endif// end .h