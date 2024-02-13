
#ifndef UART_HANDLER_H__
#define UART_HANDLER_H__

//there is a lot commented out for testing 

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_error.h"
#include "app_util_platform.h"
#include "app_fifo.h"
#include "app_uart.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "boards.h"
#include "nrf_gpio.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "math.h"
#include "bsp.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "zb_dev_defines.h"
#include "zb_store.h"
#include "zb_dongle_gloabals.h"
#include "app_timer.h"


#include "nrf_gpio.h"
#include "nrfx_gpiote.h"


#if defined (UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined (UARTE_PRESENT)
#include "nrf_uarte.h"
#endif



#define INPUT_1 20

//////////////moved delete
////function declare
//uint32_t time_nvmc_update (int value);
//uint32_t time_init();
//void error_messages(uint8_t value, bool reset);
//void warning_messages(uint8_t value, bool reset);


#define MAX_TEST_DATA_BYTES     (15U)                /**< max number of test bytes to be used for tx and rx. */
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                         /**< UART RX buffer size. */
#define UART_HWFC APP_UART_FLOW_CONTROL_ENABLED

APP_TIMER_DEF(m_repeated_timer_id);
bool paired = false;

void zigbee_periodic_transmit (uint8_t param);//declaration
void zigbee_error_transmit (uint8_t param);//declaration

extern uint8_t buffer_uart_array [128]; //changed to a global extern

uint8_t trans_len_i = 0;
bool charge_set = false;
int error_code = 0, eeww_flag = 0, index_step = 0;
uint32_t index_time = 0;//this is the counter for uart tranmissions

bool uart_enable = false;
uint8_t uart_request_i = 0;
bool uart_log_request_complete_flag = false;

//struct{
//uint32_t time;
//uint16_t value;
//}m1_log[12], m2_log[12], m3_log[12], m4_log[12], voltage_log[12], load_log[12], coul_log[12],shunt_log[12],charge_log[12],temperature_log[12], ee_log[12], ww_log[12];

struct {
  uint32_t time_val;
  uint16_t m1_val;
  uint16_t m2_val;
  uint16_t m3_val;
  uint16_t m4_val;
  uint16_t sv_val;
  uint16_t cc_val;
  uint32_t tt_val;
  uint16_t col_val;
  bool     ac_val;
  bool     dc_val;
  bool     error_state_val;
  uint8_t ee1_val;
  uint8_t ee2_val;
  uint8_t ww1_val;
  uint8_t ww2_val;
}Zigbee_uart;


void init_uart (){
    nrf_gpio_cfg_output(TX_DIR);
    nrf_gpio_cfg_output(RX_DIR);
    nrf_gpio_pin_write(TX_DIR, 0);
    nrf_gpio_pin_write(RX_DIR, 1);
    //nrf_gpio_cfg_output(SIGNAL1_DIR);
    //nrf_gpio_cfg_output(SIGNAL2_DIR);
    //nrf_gpio_cfg_output(SIGNAL3_DIR);
    //nrf_gpio_pin_write(SIGNAL1_DIR, 1);
    //nrf_gpio_pin_write(SIGNAL2_DIR, 1);
    //nrf_gpio_pin_write(SIGNAL3_DIR, 1);
    //nrf_gpio_cfg_output(SIGNAL1);
    //nrf_gpio_cfg_output(SIGNAL2);
    //nrf_gpio_cfg_output(SIGNAL3);
    //nrf_gpio_pin_write(SIGNAL1, 1);
    //nrf_gpio_pin_write(SIGNAL2, 0);
    //nrf_gpio_pin_write(SIGNAL3, 1);

    nrf_gpio_cfg_input(SIGNAL1,NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(SIGNAL2,NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(SIGNAL3,NRF_GPIO_PIN_PULLUP);


 }

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR){
        //APP_ERROR_HANDLER(p_event->data.error_communication);
        }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR){
        //APP_ERROR_HANDLER(p_event->data.error_code);
        }
}



 const app_uart_comm_params_t comm_params_external = {
    RX2_PIN_NUMBER,
    TX2_PIN_NUMBER,
    UART_PIN_DISCONNECTED,
    UART_PIN_DISCONNECTED,
    UART_HWFC,
    false,
    NRF_UART_BAUDRATE_115200        
    };

  const app_uart_comm_params_t comm_params_internal = {
    RX_PIN_NUMBER,
    TX_PIN_NUMBER,
    UART_PIN_DISCONNECTED,
    UART_PIN_DISCONNECTED,
    UART_HWFC,
    false,
    NRF_UART_BAUDRATE_115200
    };

 #define UART_RESET APP_TIMER_TICKS(1000)
