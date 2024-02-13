#ifndef ZB_STORE_H__
#define ZB_STORE_H__

#include "nrf_nvmc.h"
#include "uart_handler.h"

uint16_t error_count;
uint16_t warning_count;


uint8_t buffer_uart_array [128];

//NVCM variables
const uint32_t f_addr = 0x00080000;//this is for a storage location
uint32_t * p_addr = (uint32_t *)f_addr; 

const uint32_t f_start_nv_param_addr = 0x00080220;//this is for a storage location
uint32_t *p_start_nv_param_addr = (uint32_t *)f_start_nv_param_addr; 

const uint32_t f_EE_count_addr = 0x00080120;//this is for a storage location
uint32_t * p_EE_count_addr = (uint32_t *)f_EE_count_addr; 

const uint32_t f_WW_count_addr = 0x00080010;//this is for a storage location
uint32_t * p_WW_count_addr = (uint32_t *)f_WW_count_addr; 

const uint32_t f_col_addr = 0x0007f000;//this is for a storage location
uint32_t * p_col_addr = (uint32_t *)f_col_addr; 

const uint32_t f_error_addr = 0x00081000;//this is for a storage location
uint32_t * p_error_addr = (uint32_t *)f_error_addr; 

const uint32_t f_warning_addr = 0x00082000;//this is for a storage location
uint32_t * p_warning_addr = (uint32_t *)f_warning_addr; 

void clear_nvmc (int address){
  nrf_nvmc_page_erase(address);
}

void write_nvmc (int value, int words, int address, int offset){
nrf_nvmc_write_words((address + (offset*4)), &value, words);
}

void nvm_to_uart(uint8_t request_id, int offset, uint8_t period_count){
  int buffer = 0;
  
  switch (request_id){

    case 0xEE:
      buffer = *( p_error_addr + (error_count - period_count - 1));//it seems that the val must be written first, also, must be cleared too.
      break;

    case 0xE1:
      buffer = *(p_warning_addr + (warning_count - period_count - 1));
      break;

    default:
      break;
    }
  //this builds the buffer for send
   for (int i = 3; i>-1; i--){
      buffer_uart_array[offset + i] = (short int)buffer;
      buffer = buffer >> 8;
   }
   
}

uint32_t time_nvmc_update (int value){
  clear_nvmc(f_addr);
  value ++;
  write_nvmc(value, 1, f_addr,0);
  write_nvmc(error_count, 1, f_EE_count_addr,0);
  write_nvmc(warning_count, 1, f_WW_count_addr,0);
  
  return *p_addr;
}

uint16_t error_count_upadate (){
  error_count ++;
////////////////TEST THIS
  if(error_count > 1000){
   clear_nvmc(f_error_addr);
   error_count = 0;
  }

//if(*p_EE_count_addr == 0xFFFFFFFF){//since time_nvmc_update clears this, the val will not be updated in nv unless that is called.
// write_nvmc(error_count, 1, f_EE_count_addr,0);
// }
 return error_count;
}

uint16_t warning_count_upadate (){
  warning_count ++;

  if(warning_count > 1000){
    clear_nvmc(f_warning_addr);
    warning_count = 0;
  }

//if(*p_WW_count_addr == 0xFFFFFFFF){
// write_nvmc(warning_count, 1, f_WW_count_addr,0);
// }
 return warning_count;
}


uint32_t ave_cap (){
uint8_t ave_div;
if ( (MOD5_CAP + MOD6_CAP + MOD7_CAP + MOD8_CAP) > 0 ){
  ave_div = 8;
  }
  else {
  ave_div = 4;
  }
return (MOD1_CAP + MOD2_CAP + MOD3_CAP + MOD4_CAP + MOD5_CAP + MOD6_CAP + MOD7_CAP + MOD8_CAP)/ave_div;
}


uint32_t ave_volt (){
uint8_t ave_div;
if ( (MOD5_VOLT + MOD6_VOLT + MOD7_VOLT + MOD8_VOLT) > 0 ){
  ave_div = 8;
  }
  else {
  ave_div = 4;
  }
return (MOD1_VOLT + MOD2_VOLT + MOD3_VOLT + MOD4_VOLT + MOD5_VOLT + MOD6_VOLT + MOD7_VOLT + MOD8_VOLT)/ave_div;
}


uint8_t self_drain = 10; //TODO get this

uint16_t col_nvmc_update (bool reset, int32_t value){
  uint32_t buffer = *p_col_addr + value - self_drain;
  //int amp_periods = ((MOD1_CAP + MOD1_CAP + MOD1_CAP + MOD1_CAP)/4)*60*60/5;
  int amp_periods = (( ave_cap() )/4)*60*60/5;
  if((buffer > amp_periods && value < 400) || reset == true){ // val < X is intended to be when the charger moves to lower c rate | one reset condition is float
   buffer = amp_periods;
   }
  nrf_nvmc_page_erase(f_col_addr);
  write_nvmc(buffer, 1, f_col_addr,0);
  return ((buffer*1.0)*10000)/(amp_periods*1.0);//return percent
}

uint32_t time_init (){
return *p_addr;
}


void alert_counters_init(){//on boot this will get the count from nvm
error_count = *p_EE_count_addr;
warning_count = *p_WW_count_addr;
}


uint32_t error_nvmc_store (int16_t value, int offset){
//*p_addr is the time, value is the message
  uint32_t buffer = *p_addr<<8|value;
  write_nvmc(buffer, 1, f_error_addr,offset *4);
  return *(p_error_addr + (offset*4));
}


uint32_t warning_nvmc_store (int16_t value, int offset){
  uint32_t buffer = *p_addr<<8|value;
  write_nvmc(buffer, 1, f_warning_addr,offset*4);
  return *(p_warning_addr + (offset*4));
}

bool get_start_nv_param (){
  if(*p_start_nv_param_addr != 0xFFFFFFFF){
  return true;
  }   
  else {
  return false;
  }
}

void reboot_reset (uint8_t cmd){
  if (cmd == 1){
    write_nvmc(cmd, 1, f_start_nv_param_addr,0);
  }

zb_reset(0);
}

#endif /* ZB_STORE_H__ */
