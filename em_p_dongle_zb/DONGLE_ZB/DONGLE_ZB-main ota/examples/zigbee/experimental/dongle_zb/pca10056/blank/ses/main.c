//zigbee zed with uart
//dongle hero 0.2 OTA

#include "zboss_api.h"
#include "zb_mem_config_med.h"
#include "zb_error_handler.h"
#include "zigbee_helpers.h"
#include "zb_dongle_gloabals.h"
#include "app_timer.h"
//#include "bsp.h"
#include "bsp_btn_ble.h"
#include "boards.h"
#include "uart_handler.h"
#include "nrf_drv_gpiote.h"
#include "zb_dev_defines.h"
#include "zb_store.h"
#include "zb_ble_coms.h"
#include "zb_cluster_sends.h"
#include "df_chip_check.h"
#include "zb_df_errors.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_delay.h"

#include "zb_multi_sensor.h"
#include "zb_cluster_sends.h"

#include "sdk_config.h"
#include "zboss_api_addons.h"
#include "zb_error_handler.h"
#include "zb_nrf52_internal.h"

#include "app_pwm.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "app_uart.h"
#include "nrf_gpio.h"
#include "nrfx_gpiote.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"

#include "app_util_platform.h"
#include <string.h>

// BEGIN Block Added for DFU 
#include "nrf_pwr_mgmt.h" 
#include "nus.h"
#include "nrf_dfu_ble_svci_bond_sharing.h" 
#include "nrf_svci_async_function.h" 
#include "nrf_svci_async_handler.h" 
#include "ble_dfu.h" 
#include "nrf_bootloader_info.h"  
// END Block Added for DFU

#define APP_BLE_OBSERVER_PRIO               1                                       /**< Application's BLE observer priority. You shouldn't need to modify this value. */

#define IEEE_CHANNEL_MASK                  (1l << ZIGBEE_CHANNEL)               /**< Scan only one, predefined channel to find the coordinator. */
#define ERASE_PERSISTENT_CONFIG            ZB_FALSE                             /**< Do not erase NVRAM to save the network parameters after device reboot or power-off. */

#define ZIGBEE_NETWORK_STATE_LED           BSP_BOARD_LED_2                      

#if !defined ZB_ED_ROLE
#error Define ZB_ED_ROLE to compile End Device source code.
#endif


 #define REFRESH_RESET APP_TIMER_TICKS(10000)
APP_TIMER_DEF(refresh_time_out);// the timer that resets the node network

 #define REFRESH_REBOOT APP_TIMER_TICKS(20000)
APP_TIMER_DEF(reboot_time_out);// the timer that resets the board

 APP_TIMER_DEF(zb_app_timer);


/**@brief Function for handling DFU events  *  
* @details This function is called when entering buttonless DFU  
*   
* @param[in] event Buttonless DFU event. 
*/ 
static void ble_dfu_buttonless_evt_handler(ble_dfu_buttonless_evt_type_t event) {
     
  switch (event){ 
          
    case BLE_DFU_EVT_BOOTLOADER_ENTER_PREPARE:             
      NRF_LOG_INFO("Device is preparing to enter bootloader mode\r\n");             
      break;           
    case BLE_DFU_EVT_BOOTLOADER_ENTER:            
      NRF_LOG_INFO("Device will enter bootloader mode\r\n");             
      break;           
    case BLE_DFU_EVT_BOOTLOADER_ENTER_FAILED:             
      NRF_LOG_ERROR("Device failed to enter bootloader mode\r\n");             
      break;         
    default:             
      NRF_LOG_INFO("Unknown event from ble_dfu.\r\n");             
      break;     
      
  } 

} 


