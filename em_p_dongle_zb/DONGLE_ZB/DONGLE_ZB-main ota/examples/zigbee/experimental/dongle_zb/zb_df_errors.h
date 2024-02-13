#ifndef zb_df_error
#define zb_df_error


 
bool ee_shift = false;

void error_messages(uint8_t value, bool reset){
 if (reset == false){
  if(value != Zigbee_uart.ee1_val || value != Zigbee_uart.ee2_val){
  error_nvmc_store(value,error_count);//TODO check that this does not overflow into next addr
   if (ee_shift == false){
    Zigbee_uart.ee1_val = value;
    ee_shift = true;
    }
  else{
    Zigbee_uart.ee2_val = value;
    ee_shift = false;
  }
  }
  }
  else {//reset == true
    Zigbee_uart.ee1_val = 0;
    Zigbee_uart.ee2_val = 0;
  }
  }

bool ww_shift = false;

void warning_messages(uint8_t value, bool reset){
 if (reset == false){
  if(value != Zigbee_uart.ww1_val || value != Zigbee_uart.ww2_val){//new warning
  warning_nvmc_store(value,warning_count);//TODO check that this does not overflow into next addr
    if (ww_shift == false){
      Zigbee_uart.ww1_val = value;
      ww_shift = true;
    }
    else{
      Zigbee_uart.ww2_val = value;
      ww_shift = false;
  }
  }
  }
  else {//reset == true
    Zigbee_uart.ww1_val = 0;
    Zigbee_uart.ww2_val = 0;
  }
}


void fet_error(uint8_t param){
  //this is for the wiggle wire error method
          
  if(nrf_gpio_pin_read(SIGNAL1)){
    //Error 1    
    param = 0xFF;//this is the enumeration for sig1 state
    }
  else if(nrf_gpio_pin_read(SIGNAL2)){
    //Error 2
    //param = 0xFE;//this is the enumeration for sig2 state
  }
  else{
    //No error      
  }
  if (param != 0){
    error_messages(param, false);
    ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
  }
}



#endif