APP_TIMER_DEF(uart_time_out);// the timer that resets the interruption


void gpio_interrupt_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
    app_timer_start(uart_time_out,UART_RESET,NULL);
    nrfx_gpiote_uninit();
    uart_log_request_complete_flag = false;
    uint32_t err_code_uart;
 
    uart_enable = true;
      while(app_uart_close() != NRF_SUCCESS);
      while(app_uart_flush() != NRF_SUCCESS);

  nrf_delay_ms(10);
  uint32_t err_code;   
  APP_UART_FIFO_INIT(&comm_params_internal,
   UART_RX_BUF_SIZE,
   UART_TX_BUF_SIZE,
   uart_error_handle,
   APP_IRQ_PRIORITY_LOWEST,
   err_code
   );

  APP_ERROR_CHECK(err_code);


  }

void reset_the_interupt (uint8_t param){
    uart_enable = false;
    for(int i = 0; i<13; i++){request_buffer [i] = 0;}
    uart_request_i = 0;
    nrfx_gpiote_init();
    nrf_gpio_cfg_input(INPUT_1, NRF_GPIO_PIN_NOPULL);   
    nrfx_gpiote_in_config_t gpio_config = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);
    nrfx_err_t err_code = nrfx_gpiote_in_init(INPUT_1, &gpio_config, gpio_interrupt_handler);
    if (err_code != NRFX_SUCCESS)
    {
        // error code
    }
    nrfx_gpiote_in_event_enable(INPUT_1, true);
    }




void uart_external_init (uint8_t param){ 
  app_timer_stop(uart_time_out);
  uart_enable = false;
  while(app_uart_close() != NRF_SUCCESS);
  while(app_uart_flush() != NRF_SUCCESS);
  uint32_t err_code;    
  //bsp_board_led_off(1);   

  APP_UART_FIFO_INIT(
    &comm_params_external,
    UART_RX_BUF_SIZE,
    UART_TX_BUF_SIZE,
    uart_error_handle,
    APP_IRQ_PRIORITY_LOWEST,
    err_code
    );

   APP_ERROR_CHECK(err_code);
   ZB_SCHEDULE_APP_CALLBACK(reset_the_interupt,0);
  }

int send_len = 0;


void uart_internal_init (uint8_t param){ 
  while(app_uart_close() != NRF_SUCCESS);
  while(app_uart_flush() != NRF_SUCCESS);
  nrf_delay_ms(100);

  uint32_t err_code;   
  APP_UART_FIFO_INIT(&comm_params_internal,
   UART_RX_BUF_SIZE,
   UART_TX_BUF_SIZE,
   uart_error_handle,
   APP_IRQ_PRIORITY_LOWEST,
   err_code
   );

  APP_ERROR_CHECK(err_code);

  for (int i = 0;i<send_len;i++){
  app_uart_put(buffer_uart_array[i]);
  buffer_uart_array[i]=0;
  }
    nrf_delay_ms(50);
    send_len = 0;
    ZB_SCHEDULE_APP_CALLBACK(uart_external_init, 0);
}




int amps_check = 0;

//struct {
//int days;
//int seconds;
//}rtc_time, time_log[1000];


int float_time = 0;

int time_delta = 0;
int int_mod_val = 0;



  