/**@brief Function for handling bootloader power management events  
*  
* @details This function is called to set a persistent register which informs the   
*  bootloader it should continue or pass control back to the application  
*   
* @param[in] event Power management event.  
*/ 
static bool app_shutdown_handler(nrf_pwr_mgmt_evt_t event) {     
  switch (event)     {         
    case NRF_PWR_MGMT_EVT_PREPARE_DFU:             
      NRF_LOG_INFO("Power management wants to reset to DFU mode\r\n");              
      // Change this code to tailor to your reset strategy.             
      // Returning false here means that the device is not ready              
      // to jump to DFU mode yet.             
      //              
      // Here is an example using a variable to delay resetting the device:             
      //             
      /* if (!im_ready_for_reset){
         return false;                
      }             
      */             
      break; 

    default:             
     // Implement any of the other events available              
     // from the power management module:             
     // -NRF_PWR_MGMT_EVT_PREPARE_SYSOFF             
     // -NRF_PWR_MGMT_EVT_PREPARE_WAKEUP             
     // -NRF_PWR_MGMT_EVT_PREPARE_RESET             
     return true;     
  }     

  NRF_LOG_INFO("Power management allowed to reset to DFU mode\r\n");     
  return true; 

}  
 
NRF_PWR_MGMT_HANDLER_REGISTER(app_shutdown_handler, 0); // persistent register for determining DFU status on startup 


/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    uint32_t                              err_code;

    UNUSED_PARAMETER(p_context);

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            NRF_LOG_INFO("Connected");
            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            NRF_LOG_INFO("Disconnected");
            bsp_board_led_off(BSP_BOARD_LED_0);
            break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            NRF_LOG_DEBUG("PHY update request.");
            ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        default:
            // No implementation needed.
            break;
    }
}

void refresh_timer (){//excecute on timeout
  bdb_start_top_level_commissioning(ZB_BDB_NETWORK_FORMATION);
}

void reboot_timer (){//excecute on timeout
  zb_reset(0);
}

void ble_interupt_timer (){
  uart_external_init(0);
}


void rethink (uint8_t cmd){
  if(cmd ==  1){
    app_timer_stop(reboot_time_out);
    app_timer_stop(refresh_time_out);
    reset_counter = 0;
  }
  if(cmd == 0){
    app_timer_start(reboot_time_out,REFRESH_REBOOT,refresh_timer);
    app_timer_start(refresh_time_out,REFRESH_RESET,reboot_timer);
  } 

}

/***************************************************************************************************
 * @section Initialization
 **************************************************************************************************/

/**@brief Function for the SoftDevice initialization.
 *
 * @details This function initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}


/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
static void timers_init(void)
{
    ret_code_t err_code;

    // Initialize timer module.
    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
    app_timer_create(&uart_time_out,APP_TIMER_MODE_SINGLE_SHOT,ble_interupt_timer);
    app_timer_create(&refresh_time_out,APP_TIMER_MODE_SINGLE_SHOT,refresh_timer);
    app_timer_create(&reboot_time_out,APP_TIMER_MODE_SINGLE_SHOT,reboot_timer);
}

/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

static void ble_dfu_init(){
    
    uint32_t           err_code;

    // BEGIN Block Added for DFU 
    // ONLY ADD THIS BLOCK TO THE EXISTING FUNCTION     
    // Initialize the DFU service
         
    ble_dfu_buttonless_init_t dfus_init = {   
          
      .evt_handler = ble_dfu_buttonless_evt_handler  
         
    };     

    err_code = ble_dfu_buttonless_init(&dfus_init);     
    APP_ERROR_CHECK(err_code); 
    
    // END Block Added for DFU 

}


/**@brief Function for initializing LEDs.
 */
static void leds_init(void){
    ret_code_t error_code;

    /* Initialize LEDs and buttons - use BSP to control them. */
    error_code = bsp_init(BSP_INIT_LEDS, NULL);
    APP_ERROR_CHECK(error_code);
    bsp_board_leds_off();
}


void led_state(bool state, int8_t led_num, bool override_state){//this may not work
  if (led_states == false){
    bsp_board_led_off(0);
    bsp_board_led_off(1);
    bsp_board_led_off(2);
  }
  else{

  if (led_num > -1){
    if (state == false){
    bsp_board_led_off(led_num);
    }
    else {
    bsp_board_led_on(led_num);
    }

  } else {
    if (override_state == true){//off, to set this led_num must be -1 and  override_state must be set, otherwise override_state param doesnt change it.
      led_states = true;
    }
    else{
      led_states = false;
    }
    }
    }


}

