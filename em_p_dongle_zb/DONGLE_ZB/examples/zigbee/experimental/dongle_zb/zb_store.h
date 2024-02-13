#ifndef ZB_STORE_H__
#define ZB_STORE_H__

#include "nrf_nvmc.h"
#include "uart_handler.h"
#include "zb_dongle_gloabals.h"


uint16_t error_count;
uint16_t warning_count;


uint8_t buffer_uart_array [128];

//NVCM variables
const uint32_t f_addr = 0x00080000;//this is for a storage location
uint32_t * p_addr = (uint32_t *)f_addr;

const uint32_t f_start_nv_param_addr = 0x00080220;//this is for a storage location
uint32_t * p_start_nv_param_addr = (uint32_t *)f_start_nv_param_addr; 

const uint32_t f_paired_addr = 0x00080224;//this is for a storage location
uint32_t * p_paired_addr = (uint32_t *)f_paired_addr; 

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

const uint32_t f_dev_id_addr = 0x00083000;//this is for a storage location
uint32_t * p_dev_id_addr = (uint32_t *)f_dev_id_addr; 


void clear_nvmc (int address){
  nrf_nvmc_page_erase(address);
}

void write_nvmc (uint32_t value, int words, int address, int offset){
nrf_nvmc_write_words((address + (offset*4)), &value, words);
}