bool full_charge (){
  time_delta ++; //count everytime it is called
  if (time_delta == 1){//first call, store the m1 val
    int_mod_val =  Zigbee_uart.m1_val;
  }
  if (time_delta > 10){
    if (Zigbee_uart.ac_val == true && Zigbee_uart.m1_val == int_mod_val){//if its charging, and the voltage is not increasing after x cycles, true:its fully charged
      return true;
    }
    else {
      time_delta = 0;// reset the clock and return false
      return false;
    }
  }
  else {
    return false;// not enough time has past
  }
}

bool float_checker (){
  int delta = abs((Zigbee_uart.m1_val + Zigbee_uart.m2_val + Zigbee_uart.m2_val+Zigbee_uart.m4_val)/4 - 3375);
  if (delta < 25){float_time++;}//25 or .025v is the tolerace of the 12v charger in float (13.4-13.6) |13.5 nominal
    else {float_time = 0;}
  if (float_time > 60) {
    return true;
  } 
  else {
    return false;
    }
}



static void broadcast_pan_alert(zb_bufid_t bufid, zb_uint16_t param) ;

void get_time_from_mem (){ //call this to update the index_time from nvm
index_time = time_init();
}

void log_time (){
      //index_time ++;
      index_time = time_init();
      time_nvmc_update(index_time);
      index_step = index_time % 12;
      Zigbee_uart.time_val = index_time;
      //time_log[index_time].seconds = rtc_time.seconds;
      //time_log[index_time].days = rtc_time.days;
      }

int convert_stream_val (int index_offset){
  int convert_buffer = 0;
 for (int i = 0; i<6; i++){
    convert_buffer = convert_buffer + (buffer_uart_array[i+index_offset+2]-48)*pow(10,(5-i));
    }
  return convert_buffer;
  }