//interupt
static void find_light_bulb_cb(zb_bufid_t bufid)
{
    zb_zdo_match_desc_resp_t   * p_resp = (zb_zdo_match_desc_resp_t *) zb_buf_begin(bufid);    // Get the beginning of the response
    zb_apsde_data_indication_t * p_ind  = ZB_BUF_GET_PARAM(bufid, zb_apsde_data_indication_t); // Get the pointer to the parameters buffer, which stores APS layer response
    zb_uint8_t                 * p_match_ep;
    zb_ret_t                     zb_err_code;

    if ((p_resp->status == ZB_ZDP_STATUS_SUCCESS) && (p_resp->match_len > 0))
    {
        /* Match EP list follows right after response header */
        p_match_ep = (zb_uint8_t *)(p_resp + 1);

        /* We are searching for exact cluster, so only 1 EP may be found */
        //m_devi_ctx.bulb_params.endpoint   = *p_match_ep;
        //m_device_ctx.bulb_params.short_addr = p_ind->src_addr;

        //NRF_LOG_INFO("Found bulb addr: %d ep: %d", m_device_ctx.bulb_params.short_addr, m_device_ctx.bulb_params.endpoint);


    }

    if (bufid)
    {
        zb_buf_free(bufid);
    }
}

#define MATCH_DESC_REQ_ROLE                 ZB_NWK_BROADCAST_ALL_DEVICES /**< All devices in PAN */

static void broadcast_pan_alert(zb_bufid_t bufid, zb_uint16_t param) 
{
    zb_zdo_match_desc_param_t * p_req;

    /* Initialize pointers inside buffer and reserve space for zb_zdo_match_desc_param_t request */
    p_req = zb_buf_initial_alloc(bufid, sizeof(zb_zdo_match_desc_param_t) + (1) * sizeof(zb_uint16_t));

    p_req->nwk_addr         = MATCH_DESC_REQ_ROLE;              // Send to devices specified by MATCH_DESC_REQ_ROLE
    p_req->addr_of_interest = MATCH_DESC_REQ_ROLE;              // Get responses from devices specified by MATCH_DESC_REQ_ROLE
    p_req->profile_id       = ZB_AF_HA_PROFILE_ID;              // Look for Home Automation profile clusters or change if another profile id is requiered in Coord/Router
    
    p_req->num_in_clusters  = 2;
    p_req->num_out_clusters = 0;

    p_req->cluster_list[0]  = 0x0414U;
    p_req->cluster_list[1]  = 0x0414U;
    /*lint -restore */
    //m_dev_ctx.bulb_params.short_addr = 0xFFFF; // Set 0xFFFF to reset short address in order to parse only one response.
    //UNUSED_RETURN_VALUE(zb_zdo_match_desc_req(bufid, zigbee_error_transmit));
    //zb_zdo_match_desc_req(bufid, zigbee_error_transmit);
        //UNUSED_RETURN_VALUE(zb_zdo_match_desc_req(bufid, find_light_bulb_cb));

    //ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
}

void get_uart_from_request (uint8_t param){
  if (uart_enable){
  int8_t cr_get = 0;
  do{
    while ( app_uart_get(&cr_get) != NRF_SUCCESS);
    if (cr_get != 0){
      request_buffer[uart_request_i] = cr_get;
      uart_request_i ++;
      }
    } 
    while (cr_get != '\r');
    nrf_delay_ms(100);
    ZB_SCHEDULE_APP_CALLBACK(request_builder, 0);
 }
}    

uint8_t uart_charge_i = 0;
bool special_case_flag = false;
int col_index = 0;
int col_buff = 0;
int shunt_index = 0;
int shunt_buff = 0;