void reset_error_counter (){
  if (*p_EE_count_addr == 0xffffffff){
    write_nvmc(0, 1, f_EE_count_addr,0);
  }

  if (*p_WW_count_addr == 0xFFFFFFFF){
    write_nvmc(0, 1, f_WW_count_addr,0);
  }
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
  //put stuff here that needs to be written to after time clears it
  uint32_t paired_status = *p_paired_addr;
  clear_nvmc(f_addr);
  value ++;
  write_nvmc(value, 1, f_addr,0);
  write_nvmc(error_count, 1, f_EE_count_addr,0);
  write_nvmc(warning_count, 1, f_WW_count_addr,0);
  write_nvmc(paired_status, 1, f_paired_addr, 0);
  
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

void reboot_reset (uint8_t cmd){//call this to reset the mcu, if command is passed, write to mem, when it reboots it will reset the config rather than just reboot.
  if (cmd == 1){
    write_nvmc(cmd, 1, f_start_nv_param_addr,0);
  }

  zb_reset(0);
}

void paired_set (uint8_t cmd){//call this show that the board was zb paired, this will be the check to do the reset timers
  if(cmd == 0){
  write_nvmc(0xFFFFFFFF, 1, f_paired_addr, 0);
  }
  else{
    if(*p_paired_addr == 0xFFFFFFFF){//check if not written to 
      write_nvmc(1, 1, f_paired_addr, 0);
    }
  }
}


char parsed_sn [12];
char parsed_dt [8];

struct{
  char serial [13];
  char date [9];
  char hw_version [2][4];//1[4].2[4]
  char fw_version [3][4];//1[4].2[4].3[4]
  char cc_coef [8];
  char sv_coef [8];
  char cap [8][8];//id|val
  char volt [8][8];//id|val
  bool set;
  //TODO add the mod and volt data

}batt_data;

int ascii_convert (int val, int type){
  switch (type){
    case 0:
      if (val<0x40){
      return val-0x30;}
        else {
          return val-0x37;
        }
      if(val==0){
      return 0x30;
      }
    break;

    case 1:
      if (val<0x40){
        return val-0x0;}
          else {   
            return val-0x20;
        }
    break;

    default:
    break;
  }
  }

void write_ser_to_flash(){
  uint8_t convert_buffer[6];

  for (int i = 0; i < 6 ;i++){
    convert_buffer [i] = ascii_convert(batt_data.serial[i * 2 + 1],0)  | ascii_convert(batt_data.serial[i * 2],0)<< 4;
  }
  //padding
  convert_buffer [6] = 0x00;
  convert_buffer [7] = 0x00;

  uint32_t  p_dev_active_serial = convert_buffer[0]<<24 | convert_buffer[1] << 16| convert_buffer[2]<<8|convert_buffer[3]<<0;
  nrf_nvmc_write_word((f_dev_id_addr),p_dev_active_serial);
  uint32_t p_dev_active_serial1 = convert_buffer[4]<<24 | convert_buffer[5] << 16| convert_buffer[6]<<8|convert_buffer[7]<<0;
  nrf_nvmc_write_word((f_dev_id_addr + 4),p_dev_active_serial1);
}

void write_date_to_flash(){
  uint32_t  p_date = ascii_convert(batt_data.date[0],0) << 28 |
                     ascii_convert(batt_data.date[1],0) << 24 |
                     ascii_convert(batt_data.date[2],0) << 20 |
                     ascii_convert(batt_data.date[3],0) << 16 |
                     ascii_convert(batt_data.date[4],0) << 12 |
                     ascii_convert(batt_data.date[5],0) << 8  |
                     ascii_convert(batt_data.date[6],0) << 4  |
                     ascii_convert(batt_data.date[7],0) << 0  ;
                                                          
  nrf_nvmc_write_word((f_dev_id_addr + 8),p_date);
}

void write_versions_to_flash(uint8_t cmd){
//cmd: 3 = hw, 4 = fw, using the enum from the parent
//format FFFF.FFFF | FFFFFFFF
uint32_t  p_version;

  switch (cmd){
  case 3:
    p_version = ascii_convert(batt_data.hw_version[0][0],0) << 28 |
                ascii_convert(batt_data.hw_version[0][1],0) << 24 |
                ascii_convert(batt_data.hw_version[0][2],0) << 20 |
                ascii_convert(batt_data.hw_version[0][3],0) << 16 |

                ascii_convert(batt_data.hw_version[1][0],0) << 12 |
                ascii_convert(batt_data.hw_version[1][1],0) << 18 |
                ascii_convert(batt_data.hw_version[1][2],0) << 4  |
                ascii_convert(batt_data.hw_version[1][3],0) << 0  ;
   nrf_nvmc_write_word((f_dev_id_addr + 12),p_version);
  break;

  case 4:
    p_version = ascii_convert(batt_data.fw_version[0][0],0) << 28 |
                ascii_convert(batt_data.fw_version[0][1],0) << 24 |
                ascii_convert(batt_data.fw_version[0][2],0) << 20 |
                ascii_convert(batt_data.fw_version[0][3],0) << 16 |

                ascii_convert(batt_data.fw_version[1][0],0) << 12 |
                ascii_convert(batt_data.fw_version[1][1],0) << 18 |
                ascii_convert(batt_data.fw_version[1][2],0) << 4  |
                ascii_convert(batt_data.fw_version[1][3],0) << 0  ;
   nrf_nvmc_write_word((f_dev_id_addr + 16),p_version);

    p_version = ascii_convert(batt_data.fw_version[2][0],0) << 28 |
                ascii_convert(batt_data.fw_version[2][1],0) << 24 |
                ascii_convert(batt_data.fw_version[2][2],0) << 20 |
                ascii_convert(batt_data.fw_version[2][3],0) << 16 ;

   nrf_nvmc_write_word((f_dev_id_addr + 20),p_version);
  break;
  }
}

void write_coef_to_flash(uint8_t cmd){
  uint32_t p_char;
  switch (cmd){
    case 7:
      p_char = ascii_convert(batt_data.cc_coef[0],0) << 28 |
               ascii_convert(batt_data.cc_coef[1],0) << 24 |
               ascii_convert(batt_data.cc_coef[2],0) << 20 |
               ascii_convert(batt_data.cc_coef[3],0) << 16 |
               ascii_convert(batt_data.cc_coef[4],0) << 12 |
               ascii_convert(batt_data.cc_coef[5],0) << 8  |
               ascii_convert(batt_data.cc_coef[6],0) << 4  |
               ascii_convert(batt_data.cc_coef[7],0) << 0  ;                         
     break;
     
     case 8:
      p_char = ascii_convert(batt_data.sv_coef[0],0) << 28 |
               ascii_convert(batt_data.sv_coef[1],0) << 24 |
               ascii_convert(batt_data.sv_coef[2],0) << 20 |
               ascii_convert(batt_data.sv_coef[3],0) << 16 |
               ascii_convert(batt_data.sv_coef[4],0) << 12 |
               ascii_convert(batt_data.sv_coef[5],0) << 8  |
               ascii_convert(batt_data.sv_coef[6],0) << 4  |
               ascii_convert(batt_data.sv_coef[7],0) << 0  ;
      break;
  
  }

  nrf_nvmc_write_word((f_dev_id_addr + (cmd*4)),p_char);
}



void write_cyc_to_flash(uint8_t cmd, uint8_t pos){
  uint32_t p_char;
  switch (cmd){
    case 5:
      p_char = ascii_convert(batt_data.volt[pos][0],0) << 28 |
               ascii_convert(batt_data.volt[pos][1],0) << 24 |
               ascii_convert(batt_data.volt[pos][2],0) << 20 |
               ascii_convert(batt_data.volt[pos][3],0) << 16 |
               ascii_convert(batt_data.volt[pos][4],0) << 12 |
               ascii_convert(batt_data.volt[pos][5],0) << 8  |
               ascii_convert(batt_data.volt[pos][6],0) << 4  |
               ascii_convert(batt_data.volt[pos][7],0) << 0  ;

               nrf_nvmc_write_word((f_dev_id_addr + (32 + (pos*4))),p_char);                      
     break;
     
     case 6:
      p_char = ascii_convert(batt_data.cap[pos][0],0) << 28 |
               ascii_convert(batt_data.cap[pos][1],0) << 24 |
               ascii_convert(batt_data.cap[pos][2],0) << 20 |
               ascii_convert(batt_data.cap[pos][3],0) << 16 |
               ascii_convert(batt_data.cap[pos][4],0) << 12 |
               ascii_convert(batt_data.cap[pos][5],0) << 8  |
               ascii_convert(batt_data.cap[pos][6],0) << 4  |
               ascii_convert(batt_data.cap[pos][7],0) << 0  ;   

               nrf_nvmc_write_word((f_dev_id_addr + (64 + (pos*4))),p_char);
  break;
  
  }

}

bool header_complete(){
  bool resp_state = true;
  for (int i=0; i<5;i++){//TODO this needs to represent all mems
    if(*(p_dev_id_addr + i) == 0xFFFFFFFF){
      resp_state = false;
      batt_data.set = false;
    break;
    }
    else{
    resp_state = true;
    }
  }
  if(resp_state == true){
   nrf_gpio_pin_write(CONFIRMATION_PIN_42Q,0xFFFFFFFF);
   batt_data.set = true;
   init_header();
   }

}

void id_store (uint8_t cmd, uint8_t len){
  
  uint8_t start;
  uint8_t mem_offset;
  start = 8;
  header_complete();
  switch (cmd){
    case 0xF://clear mem
      clear_nvmc(f_dev_id_addr);
    break;
    case 1://serial
      mem_offset = 0;
      for (int i = 0; i<len; i++){
        batt_data.serial[i] = request_buffer[i+start];
      }
      write_ser_to_flash();
   break;

  case 2://date
      for (int i = 0; i<len; i++){
        batt_data.date[i] = request_buffer[i+start];
      }
      write_date_to_flash();
  break;

  case 3://hw_vers //fallthrough
  case 4://fw_vers
      for (int val = 0, hi_i = 0, val_i = 0; val < len; val++){
        if(request_buffer[val+start] == '.'){
          hi_i ++;
          val_i = 0;
        }
        else if (request_buffer[val+start] == '\r'){val_i ++;}
        else {
          if (cmd == 3){
            batt_data.hw_version[hi_i][val_i] = request_buffer[val+start];
          }
          else if (cmd == 4){
            batt_data.fw_version[hi_i][val_i] = request_buffer[val+start];
          }
          //val_i ++;
        }
        }
        for(int i = 0; i<8; i++){
          if (cmd == 3 && (batt_data.hw_version[i/4][i%4] == 'F' || batt_data.hw_version[i/4][i%4] == '\r' || batt_data.hw_version[i/4][i%4] == 0x0)){
            batt_data.hw_version[i/4][i%4] = '0';
          }
         }
        for(int i = 0; i<12; i++){
          if (cmd == 4 && (batt_data.fw_version[i/4][i%4] == 'F' || batt_data.fw_version[i/4][i%4] == '\r' || batt_data.fw_version[i/4][i%4] == 0x0)){
            batt_data.fw_version[i/4][i%4] = '0';
          }
        }
      write_versions_to_flash(cmd);
      break;   
      
  case 5://voltage
    for (int i = 0; i<8; i++){
      if(request_buffer[i+start] == '\r' || request_buffer[i+start] == 'F' || request_buffer[i+start] == 0x0){
        batt_data.volt[request_buffer[2]-1][i] = '0';
        }
        else {
          batt_data.volt[request_buffer[2]-1][i] = request_buffer[i+start];
        }
    }
    write_cyc_to_flash(cmd,(request_buffer[2]-1));
  break;

  case 6://cap
    for (int i = 0; i<8; i++){
      if(request_buffer[i+start] == '\r' || request_buffer[i+start] == 'F' || request_buffer[i+start] == 0x0){
        batt_data.cap[request_buffer[2]-1][i] = '0';
        }
        else {
          batt_data.cap[request_buffer[2]-1][i] = request_buffer[i+start];
        }
    }
    write_cyc_to_flash(cmd,(request_buffer[2]-1));
  break;

  case 7://cc_coef
    for (int i = 0; i<8; i++){
      if(request_buffer[i+start] == '\r' || request_buffer[i+start] == 'F'){
        batt_data.cc_coef[i] = '0';
        }
        else {
          batt_data.cc_coef[i] = request_buffer[i+start];
        }
    }
  write_coef_to_flash(cmd);
  break;

  case 8://sv_coef
    for (int i = 0; i<8; i++){
      if(request_buffer[i+start] == '\r' || request_buffer[i+start] == 'F' ){
        batt_data.sv_coef[i] = '0';
        }
        else {
          batt_data.sv_coef[i] = request_buffer[i+start];
        }
    }
  write_coef_to_flash(cmd);
  break;


  }

  *p_dev_id_addr = 1;
  }


  //TODO this is still ugly and inelegant
unsigned char string_buffer[24];
unsigned char string_buffer2[12];

void vm_ser_data (){
  uint64_t mem_buffer = *(p_dev_id_addr);
  uint64_t mem_buffer2 =  *(p_dev_id_addr + 1)<<0;
  sprintf(string_buffer,"%06x",mem_buffer);
  sprintf(string_buffer2,"%08x",mem_buffer2);
  for (int i = 0; i<4 ;i++){
    string_buffer[8+i]=string_buffer2[i];
  }
  for (int i = 0; i<12;i++){
    string_buffer[i] =  ascii_convert(string_buffer[i],1);
  }
}

void vm_date_data(){
  uint64_t mem_buffer = *(p_dev_id_addr + 2);
  sprintf(string_buffer,"%06x",mem_buffer);
  for (int i = 0; i<12;i++){
    string_buffer[i] =  ascii_convert(string_buffer[i],1);
  }
}

void vm_version_data(uint8_t cmd){
  uint64_t mem_buffer;
  uint64_t mem_buffer2;
  switch (cmd){
    case 3://hardware
      mem_buffer = *(p_dev_id_addr + 3);
      sprintf(string_buffer,"%06x",mem_buffer);
      for (int i = 0; i<12;i++){
        string_buffer[i] =  ascii_convert(string_buffer[i],1);
        }
    break;
    case 4://firmware
      mem_buffer = *(p_dev_id_addr + 4);
      mem_buffer2 = *(p_dev_id_addr + 5);
      sprintf(string_buffer,"%06x",mem_buffer);
      sprintf(string_buffer2,"%06x",mem_buffer2);
      for (int i = 0; i<12;i++){
        string_buffer[i] =  ascii_convert(string_buffer[i],1);
        string_buffer[i+8] =  ascii_convert(string_buffer2[i],1);
        }
    break;
  }
}

void vm_coef_data(uint8_t cmd){
  uint64_t mem_buffer = *(p_dev_id_addr + cmd);
  sprintf(string_buffer,"%06x",mem_buffer);
  for (int i = 0; i<12;i++){
    string_buffer[i] =  ascii_convert(string_buffer[i],1);
  }
}

void vm_volt_data(){
  for(int id = 0; id<8; id++){
    uint64_t mem_buffer = *(p_dev_id_addr+8+id);
    sprintf(string_buffer,"%06x",mem_buffer);
    for (int i = 0; i<8;i++){
      batt_data.volt[id][i] =  ascii_convert(string_buffer[i],1);
    }
  }
}

void vm_cap_data(){
  for(int id = 0; id<8; id++){
    uint64_t mem_buffer = *(p_dev_id_addr+16+id);
    sprintf(string_buffer,"%06x",mem_buffer);
    for (int i = 0; i<8;i++){
      batt_data.cap[id][i] =  ascii_convert(string_buffer[i],1);
    }
  }
}

void clear_info_str (uint8_t cmd){
  switch (cmd){
  case 1:
    for (int index = 0; index<12; index ++){
      batt_data.serial[index] = 0;
    }
  break;
  case 2:
    for (int index = 0; index<8; index ++){
      batt_data.date[index] = 0;
    }
  break;
  case 3:
    for (int index = 0; index<8; index ++){
      batt_data.hw_version[index/4][index%4] = 0;
    }
  break;
  case 4:
    for (int index = 0; index<12; index ++){
      batt_data.fw_version[index/4][index%4] = 0;
    }
  break;
  }

}


void batt_info_init(){//TODO these should have the loop inside the function, there is no need to have the two loops
  vm_ser_data();
  clear_info_str (1);
  for (int index = 0; index<12; index ++){
    batt_data.serial[index] = string_buffer[index];
  }
  vm_date_data();
  clear_info_str (2);
  for (int index = 0; index<8; index ++){
    batt_data.date[index] = string_buffer[index];
  }
  vm_version_data(3);
  clear_info_str (3);
  if(*(p_dev_id_addr + 3) != 0xFFFFFFFF){
    for (int index = 0; index<8; index ++){
      batt_data.hw_version[index/4][index%4] = string_buffer[index];
    }
 }

  vm_version_data(4);
  clear_info_str (4);
  if(*(p_dev_id_addr + 4) != 0xFFFFFFFF){
    for (int index = 0; index<12; index ++){
      batt_data.fw_version[index/4][index%4] = string_buffer[index];
    }
  }

  vm_volt_data();
  vm_cap_data();

  vm_coef_data(7);
   for (int index = 0; index<8; index ++){
    batt_data.cc_coef[index] = string_buffer[index];
  }
    vm_coef_data(8);
   for (int index = 0; index<8; index ++){
    batt_data.sv_coef[index] = string_buffer[index];
  }
}


#endif /* ZB_STORE_H__ */