void bms_uart_builder (uint8_t param){
  int index_of_m1 = 0, index_of_m2 = 0, index_of_m3 = 0, index_of_m4 = 0, index_of_cc = 0, index_of_tt = 0, index_of_sv = 0, index_of_ch = 0, index_of_ee = 0, index_of_ww = 0;
  bool error_flag = false, discharge_flag = false, charge_flag = false;
  int8_t c_dir = 0;
 
  for (int i=0; i<80;i++){
    if (buffer_uart_array[i] == 'M' && buffer_uart_array[i+1] == '1'){index_of_m1 = i;}
    if (buffer_uart_array[i] == 'M' && buffer_uart_array[i+1] == '2'){index_of_m2 = i;}
    if (buffer_uart_array[i] == 'M' && buffer_uart_array[i+1] == '3'){index_of_m3 = i;}
    if (buffer_uart_array[i] == 'M' && buffer_uart_array[i+1] == '4'){index_of_m4 = i;}
    if (buffer_uart_array[i] == 'C' && buffer_uart_array[i+1] == 'C'){index_of_cc = i;}
    if (buffer_uart_array[i] == 'S' && buffer_uart_array[i+1] == 'V'){index_of_sv = i;}
    if (buffer_uart_array[i] == 'T' && buffer_uart_array[i+1] == 'T'){index_of_tt = i;}
    if (buffer_uart_array[i] == 'C' && buffer_uart_array[i+1] == 'M'){
    index_of_ch = i;
    charge_set = true;
    }
    if (buffer_uart_array[i] == 'D' && buffer_uart_array[i+1] == 'C'){
    discharge_flag = true;
    }
    if (buffer_uart_array[i] == 'A' && buffer_uart_array[i+1] == 'C'){
    charge_flag = true;
    }
    
    if (buffer_uart_array[i] == 'E' && buffer_uart_array[i+1] == 'E'){
      error_count_upadate();//this will count the errors, write the count to NV if avail;
      index_of_ee = i;
      eeww_flag = 1;
      error_code = buffer_uart_array[i+2] + buffer_uart_array[i+3];
      error_messages(error_code,false);
      //Zigbee_uart.ee_val = error_code;
      //ee_log[index_step].time = index_time; 
      //ee_log[index_step].value = error_code;
      error_flag = true;
      }//end if error

    if (buffer_uart_array[i] == 'W' && buffer_uart_array[i+1] == 'W'){
      warning_count_upadate();
      index_of_ww = i;
      eeww_flag = 2;
      error_code = buffer_uart_array[i+2] + buffer_uart_array[i+3];
      warning_messages(error_code,false);
      //Zigbee_uart.ww_val = error_code;
      //ww_log[index_step].time = index_time; 
      //ww_log[index_step].value = error_code;
      error_flag = true;
      }//end if warn

      }//end for


      if (error_flag == false){
        log_time ();
        Zigbee_uart.error_state_val = false;
        
        //voltage
        //voltage_log [index_step].time = index_time;
        //voltage_log [index_step].value = convert_stream_val(index_of_m1) + convert_stream_val(index_of_m2) + convert_stream_val(index_of_m3) + convert_stream_val(index_of_m4);

        //m1_log[index_step].time = index_time;
        //m2_log[index_step].time = index_time;
        //m3_log[index_step].time = index_time;
        //m4_log[index_step].time = index_time;
        //m1_log[index_step].value = convert_stream_val(index_of_m1);
        //m2_log[index_step].value = convert_stream_val(index_of_m2);
        //m3_log[index_step].value = convert_stream_val(index_of_m3);
        //m4_log[index_step].value = convert_stream_val(index_of_m4);
        ////zigbee vars
        //Zigbee_uart.m1_val = m1_log[index_step].value;
        //Zigbee_uart.m2_val = m2_log[index_step].value;
        //Zigbee_uart.m3_val = m3_log[index_step].value;
        //Zigbee_uart.m4_val = m3_log[index_step].value;

        Zigbee_uart.m1_val = convert_stream_val(index_of_m1);
        Zigbee_uart.m2_val = convert_stream_val(index_of_m2);
        Zigbee_uart.m3_val = convert_stream_val(index_of_m3);
        Zigbee_uart.m4_val = convert_stream_val(index_of_m4);

        ////current
        //coul_log[index_step].time = index_time;
        //coul_log[index_step].value = convert_stream_val(index_of_cc);
        //Zigbee_uart.cc_val = coul_log[index_step].value;
        Zigbee_uart.cc_val = convert_stream_val(index_of_cc);
        
        //shunt_log[index_step].time = index_time;
        //shunt_log[index_step].value = convert_stream_val(index_of_sv);
        //Zigbee_uart.sv_val = shunt_log[index_step].value;
        Zigbee_uart.sv_val = convert_stream_val(index_of_sv);

        ////Temperature
        //temperature_log [index_step].time = index_time;
        //temperature_log [index_step].value = convert_stream_val(index_of_tt);
        //Zigbee_uart.tt_val = temperature_log [index_step].value;
        Zigbee_uart.tt_val = convert_stream_val(index_of_tt);
        ////charging
        //charge_log [index_step].time = index_time;
        if (charge_flag == true){
          //charge_log [index_step].value = 2;
          Zigbee_uart.ac_val = true;
          Zigbee_uart.dc_val = false;
          c_dir = 1;
          } 
        if (discharge_flag == true){
          //charge_log [index_step].value = 1;
          Zigbee_uart.dc_val = true;
          Zigbee_uart.ac_val = false;
          c_dir = -1;
          }

        if (charge_flag == false && discharge_flag == false){
          Zigbee_uart.dc_val = false;
          Zigbee_uart.ac_val = false;
          c_dir = 0;
          }

        Zigbee_uart.col_val = col_nvmc_update(float_checker(),(Zigbee_uart.cc_val*CC_COEF/1000000*c_dir));
        //amps_check = Zigbee_uart.cc_val*CC_COEF/1000000*c_dir;

        ZB_SCHEDULE_APP_CALLBACK(zigbee_periodic_transmit, 0);
        //zb_buf_get_out_delayed_ext(broadcast_pan_alert,0,0);
        ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
        }//end period send

      if (error_flag == true){
     
        Zigbee_uart.error_state_val = true;
        eeww_flag = 0;
        trans_len_i = 0;
       
        
        zb_buf_get_out_delayed_ext(broadcast_pan_alert,0,0);
        ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
        }//end error true
      
      }//end fun


      



 



#endif  /* _ UART_HANDLER_H__ */