void get_uart_from_bms (uint8_t param){
  bool uart_complete = false;
  int8_t cr_get = 0;
  app_uart_get(&cr_get);
  if (cr_get != 0){
    led_state(true,1,false);
    buffer_uart_array[trans_len_i] = cr_get;

  if(buffer_uart_array[trans_len_i] == 'C' && buffer_uart_array[trans_len_i - 1] == 'C'){
    col_index = trans_len_i; 
    }

  if (col_index != 0 && trans_len_i == (col_index + 6)){
    for (int i = 0; i<6; i++){
    col_buff = col_buff + (buffer_uart_array[trans_len_i - i]-48);
    }
    }

  if(buffer_uart_array[trans_len_i] == 'V' && buffer_uart_array[trans_len_i - 1] == 'S'){
    shunt_index = trans_len_i; 
    }

  if (shunt_index != 0 && trans_len_i == (shunt_index + 6)){
    for (int i = 0; i<6; i++){
      shunt_buff = shunt_buff + (buffer_uart_array[trans_len_i - i]-48);
       }
       }
 
  if(col_buff > 0 || shunt_buff > 0
   ){
    send_len = 72;
    } else {
      send_len = 63;
      }

    if ((buffer_uart_array [trans_len_i - 6] == 'E' 
    && buffer_uart_array [trans_len_i - 7] == 'E') ||
    (buffer_uart_array [trans_len_i- 6] == 'W'
    && buffer_uart_array [trans_len_i - 7] == 'W'))
       {
       special_case_flag = true;
       }


    trans_len_i++;
        if (
    (trans_len_i == send_len && special_case_flag == false)
    || special_case_flag == true){
      charge_set = false;
      col_buff = 0;
      shunt_buff = 0;
      trans_len_i = 0;
      special_case_flag = false;
      led_state(false,1,false);
      
      

      //this was changed from sheded
      ZB_SCHEDULE_APP_CALLBACK(bms_uart_builder, 0);

   }
    }
 }


 


 
static void zb_app_timer_handler(void * context){
  //rtc_time.seconds ++;
  //  if (rtc_time.seconds >= 86400){
  //    rtc_time.days ++;
  //    rtc_time.seconds = 0;
      
    //}
    //zb_reset(0);
}



void refresh_zb(){
  reset_counter ++;
  if (reset_counter > 30){
  //zb_reset(0);
  bdb_start_top_level_commissioning(ZB_BDB_NETWORK_FORMATION);
  reset_counter = 0;
  }
}


static void identify_handler(zb_uint8_t param)
{
    if (param)    {        NRF_LOG_INFO("Start identifying.");    }
    else    {        NRF_LOG_INFO("Stop identifying.");    }
}




/**@brief Zigbee stack event handler.
 * @param[in]   bufid   Reference to the Zigbee stack buffer used to pass signal.
 */
void zboss_signal_handler(zb_bufid_t bufid)
{
    zb_zdo_app_signal_hdr_t  * p_sg_p      = NULL;
    zb_zdo_app_signal_type_t   sig         = zb_get_app_signal(bufid, &p_sg_p);
    zb_ret_t                   status      = ZB_GET_APP_SIGNAL_STATUS(bufid);

    /* Update network status LED */
    zigbee_led_status_update(bufid, ZIGBEE_NETWORK_STATE_LED);
    
    switch (sig)
    {
        case ZB_BDB_SIGNAL_DEVICE_REBOOT:
        //bdb_start_top_level_commissioning(ZB_BDB_NETWORK_FORMATION);
        //bdb_start_top_level_commissioning(ZB_BDB_NETWORK_STEERING);
        ////bdb_start_top_level_commissioning(ZB_BDB_NETWORK_STEERING);
        

            break;
        case ZB_BDB_SIGNAL_DEVICE_FIRST_START:
        //bdb_start_top_level_commissioning(ZB_BDB_REJOIN);
        ////bdb_start_top_level_commissioning(ZB_BDB_NETWORK_FORMATION);
        
            break;
        case ZB_BDB_SIGNAL_STEERING:
            /* Call default signal handler. */
            ZB_ERROR_CHECK(zigbee_default_signal_handler(bufid));
            if (status == RET_OK)
            {
                //ret_code_t err_code = app_timer_start(zb_app_timer, APP_TIMER_TICKS(5000), NULL);
                //APP_ERROR_CHECK(err_code);
            }
            break;

        default:
            /* Call default signal handler. */
            ZB_ERROR_CHECK(zigbee_default_signal_handler(bufid));
            break;
    }

    if (bufid)
    {
        zb_buf_free(bufid);
    }
}

/**@brief Function for application main entry.
 */
