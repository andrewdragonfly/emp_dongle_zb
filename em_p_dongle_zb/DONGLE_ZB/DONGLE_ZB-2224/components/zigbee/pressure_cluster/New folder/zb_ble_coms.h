#ifndef ZB_BLE_COMS__
#define ZB_BLE_COMS__


 void request_builder (uint8_t param){
  //int period = (request_buffer[1]*100) + (request_buffer[2]*10) + request_buffer[3];
 int period = (request_buffer[1]);
  ////  for (int i = 0;i<100;i++){
  ////buffer_uart_array[i]=0;
  ////}

  switch (request_buffer[0]){


      
  //    case 0xFF://All values,
  //    //this doesnt do anything ATM
  //    break;

  //    case 0xA0: //batt voltage

  //      for (int i = 0; i<10; i++){
  //        log_data_send [i] = 1000 + i*10;
  //        }
  //        send_len = 10;
  //        //ZB_SCHEDULE_APP_CALLBACK(uart_internal_init, 0);
    

  //      //data_dump[send_counter] = voltage_log_index_p[voltage_log_p_i - i].time;
  //      //data_dump[send_counter + 1] = voltage_log_index_p[voltage_log_p_i - i].value;
  //    break;

      case 0xA1: //mod 1 voltage//test
      clear_nvmc(f_error_addr);
      nrf_nvmc_write_word (f_error_addr,0x12345678);
      nrf_nvmc_write_word (f_error_addr + 4,0x22222222);
      nrf_nvmc_write_word (f_error_addr + 8,0x33333333);

      break;

     case 0xA2: //mod 2 voltage
     error_count_upadate();
     break;

  //    case 0xA3: //mod 3 voltage
  //    break;

  //    case 0xA4: //mod 4 voltage 
  //    break;

  //    case 0xAB: //mod delta voltage
  //    break;

      case 0xAF: //Zigbee info
      //ZB_SCHEDULE_APP_CALLBACK(zigbee_info_transmit, period);
      break;

  //    case 0xB0: //Coulumbs(load)
  //    break;

  //    case 0xB1: //Amps(load)
  //    break;

  //    case 0xC0: //State of Charge
  //    break;

  case 0xC1: //State of Charge %
  for(int i = 0;i<8;i++){
    buffer_uart_array [i] = Zigbee_uart.col_val>>4;
    }

  break;

      case 0xEE: //Errors

        //this will get data from mem and write it to uart buffer     

        for(int i = 0; i<period; i++){
        if(i <= error_count){//so it doesnt run off of the addr
          nvm_to_uart(request_buffer[0],(i%16)*4,i);
          }
          else {
          buffer_uart_array[(i%16)*4] = 0; //it will just write a 0

          }
        
          if(i != 0 &&((i % 16) == 0 || i == period -1)) {//buffer is full || remainder is done (last group)
            send_len = (i % 16)*4 + 4;
            buffer_uart_array[send_len] = '\r';
            ZB_SCHEDULE_APP_CALLBACK(uart_internal_init, 0);
            }
           }
        
      break;

  //    case 0xCF: //Temperature
  //    //if (temperature_log_p_i - i > 0){
  //    //  data_dump[send_counter] = temperature_log_index_p[temperature_log_p_i - i].time;
  //    //  data_dump[send_counter + 1] = temperature_log_index_p[temperature_log_p_i - i].value;
  //    //}
  //    //else{
  //    //  data_dump[send_counter] = 0;
  //    //  data_dump[send_counter + 1] = 0;
  //    //}
  //    break;

  //    case 0xD0: //Dod
  //      //data_dump[send_counter + 0] = DoD_log_index_p[DoD_log_p_i - i].time;
  //      //data_dump[send_counter + 1] = DoD_log_index_p[DoD_log_p_i - i].value;
  //    break;

  //    case 0xD1: //State log
  //      //data_dump[send_counter + 0] = charge_log_index_p[charge_log_p_i - i].time;
  //      //data_dump[send_counter + 1] = charge_log_index_p[charge_log_p_i - i].value;
  //    break;

  //    case 0xD2: //Complete Discharges
  //      //data_dump[send_counter + 1] = number_of_complete_discharges_p;
  //    break;
case 0xE0: //error clear
  Zigbee_uart.error_state_val = false;
  warning_messages(0,true);
  error_messages(0,true);
  //Zigbee_uart.ee1_val = 0;
  //Zigbee_uart.ww_val = 0;
  zb_buf_get_out_delayed_ext(broadcast_pan_alert,0,0);
break;

case 0xE1://imbalance Error
  Zigbee_uart.error_state_val = true;
  error_messages(1,false);
  //zb_buf_get_out_delayed_ext(broadcast_pan_alert,0,0);
  error_nvmc_store(1,0);
break;

case 0xE2://Over voltage error
  Zigbee_uart.error_state_val = true;
  error_messages(2,false);
  zb_buf_get_out_delayed_ext(broadcast_pan_alert,0,0);
break;

case 0xE3://under voltage error
  Zigbee_uart.error_state_val = true;
  error_messages(3,false);
  ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
break;

case 0xE4://Over Temperature error
  Zigbee_uart.error_state_val = true;
  error_messages(4,false);
  ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
break;

case 0xE5://Under Temperature error
  Zigbee_uart.error_state_val = true;
  error_messages(5,false);
  ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
break;

case 0xE6:// over current limit 1
  Zigbee_uart.error_state_val = true;
  error_messages(6,false);
  ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
break;

case 0xE7://Over current limit 2
  Zigbee_uart.error_state_val = true;
  error_messages(7,false);
  ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
break;

case 0xE8://Short Circuit Error
  Zigbee_uart.error_state_val = true;
  error_messages(8,false);
  ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
break;

case 0xE9://Imbalance Warning
  Zigbee_uart.error_state_val = true;
  warning_messages(1,false);
  ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
break;


case 0xEA://Over Voltage Warning
  Zigbee_uart.error_state_val = true;
  warning_messages(2,false);
  ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
break;

case 0xEB:// Undervoltage Warning
  Zigbee_uart.error_state_val = true;
  warning_messages(3,false);
  ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
break;

case 0xEC:// Over temperature warning
  Zigbee_uart.error_state_val = true;
  warning_messages(4,false);
  ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
break;

case 0xED://Under temperature warning
  Zigbee_uart.error_state_val = true;
  warning_messages(5,false);
  ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
break;


case 0xF0: //steering/ commisioning
  bdb_start_top_level_commissioning(ZB_BDB_NETWORK_FORMATION);
break;

case 0xF1: //Reboot
  reboot_reset(request_buffer[1]);
break;
    
default:
break;
      }
      ZB_SCHEDULE_APP_CALLBACK(uart_internal_init, 0);
      //ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);//since this is not an error call, this should not exe everytime like it is.
 }


#endif