int main(void){
    uint32_t err_code;

    zb_ret_t       zb_err_code;
    zb_ieee_addr_t ieee_addr;

    timers_init();
    leds_init();

    //get_all_defs();

    power_management_init();

    /* Bluetooth initialization. */
    ble_stack_init();
    /* NUS Commander initialization. */
    nus_init(NULL);

    ble_dfu_init();

    /* Create Timer for reporting attribute */
    //this timer is to start the rtc, not using it at this time
    err_code = app_timer_create(&zb_app_timer, APP_TIMER_MODE_REPEATED, zb_app_timer_handler);
    APP_ERROR_CHECK(err_code);

    
    

        // Initialize GPIOTE module
    nrfx_gpiote_init();

    //Configure Input
    nrf_gpio_cfg_input(INPUT_1, NRF_GPIO_PIN_NOPULL);     //Board already has a PullUp
    //nrf_gpio_cfg_output(OUTPUT_1);
    

    // Setting and enable configuration of interruption GPIO
    nrfx_gpiote_in_config_t gpio_config = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);
    nrfx_err_t nrfx_err_code = nrfx_gpiote_in_init(INPUT_1, &gpio_config, gpio_interrupt_handler);
    if (nrfx_err_code != NRFX_SUCCESS)
    {
        // error code
    }

    // Enable GPIO interruption detection
    nrfx_gpiote_in_event_enable(INPUT_1, true);

    /* Set Zigbee stack logging level and traffic dump subsystem. */
    ZB_SET_TRACE_LEVEL(ZIGBEE_TRACE_LEVEL);
    ZB_SET_TRACE_MASK(ZIGBEE_TRACE_MASK);
    ZB_SET_TRAF_DUMP_OFF();

    /* Initialize Zigbee stack. */
    ZB_INIT("Smart Battery Dongle");

    /* Set device address to the value read from FICR registers. */
    zb_osif_get_ieee_eui64(ieee_addr);
    zb_set_long_address(ieee_addr);

    /* Set static long IEEE address. */
    zb_set_network_ed_role(IEEE_CHANNEL_MASK);
    zigbee_erase_persistent_storage(ERASE_PERSISTENT_CONFIG);//TODO this was commented out
    

    zb_set_nvram_erase_at_start(get_start_nv_param());
    //zb_set_nvram_erase_at_start(ZB_TRUE);//true will repair with provision, false will not

    zb_set_ed_timeout(ED_AGING_TIMEOUT_64MIN);
    zb_set_keepalive_timeout(ZB_MILLISECONDS_TO_BEACON_INTERVAL(3000));
    zb_set_rx_on_when_idle(ZB_FALSE);

    /* Initialize application context structure. */
    UNUSED_RETURN_VALUE(ZB_MEMSET(&m_dev_ctx, 0, sizeof(m_dev_ctx)));

    /* Register temperature sensor device context (endpoints). */
    ZB_AF_REGISTER_DEVICE_CTX(&multi_sensor_ctx);

    /* Register handlers to identify notifications */
    ZB_AF_SET_IDENTIFY_NOTIFICATION_HANDLER(MULTI_SENSOR_ENDPOINT, identify_handler);

    /* Initialize sensor device attibutes */
    multi_sensor_clusters_attr_init();

    /** Start Zigbee Stack. */
    zb_err_code = zboss_start_no_autostart();
    //zb_err_code = zboss_start();
    ZB_ERROR_CHECK(zb_err_code);

    reset_error_counter();
    
    alert_counters_init();
    time_init();

    nrf_delay_ms(500);
    bsp_board_led_on(BSP_BOARD_LED_0);
    init_uart();
    uart_external_init (0);
    //chip_check();
    //batt_info_init();
    //zigbee_info ();
    zigbee_error_init();
    
    
    while(1)
    {
        zboss_main_loop_iteration();
        //ZB_SCHEDULE_APP_CALLBACK(get_uart, 0);//this is not working, this is to thankle the place of ..from_bms and ..from_request
        ZB_SCHEDULE_APP_CALLBACK(fet_error, 0);
        ZB_SCHEDULE_APP_CALLBACK(get_uart_from_bms, 0); 
       ZB_SCHEDULE_APP_CALLBACK(get_uart_from_request, 0);
       
    }
}


/**
 * @}
 */
