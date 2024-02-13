/**
 * Copyright (c) 2018 - 2022, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 *
 * @defgroup zigbee_examples_multiprotocol_nus_switch main.c
 * @{
 * @ingroup  zigbee_examples
 * @brief    UART over BLE application with Zigbee HA Color Dimmer Switch profile.
 *
 * This file contains the source code for a sample application that uses the Nordic UART service
 * and a color dimmer light switch operating a Zigbee network.
 * This application uses the @ref srvlib_conn_params module.
 */
#include "zboss_api.h"
#include "zboss_api_addons.h"
#include "zb_mem_config_max.h"
#include "zb_error_handler.h"
#include "zigbee_color_dimmer_switch.h"
#include "zb_zcl_identify.h"
#include "zigbee_helpers.h"

#include "nrf_pwr_mgmt.h"

#include "nus.h"

#include "app_timer.h"
#include "boards.h"
#include "bsp_btn_ble.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


// BEGIN Block Added for DFU  
#include "nrf_dfu_ble_svci_bond_sharing.h" 
#include "nrf_svci_async_function.h" 
#include "nrf_svci_async_handler.h" 
#include "ble_dfu.h" 
#include "nrf_bootloader_info.h"  
// END Block Added for DFU 

#ifdef ARDUINO_JOYSTICK_SHIELD_V1A
#include "joystick.h"
#endif

#define APP_BLE_OBSERVER_PRIO               1                                       /**< Application's BLE observer priority. You shouldn't need to modify this value. */
#define IEEE_CHANNEL_MASK                   (1l << ZIGBEE_CHANNEL)                  /**< Scan only one, predefined channel to find the coordinator. */
#define LIGHT_SWITCH_ENDPOINT_I             1                                       /**< First source endpoint used to control light bulb(s). */
#define LIGHT_SWITCH_ENDPOINT_II            2                                       /**< Second source endpoint used to control light bulb(s). */
#define LIGHT_SWITCH_ENDPOINT_III           3                                       /**< Third source endpoint used to control light bulb(s). */
#define MATCH_DESC_REQ_START_DELAY          (2 * ZB_TIME_ONE_SECOND)                /**< Delay between the light switch startup and light bulb finding procedure. */
#define MATCH_DESC_REQ_TIMEOUT              (5 * ZB_TIME_ONE_SECOND)                /**< Timeout for finding procedure. */
#define ERASE_PERSISTENT_CONFIG             ZB_FALSE                                /**< Do not erase NVRAM to save the network parameters after device reboot or power-off. NOTE: If this option is set to ZB_TRUE then do full device erase for all network devices before running other samples. */
#define FINDING_N_BINDING_STATE_LED         BSP_BOARD_LED_1                         /**< LED indicating that Finding & Binding process is ongoing. */
#define ZIGBEE_NETWORK_STATE_LED            BSP_BOARD_LED_2                         /**< LED indicating that light switch successfully joind Zigbee network. */
#define ENDPOINT_SELECTED_LED               BSP_BOARD_LED_3                         /**< LED indicating which endpoint is selected (number of pulses equals the number of endpoint) */
#define LIGHT_SWITCH_BUTTON_ON              BSP_BOARD_BUTTON_0                      /**< Button ID used to switch on the light bulb. */
#define LIGHT_SWITCH_BUTTON_OFF             BSP_BOARD_BUTTON_1                      /**< Button ID used to switch off the light bulb. */
#define SLEEPY_ON_BUTTON                    BSP_BOARD_BUTTON_2                      /**< Button ID used to determine if we need the sleepy device behaviour at start (pressed means yes). */
#define SWITCH_ENDPOINT_BUTTON              BSP_BOARD_BUTTON_2                      /**< Button ID used to switch to the next Ligt Switch endpoint. */
#define FINDING_N_BINDING_BUTTON            BSP_BOARD_BUTTON_3                      /**< Button ID used to initiate Finding & Binding procedure. */

#define LIGHT_SWITCH_DIM_STEP               15                                      /**< Dim step size - increases/decreses current level (range 0x00 - 0xFE). */
#define LIGHT_SWITCH_DIM_TRANSITION_TIME    2                                       /**< Transition time for a single step operation in 0.1 sec units. 0xFFFF - immediate change. */
#define LIGHT_SWITCH_COLOR_TRANSITION_TIME  0                                       /**< Transition time for a move to hue and saturation operation in 0.1 sec units. 0xFFFF - immediate change. */

#define LIGHT_SWITCH_BUTTON_THRESHOLD       ZB_TIME_ONE_SECOND                      /**< Number of beacon intervals the button should be pressed to dimm the light bulb. */
#define LIGHT_SWITCH_BUTTON_SHORT_POLL_TMO  ZB_MILLISECONDS_TO_BEACON_INTERVAL(50)  /**< Delay between button state checks used in order to detect button long press. */
#define LIGHT_SWITCH_BUTTON_LONG_POLL_TMO   ZB_MILLISECONDS_TO_BEACON_INTERVAL(300) /**< Time after which the button state is checked again to detect button hold - the dimm command is sent again. */

#define LED_ENDPOINT_INDICATION_ON_TIME_MS  750                                     /**< Period of LED indication staying on. */
#define LED_ENDPOINT_INDICATION_OFF_TIME_MS 3000                                    /**< Period of LED indication staying off. */

/* NOTE: Any numeric value within range 0 - 999 received over BLE UART will start a delayed toggle operation. */
#define COMMAND_ON                          "n"                                     /**< UART command that will turn on found light bulb(s). */
#define COMMAND_OFF                         "f"                                     /**< UART command that will turn off found light bulb(s). */
#define COMMAND_TOGGLE                      "t"                                     /**< UART command that will turn toggle found light bulb(s). */
#define COMMAND_INCREASE                    "i"                                     /**< UART command that will increase brightness of found light bulb(s). */
#define COMMAND_DECREASE                    "d"                                     /**< UART command that will decrease brightness of found light bulb(s). */
#define COMMAND_SWITCH_NEXT_EP              "sn"                                    /**< UART command that will switch the active endpoint to the next one. */
#define COMMAND_SWITCH_PREV_EP              "sp"                                    /**< UART command that will switch the active endpoint to the previous one. */
#define COMMAND_FIND_N_BIND                 "&"                                     /**< UART command that will start the Finding & Binding procedure. */
#define COMMAND_HUE_SATURATION              "h*s*"                                  /**< UART command description that will change hue to first * and saturation to the second * of found light bulb(s). */
#define COMMAND_TOGGLE_DELAY                "*"                                     /**< UART command description that will turn toggle found light bulb(s) after * seconds. */
#define DELAYED_COMMAND_RETRY_MS            100                                     /**< If sending toggle command was impossible due tothe lack of Zigbee buffers, retry sending it after DELAYED_COMMAND_RETRY_MS ms. */

#if !defined ZB_ED_ROLE
#error Define ZB_ED_ROLE to compile light switch (End Device) source code.
#endif


/* Variables used to recognize the type of button press. */
typedef struct light_switch_button_s
{
    zb_bool_t in_progress;
    zb_time_t timestamp;
} light_switch_button_t;

typedef struct find_n_bind_s
{
    zb_uint8_t current_ep;
} find_n_bind_t;

/* Main application customizable context. Stores all settings and static values. */
typedef struct light_switch_ctx_s
{
    light_switch_button_t      button;
    find_n_bind_t              find_n_bind;
} light_switch_ctx_t;


static void light_switch_send_delayed_toggle(void * p_context);
static void led_indicate_endpoint(void * p_context);
static void led_indicate_endpoint_cb(void * p_context);

APP_TIMER_DEF(m_toggle_timer);                                                      /**< APP timer that is responsible for sending a delayed Zigbee toggle command. */
APP_TIMER_DEF(m_led_indication_timer);                                              /**< APP timer that is responsible for inidicating the selected endpoint. */
APP_TIMER_DEF(m_led_indication_timer_aux);                                          /**< Auxiliary APP timer for the LED inidication timer. */


static light_switch_ctx_t m_device_ctx;

/* Declare context variable, cluster, and attribute list for first endpoint */
static zb_color_dimmer_switch_dev_ctx_t zb_dev_ctx_first;
ZB_DECLARE_COLOR_DIMMER_SWITCH_CLUSTER_ATTR_LIST(zb_dev_ctx_first, color_dimmer_switch_clusters_first);

/* Declare context variable, cluster, and attribute list for second endpoint */
static zb_color_dimmer_switch_dev_ctx_t zb_dev_ctx_second;
ZB_DECLARE_COLOR_DIMMER_SWITCH_CLUSTER_ATTR_LIST(zb_dev_ctx_second, color_dimmer_switch_clusters_second);

/* Declare context variable, cluster, and attribute list for third endpoint */
static zb_color_dimmer_switch_dev_ctx_t zb_dev_ctx_third;
ZB_DECLARE_COLOR_DIMMER_SWITCH_CLUSTER_ATTR_LIST(zb_dev_ctx_third, color_dimmer_switch_clusters_third);

/* Declare endpoints for Dimmer Switch device. */
ZB_ZCL_DECLARE_COLOR_DIMMER_SWITCH_EP(color_dimmer_switch_ep_i,
                                      LIGHT_SWITCH_ENDPOINT_I,
                                      color_dimmer_switch_clusters_first);

ZB_ZCL_DECLARE_COLOR_DIMMER_SWITCH_EP(color_dimmer_switch_ep_ii,
                                      LIGHT_SWITCH_ENDPOINT_II,
                                      color_dimmer_switch_clusters_second);

ZB_ZCL_DECLARE_COLOR_DIMMER_SWITCH_EP(color_dimmer_switch_ep_iii,
                                      LIGHT_SWITCH_ENDPOINT_III,
                                      color_dimmer_switch_clusters_third);


/* Declare application's device context (list of registered endpoints) for Dimmer Switch device. */
ZBOSS_DECLARE_DEVICE_CTX_EP_VA(color_dimmer_switch_ctx, &color_dimmer_switch_ep_i, &color_dimmer_switch_ep_ii, &color_dimmer_switch_ep_iii);


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


/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/**@brief Function for initializing the application timer.
 */
static void timer_init(void)
{
    uint32_t error_code;
    error_code          = app_timer_init();
    APP_ERROR_CHECK(error_code);

    error_code = app_timer_create(&m_toggle_timer, APP_TIMER_MODE_SINGLE_SHOT, light_switch_send_delayed_toggle);
    APP_ERROR_CHECK(error_code);

    error_code = app_timer_create(&m_led_indication_timer, APP_TIMER_MODE_REPEATED, led_indicate_endpoint);
    APP_ERROR_CHECK(error_code);

    error_code = app_timer_create(&m_led_indication_timer_aux, APP_TIMER_MODE_SINGLE_SHOT, led_indicate_endpoint_cb);
    APP_ERROR_CHECK(error_code);
}


/***************************************************************************************************
 * @section Zigbee stack related functions.
 **************************************************************************************************/

/**@brief Function for initializing clusters attributes.
 *
 * @param[IN]  p_device_ctx  Pointer to structure with device_ctx.
 */
static void light_switch_clusters_attr_init(zb_color_dimmer_switch_dev_ctx_t * p_device_ctx)
{
    /* Basic cluster attributes data */
    p_device_ctx->basic_attr.zcl_version   = ZB_ZCL_VERSION;
    p_device_ctx->basic_attr.power_source = ZB_ZCL_BASIC_POWER_SOURCE_UNKNOWN;

    /* Identify cluster attributes data */
    p_device_ctx->identify_attr.identify_time = ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE;
}

/**@brief Function for sending ON/OFF requests to the light bulb.
 *
 * @param[in]   bufid    Non-zero reference to Zigbee stack buffer that will be used to construct on/off request.
 * @param[in]   on_off   Requested state of the light bulb.
 */
static void light_switch_identify_blink(zb_bufid_t bufid)
{
    zb_uint16_t addr = 0;

    NRF_LOG_INFO("Send Identify Trigger Effect Blink command");

    ZB_ZCL_IDENTIFY_SEND_TRIGGER_VARIANT_REQ_NO_APS_ACK(
                        bufid,
                        addr,
                        ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT,
                        0,
                        m_device_ctx.find_n_bind.current_ep,
                        ZB_AF_HA_PROFILE_ID,
                        ZB_ZCL_DISABLE_DEFAULT_RESPONSE,
                        NULL,
                        ZB_ZCL_IDENTIFY_EFFECT_ID_BLINK,
                        ZB_ZCL_IDENTIFY_EFFECT_ID_VARIANT_DEFAULT);
}

/**@brief Function for switching to the next Light Switch endpoint.
 * 
 * @param[in]   next    Boolean to indicate should the next or previous endpoint from the list to be selected.
 */
static void cycle_endpoint(zb_bool_t next)
{
    uint32_t error_code;
    zb_ret_t zb_err_code;

    /* Stop the timers which drive the indication LED. */
    error_code = app_timer_stop(m_led_indication_timer_aux);
    APP_ERROR_CHECK(error_code);
    error_code = app_timer_stop(m_led_indication_timer);
    APP_ERROR_CHECK(error_code);

    bsp_board_led_off(ENDPOINT_SELECTED_LED);

    static zb_uint8_t const ep_cycle_map[] = {LIGHT_SWITCH_ENDPOINT_I, LIGHT_SWITCH_ENDPOINT_II, LIGHT_SWITCH_ENDPOINT_III};
    static int ep_cycle_map_len = sizeof(ep_cycle_map)/sizeof(ep_cycle_map[0]);
    int idx = 0;

    while (ep_cycle_map[idx] != m_device_ctx.find_n_bind.current_ep)
    {
        idx++;
    }

    idx = next ? idx + 1 : idx - 1;
    
    if (idx == ep_cycle_map_len)
    {
        m_device_ctx.find_n_bind.current_ep = ep_cycle_map[0];
    }
    else if (idx == -1)
    {
        m_device_ctx.find_n_bind.current_ep = ep_cycle_map[ep_cycle_map_len - 1];
    }
    else
    {
        m_device_ctx.find_n_bind.current_ep = ep_cycle_map[idx];
    }

    NRF_LOG_INFO("Switched to the endpoint %d", m_device_ctx.find_n_bind.current_ep);

    /* Resume the indication LED procedure. */
    error_code = app_timer_start(m_led_indication_timer, APP_TIMER_TICKS(LED_ENDPOINT_INDICATION_OFF_TIME_MS), NULL);
    APP_ERROR_CHECK(error_code);

    zb_err_code = zb_buf_get_out_delayed(light_switch_identify_blink);
    UNUSED_VARIABLE(zb_err_code);
}

/**@brief Callback to finding and binding procedure.
 *
 * @param[IN]   status  Procedure status.
 * @param[IN]   addr    Found device address.
 * @param[IN]   ep      Found device endpoint.
 * @param[IN]   cluster Common cluster ID.
 *
 * @return      Returned boolean value is used to decide if found device's cluster (ID given as parameter) should be bound.
 */
static zb_bool_t finding_n_binding_cb(zb_int16_t status, zb_ieee_addr_t addr, zb_uint8_t ep, zb_uint16_t cluster)
{
    zb_bool_t ret = ZB_FALSE;
    zb_char_t addr_buf[2 * 8 + 1];    /* 8 bytes (2 characters) plus one byte for null-terminator. */
    
    UNUSED_RETURN_VALUE(ieee_addr_to_str(addr_buf, sizeof(addr_buf), addr));

    switch (status)
    {
        case ZB_BDB_COMM_BIND_SUCCESS:
            NRF_LOG_INFO("Successfully bound node %s ep %hd cluster %hd",  NRF_LOG_PUSH(addr_buf), ep, cluster);
            break;

        case ZB_BDB_COMM_BIND_FAIL:
            NRF_LOG_INFO("Failed to bind node %s ep %hd cluster %hd", NRF_LOG_PUSH(addr_buf), ep, cluster);
            break;

        case ZB_BDB_COMM_BIND_ASK_USER:
            switch (cluster)
            {
                case ZB_ZCL_CLUSTER_ID_ON_OFF:
                case ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL:
                case ZB_ZCL_CLUSTER_ID_IDENTIFY:
                case ZB_ZCL_CLUSTER_ID_COLOR_CONTROL:
                    NRF_LOG_INFO("Trying to bind node %s ep %hd cluster %hd", NRF_LOG_PUSH(addr_buf), ep, cluster);
                    ret = ZB_TRUE;
                    break;
                default:
                    /* We are not interested in this cluster. */
                    break;
            }
            break;

        default:
            /* Should not happen */
            break;
    }

    return ret;
}

/**@brief Function for sending ON/OFF requests to the light bulb.
 *
 * @param[in]   bufid    Non-zero reference to Zigbee stack buffer that will be used to construct on/off request.
 * @param[in]   cmd_id   Type of the On/Off command.
 */
static void light_switch_send_on_off(zb_bufid_t bufid, zb_uint16_t cmd_id)
{
    zb_uint16_t addr = 0;

    NRF_LOG_INFO("Send ON/OFF command: 0x%02x", cmd_id);

    ZB_ZCL_ON_OFF_SEND_REQ_NO_APS_ACK(
                        bufid,
                        addr,
                        ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT,
                        0,
                        m_device_ctx.find_n_bind.current_ep,
                        ZB_AF_HA_PROFILE_ID,
                        ZB_ZCL_DISABLE_DEFAULT_RESPONSE,
                        (zb_uint8_t)cmd_id,
                        NULL);
}

/**@brief Function for getting a new Zigbee buffer and sending ON/OFF toggle request to the light bulb.
 *
 * @param[in]   p_context  Not used. Required by app_timer API.
 */
static void light_switch_send_delayed_toggle(void * p_context)
{
    zb_ret_t    zb_err_code;
    ret_code_t  err_code;

    UNUSED_PARAMETER(p_context);

    /* Request a buffer and call light_switch_send_toggle. */
    zb_err_code = zb_buf_get_out_delayed_ext(light_switch_send_on_off, ZB_ZCL_CMD_ON_OFF_TOGGLE_ID, 0);
    if (zb_err_code != RET_OK)
    {
        /* If there are no available buffers - reschedule toggle command. */
        err_code = app_timer_start(m_toggle_timer, APP_TIMER_TICKS(DELAYED_COMMAND_RETRY_MS), NULL);
        APP_ERROR_CHECK(err_code);
    }
}

/**@brief Function for indicating the selected endpoint by the number of LED flashes.
 *
 * @param[in]   p_context  Not used. Required by app_timer API.
 */
static void led_indicate_endpoint(void * p_context)
{
    uint32_t error_code;
    uint32_t ticks = (1 + 2 * (m_device_ctx.find_n_bind.current_ep - 1));
    uint32_t delay = LED_ENDPOINT_INDICATION_ON_TIME_MS/ticks;

    UNUSED_PARAMETER(p_context);

    bsp_board_led_on(ENDPOINT_SELECTED_LED);

    error_code = app_timer_start(m_led_indication_timer_aux, APP_TIMER_TICKS(delay), (void *)ticks);
    APP_ERROR_CHECK(error_code);
}

/**@brief Function for indicating the selected endpoint by the number of LED flashes.
 *
 * @param[in]   p_context  Not used. Required by app_timer API.
 */
static void led_indicate_endpoint_cb(void * p_context)
{
    uint32_t error_code;
    uint32_t ticks_left = (uint32_t)p_context;
    uint32_t ticks = (1 + 2 * (m_device_ctx.find_n_bind.current_ep - 1));
    uint32_t delay = LED_ENDPOINT_INDICATION_ON_TIME_MS/ticks;

    bsp_board_led_invert(ENDPOINT_SELECTED_LED);

    if (--ticks_left != 0)
    {
        error_code = app_timer_start(m_led_indication_timer_aux, APP_TIMER_TICKS(delay), (void *)ticks_left);
        APP_ERROR_CHECK(error_code);
    }
}

/**@brief Function for sending step requests to the light bulb.
  *
  * @param[in]   bufid        Non-zero reference to Zigbee stack buffer that will be used to construct step request.
  * @param[in]   cmd_id       Command of the Level Cluster.
  */
static void light_switch_send_step(zb_bufid_t bufid, zb_uint16_t cmd_id)
{
    zb_uint16_t addr = 0;

    NRF_LOG_INFO("Send step level command: 0x%02x", cmd_id);

    ZB_ZCL_LEVEL_CONTROL_SEND_STEP_REQ_NO_APS_ACK(
                                    bufid,
                                    addr,
                                    ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT,
                                    0,
                                    m_device_ctx.find_n_bind.current_ep,
                                    ZB_AF_HA_PROFILE_ID,
                                    ZB_ZCL_DISABLE_DEFAULT_RESPONSE,
                                    NULL,
                                    (zb_uint8_t)cmd_id,
                                    LIGHT_SWITCH_DIM_STEP,
                                    LIGHT_SWITCH_DIM_TRANSITION_TIME);
}

/**@brief Function for sending move to hue saturation requests to the light bulb.
 *
 * @param[in]   bufid            Non-zero reference to Zigbee stack buffer that will be used to construct step request.
 * @param[in]   hue_saturation   Hue and saturation values squashed into a signle value. MSB represents hue, LSB represents saturation.
 */
static void light_switch_send_move_to_hue_saturation(zb_bufid_t bufid, zb_uint16_t hue_saturation)
{
    zb_uint16_t addr       = 0;
    zb_uint8_t  hue        = hue_saturation >> 8;
    zb_uint8_t  saturation = hue_saturation & 0x00FF;

    NRF_LOG_INFO("Send move to hue saturation command: hue = 0x%02x saturation = 0x%02x", hue, saturation);

    ZB_ZCL_COLOR_CONTROL_SEND_MOVE_TO_HUE_SATURATION_REQ_NO_APS_ACK(
            bufid,
            addr,
            ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT,
            0,
            m_device_ctx.find_n_bind.current_ep,
            ZB_AF_HA_PROFILE_ID,
            ZB_ZCL_DISABLE_DEFAULT_RESPONSE,
            NULL,
            hue,
            saturation,
            LIGHT_SWITCH_COLOR_TRANSITION_TIME);
}

/**@brief Callback for detecting button press duration.
 *
 * @param[in]   button   BSP Button that was pressed.
 */
static void light_switch_button_handler(zb_uint8_t button)
{
    zb_time_t   current_time;
    zb_bool_t   short_expired;
    zb_uint16_t cmd_id;
    zb_ret_t    zb_err_code;

    current_time = ZB_TIMER_GET();

    if (ZB_TIME_SUBTRACT(current_time, m_device_ctx.button.timestamp) > LIGHT_SWITCH_BUTTON_THRESHOLD)
    {
        short_expired = ZB_TRUE;
    }
    else
    {
        short_expired = ZB_FALSE;
    }

    /* Check if button was released during LIGHT_SWITCH_BUTTON_SHORT_POLL_TMO. */
    if (!bsp_button_is_pressed(button))
    {
        if (!short_expired)
            {
                /* Allocate output buffer and send on/off command. */
            cmd_id = button == LIGHT_SWITCH_BUTTON_ON ? ZB_ZCL_CMD_ON_OFF_ON_ID : ZB_ZCL_CMD_ON_OFF_OFF_ID;
            zb_err_code = zb_buf_get_out_delayed_ext(light_switch_send_on_off, cmd_id, 0);
                if (zb_err_code == RET_ERROR)
                {
                    NRF_LOG_WARNING("No frame sent - out of buffers. Wait a bit.");
                }
                else
                {
                    ZB_ERROR_CHECK(zb_err_code);
                }
            }

        /* Button released - wait for accept next event. */
        m_device_ctx.button.in_progress = ZB_FALSE;
    }
    else
    {
        if (short_expired)
        {
            /* The button is still pressed - allocate output buffer and send step command. */
            cmd_id = button == LIGHT_SWITCH_BUTTON_ON ? ZB_ZCL_LEVEL_CONTROL_STEP_MODE_UP : ZB_ZCL_LEVEL_CONTROL_STEP_MODE_DOWN;
            zb_err_code = zb_buf_get_out_delayed_ext(light_switch_send_step, cmd_id, 0);
            if (zb_err_code == RET_ERROR)
            {
                NRF_LOG_WARNING("No frame sent - out of buffers. Wait a bit.");
            }
            else
            {
                ZB_ERROR_CHECK(zb_err_code);
            }
            zb_err_code = ZB_SCHEDULE_APP_ALARM(light_switch_button_handler, button, LIGHT_SWITCH_BUTTON_LONG_POLL_TMO);
            if (zb_err_code == RET_OVERFLOW)
            {
                NRF_LOG_WARNING("Can not schedule another alarm, queue is full.");
                m_device_ctx.button.in_progress = ZB_FALSE;
            }
            else
            {
                ZB_ERROR_CHECK(zb_err_code);
            }
        }
        else
        {
            /* Wait another LIGHT_SWITCH_BUTTON_SHORT_POLL_TMO, until LIGHT_SWITCH_BUTTON_THRESHOLD will be reached. */
            zb_err_code = ZB_SCHEDULE_APP_ALARM(light_switch_button_handler, button, LIGHT_SWITCH_BUTTON_SHORT_POLL_TMO);
            if (zb_err_code == RET_OVERFLOW)
            {
                NRF_LOG_WARNING("Can not schedule another alarm, queue is full.");
                m_device_ctx.button.in_progress = ZB_FALSE;
            }
            else
            {
                ZB_ERROR_CHECK(zb_err_code);
            }
        }
    }
}

/**@brief Function to start the Finding & Binding Procedure.
 *        If the Finding & Binding Procedure was already started, cancel it.
 */
static void toggle_find_n_bind(void)
{
    zb_ret_t zb_err_code;

    zb_err_code = zb_bdb_finding_binding_initiator(m_device_ctx.find_n_bind.current_ep, finding_n_binding_cb);
    if (zb_err_code == RET_OK)
    {
        bsp_board_led_on(FINDING_N_BINDING_STATE_LED);
        NRF_LOG_INFO("F&B: Started Finding & Binding procedure on the endpoint %d.", m_device_ctx.find_n_bind.current_ep);
    }
    else if (zb_err_code == RET_BUSY)
    {
        zb_bdb_finding_binding_initiator_cancel();
    }
    else if (zb_err_code == RET_INVALID_STATE)
    {
        NRF_LOG_WARNING("Device not yet commissionned!");
    }
    else
    {
        ZB_ERROR_CHECK(zb_err_code);
    }
}

/**@brief Callback for button events.
 *
 * @param[in]   evt      Incoming event from the BSP subsystem.
 */
static void buttons_handler(bsp_event_t evt)
{
    zb_ret_t   zb_err_code;
    zb_int32_t button;

    /* Inform default signal handler about user input at the device. */
    user_input_indicate();

    switch(evt)
    {
        case BSP_EVENT_KEY_0:
            button = LIGHT_SWITCH_BUTTON_ON;
            break;

        case BSP_EVENT_KEY_1:
            button = LIGHT_SWITCH_BUTTON_OFF;
            break;

        case BSP_EVENT_KEY_2:
            cycle_endpoint(ZB_TRUE);
            return;

        case BSP_EVENT_KEY_3:
            toggle_find_n_bind();
            return;

#ifdef ARDUINO_JOYSTICK_SHIELD_V1A
        case BSP_EVENT_KEY_4:
            cycle_endpoint(ZB_FALSE);
            return;
#endif

        default:
            NRF_LOG_INFO("Unhandled BSP Event received: %d", evt);
            return;
    }

    if (!m_device_ctx.button.in_progress)
    {
        m_device_ctx.button.in_progress = ZB_TRUE;
        m_device_ctx.button.timestamp = ZB_TIMER_GET();

        zb_err_code = ZB_SCHEDULE_APP_ALARM(light_switch_button_handler, button, LIGHT_SWITCH_BUTTON_SHORT_POLL_TMO);
        if (zb_err_code == RET_OVERFLOW)
        {
            NRF_LOG_WARNING("Can not schedule another alarm, queue is full.");
            m_device_ctx.button.in_progress = ZB_FALSE;
        }
        else
        {
            ZB_ERROR_CHECK(zb_err_code);
        }
    }
}


/**@brief Function for initializing LEDs and buttons.
 */
static void leds_buttons_init(void)
{
    ret_code_t error_code;

    /* Initialize LEDs and buttons - use BSP to control them. */
    error_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, buttons_handler);
    APP_ERROR_CHECK(error_code);
    /* By default the bsp_init attaches BSP_KEY_EVENTS_{0-4} to the PUSH events of the corresponding buttons. */

    bsp_board_leds_off();
}

/**@brief Function to set the Sleeping Mode according to the SLEEPY_ON_BUTTON state.
*/
static void sleepy_device_setup(void)
{
    zb_set_rx_on_when_idle(bsp_button_is_pressed(SLEEPY_ON_BUTTON) ? ZB_FALSE : ZB_TRUE);

#if ! defined DISABLE_POWER_CONSUMPTION_OPTIMIZATION
    /* If sleepy behaviour is enabled, power off unused RAM to save maximum energy */
    if (ZB_PIBCACHE_RX_ON_WHEN_IDLE() == ZB_FALSE)
    {
        zigbee_power_down_unused_ram();
    }
#endif /* ! defined DISABLE_POWER_CONSUMPTION_OPTIMIZATION */
}

#ifdef ARDUINO_JOYSTICK_SHIELD_V1A
/**@brief Callback to run when the joystick-based values of hue and saturation have been calculated. */
static void joystick_cb (void * p_context)
{
    zb_uint32_t hue_saturation = (zb_uint32_t) p_context;

    if (ZB_JOINED())
    {
        UNUSED_RETURN_VALUE(zb_buf_get_out_delayed_ext(light_switch_send_move_to_hue_saturation, (zb_uint16_t)hue_saturation, 0));
    }
}
#endif

/**@brief Function to handle identify notification events.
 *
 * @param[IN]   param   Parameter handler is called with.
 * @param[IN]   ep      Endpoint number.
 */
static void identify_handler(zb_uint8_t param, zb_uint8_t ep)
{
    if (param)
    {
        NRF_LOG_INFO("Start identifying on endpoint: %d.", ep);
    }
    else
    {
        NRF_LOG_INFO("Stop identifying on endpoint: %d.", ep);
    }
}

/**@brief Function to handle identify notification events.
 *
 * @param[IN]   param   Parameter handler is called with.
 */
static void identify_handler_i(zb_uint8_t param)
{
    identify_handler(param, LIGHT_SWITCH_ENDPOINT_I);
}

/**@brief Function to handle identify notification events.
 *
 * @param[IN]   param   Parameter handler is called with.
 */
static void identify_handler_ii(zb_uint8_t param)
{
    identify_handler(param, LIGHT_SWITCH_ENDPOINT_II);
}

/**@brief Function to handle identify notification events.
 *
 * @param[IN]   param   Parameter handler is called with.
 */
static void identify_handler_iii(zb_uint8_t param)
{
    identify_handler(param, LIGHT_SWITCH_ENDPOINT_III);
}

/**@brief Function for initializing the Zigbee Stack
 */
static void zigbee_init(void)
{
    zb_ieee_addr_t ieee_addr;

    /* Set Zigbee stack logging level and traffic dump subsystem. */
    ZB_SET_TRACE_LEVEL(ZIGBEE_TRACE_LEVEL);
    ZB_SET_TRACE_MASK(ZIGBEE_TRACE_MASK);
    ZB_SET_TRAF_DUMP_OFF();

    /* Initialize Zigbee stack. */
    ZB_INIT("light_switch_nus");

    /* Set device address to the value read from FICR registers. */
    zb_osif_get_ieee_eui64(ieee_addr);
    zb_set_long_address(ieee_addr);

    /* Set up Zigbee protocol main parameters. */
    zb_set_network_ed_role(IEEE_CHANNEL_MASK);
    zigbee_erase_persistent_storage(ERASE_PERSISTENT_CONFIG);

    zb_set_ed_timeout(ED_AGING_TIMEOUT_64MIN);
    zb_set_keepalive_timeout(ZB_MILLISECONDS_TO_BEACON_INTERVAL(3000));
    sleepy_device_setup();

    /* Register dimmer switch device context (endpoints). */
    ZB_AF_REGISTER_DEVICE_CTX(&color_dimmer_switch_ctx);

    /* Register handlers to identify notifications */
    ZB_AF_SET_IDENTIFY_NOTIFICATION_HANDLER(LIGHT_SWITCH_ENDPOINT_I, identify_handler_i);
    ZB_AF_SET_IDENTIFY_NOTIFICATION_HANDLER(LIGHT_SWITCH_ENDPOINT_II, identify_handler_ii);
    ZB_AF_SET_IDENTIFY_NOTIFICATION_HANDLER(LIGHT_SWITCH_ENDPOINT_III, identify_handler_iii);

    /* Initialize cluster's attributes. */
    light_switch_clusters_attr_init(&zb_dev_ctx_first);
    light_switch_clusters_attr_init(&zb_dev_ctx_second);
    light_switch_clusters_attr_init(&zb_dev_ctx_third);

    /* Set the current endpoint to the first one */
    m_device_ctx.find_n_bind.current_ep = LIGHT_SWITCH_ENDPOINT_I;
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

/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
static void idle_state_handle(void)
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}

/**@brief Zigbee stack event handler.
 *
 * @param[in]   bufid   Reference to the Zigbee stack buffer used to pass signal.
 */
void zboss_signal_handler(zb_bufid_t bufid)
{
    zb_zdo_app_signal_hdr_t  * p_sg_p = NULL;
    zb_zdo_app_signal_type_t   sig    = zb_get_app_signal(bufid, &p_sg_p);

    /* Update network status LED */
    zigbee_led_status_update(bufid, ZIGBEE_NETWORK_STATE_LED);

    switch(sig)
    {
        case ZB_BDB_SIGNAL_FINDING_AND_BINDING_INITIATOR_FINISHED:
            {
                zb_zdo_signal_fb_initiator_finished_params_t * f_n_b_status = ZB_ZDO_SIGNAL_GET_PARAMS(p_sg_p, zb_zdo_signal_fb_initiator_finished_params_t);
                switch(f_n_b_status->status)
                {
                    case ZB_ZDO_FB_INITIATOR_STATUS_SUCCESS:
                        NRF_LOG_INFO("F&B: Remote peer has been bound");
                        break;

                    case ZB_ZDO_FB_INITIATOR_STATUS_CANCEL:
                        NRF_LOG_INFO("F&B: Initiator process was canceled");
                        break;

                    case ZB_ZDO_FB_INITIATOR_STATUS_ALARM:
                        NRF_LOG_INFO("F&B: Initiator process was timed out");
                        break;

                    case ZB_ZDO_FB_INITIATOR_STATUS_ERROR:
                        NRF_LOG_ERROR("F&B: Error.");
                        break;

                    default:
                        NRF_LOG_ERROR("F&B: Unknown error (status %d)", f_n_b_status->status);
                        break;
                }
            }
            bsp_board_led_off(FINDING_N_BINDING_STATE_LED);
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

static void hue_saturation_cmd(int count, uint32_t h, uint32_t s)
{
    UNUSED_PARAMETER(count);
    int32_t   hue = h & 0xFF;
    int32_t   saturation = s & 0xFF;

    UNUSED_RETURN_VALUE(zb_buf_get_out_delayed_ext(light_switch_send_move_to_hue_saturation, (hue << 8) | saturation, 0));
}

static void finding_n_binding_cmd(int count)
{
    UNUSED_PARAMETER(count);
    toggle_find_n_bind();
}

static void turn_on_cmd(int count)
{
    UNUSED_PARAMETER(count);
    UNUSED_RETURN_VALUE(zb_buf_get_out_delayed_ext(light_switch_send_on_off, ZB_ZCL_CMD_ON_OFF_ON_ID, 0));
}

static void turn_off_cmd(int count)
{
    UNUSED_PARAMETER(count);
    UNUSED_RETURN_VALUE(zb_buf_get_out_delayed_ext(light_switch_send_on_off, ZB_ZCL_CMD_ON_OFF_OFF_ID, 0));
}

static void toggle_cmd(int count)
{
    UNUSED_PARAMETER(count);
    UNUSED_RETURN_VALUE(zb_buf_get_out_delayed_ext(light_switch_send_on_off, ZB_ZCL_CMD_ON_OFF_TOGGLE_ID, 0));
}

static void increase_cmd(int count)
{
    UNUSED_PARAMETER(count);
    UNUSED_RETURN_VALUE(zb_buf_get_out_delayed_ext(light_switch_send_step, ZB_ZCL_LEVEL_CONTROL_STEP_MODE_UP, 0));
}

static void decrease_cmd(int count)
{
    UNUSED_PARAMETER(count);
    UNUSED_RETURN_VALUE(zb_buf_get_out_delayed_ext(light_switch_send_step, ZB_ZCL_LEVEL_CONTROL_STEP_MODE_DOWN, 0));
}

static void next_ep_cmd(int count)
{
    UNUSED_PARAMETER(count);
    cycle_endpoint(ZB_TRUE);
}

static void prev_ep_cmd(int count)
{
    UNUSED_PARAMETER(count);
    cycle_endpoint(ZB_FALSE);
}

static void turn_delay_cmd(int count, uint32_t delay)
{
    UNUSED_PARAMETER(count);
    uint32_t err_code;

    if (delay > 999)
    {
            NRF_LOG_INFO("Delay value out of range 0-999:");
            return;
    }

    /* Cancel previous delayed toggle command. */
    err_code = app_timer_stop(m_toggle_timer);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Schedule delay: %d", delay);

    /* Check if delay is not too short. */
    if (APP_TIMER_TICKS(delay * 1000LL) < 5)
    {
        light_switch_send_delayed_toggle(NULL);
        return;
    }

    /* Start toggle timer. */
    err_code = app_timer_start(m_toggle_timer, APP_TIMER_TICKS(delay * 1000LL), NULL);
    APP_ERROR_CHECK(err_code);
}

static void add_nus_commands(void)
{
    UNUSED_RETURN_VALUE(NUS_ADD_COMMAND(COMMAND_ON, turn_on_cmd));
    UNUSED_RETURN_VALUE(NUS_ADD_COMMAND(COMMAND_OFF, turn_off_cmd));
    UNUSED_RETURN_VALUE(NUS_ADD_COMMAND(COMMAND_TOGGLE, toggle_cmd));
    UNUSED_RETURN_VALUE(NUS_ADD_COMMAND(COMMAND_INCREASE, increase_cmd));
    UNUSED_RETURN_VALUE(NUS_ADD_COMMAND(COMMAND_DECREASE, decrease_cmd));
    UNUSED_RETURN_VALUE(NUS_ADD_COMMAND(COMMAND_SWITCH_NEXT_EP, next_ep_cmd));
    UNUSED_RETURN_VALUE(NUS_ADD_COMMAND(COMMAND_SWITCH_PREV_EP, prev_ep_cmd));
    UNUSED_RETURN_VALUE(NUS_ADD_COMMAND(COMMAND_HUE_SATURATION, hue_saturation_cmd));
    UNUSED_RETURN_VALUE(NUS_ADD_COMMAND(COMMAND_FIND_N_BIND, finding_n_binding_cmd));
    UNUSED_RETURN_VALUE(NUS_ADD_COMMAND(COMMAND_TOGGLE_DELAY, turn_delay_cmd));
}

/***************************************************************************************************
* @section Main
**************************************************************************************************/


/**@brief Function for application main entry.
 */
int main(void)
{
    zb_ret_t   zb_err_code;

    /* Initialize loging system and GPIOs. */
    log_init();
    timer_init();
    leds_buttons_init();

    power_management_init();

    /* Bluetooth initialization. */
    ble_stack_init();
    /* NUS Commander initialization. */
    nus_init(NULL);

    ble_dfu_init();
    
    /* Add commands to NUS */
    add_nus_commands();

    /* Initialize Zigbee stack. */
    zigbee_init();

#ifdef ARDUINO_JOYSTICK_SHIELD_V1A
    /* Initialize the Joystick routines. */
    joystick_init(joystick_cb);
#endif

    /* Start execution. */
    NRF_LOG_INFO("BLE Zigbee dynamic light switch example started.");

    /** Start Zigbee Stack. */
    zb_err_code = zboss_start_no_autostart();
    ZB_ERROR_CHECK(zb_err_code);

    while(1)
    {
        zboss_main_loop_iteration();
        UNUSED_RETURN_VALUE(NRF_LOG_PROCESS());
        //idle_state_handle();
    }
}


/**
 * @}
 */










 //This goes in pca10056.h file
#define RX2_PIN_NUMBER  41  
#define TX2_PIN_NUMBER  11



//zigbee zed with uart
//dongle hero 0.2
#include "zboss_api.h"
#include "zb_mem_config_max.h"//#include "zb_mem_config_med.h"
#include "zb_error_handler.h"
#include "zigbee_helpers.h"
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

uint8_t request_buffer [13] = {0};


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

bool io_1 = false;
bool io_2 = false;


/***************************************************************************************************
* @section Main
**************************************************************************************************/


/**@brief Function for application main entry.
 */
int main(void)
{

    uint32_t err_code;

    zb_ret_t       zb_err_code;
    zb_ieee_addr_t ieee_addr;

    timers_init();
    leds_init();

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

    nrf_delay_ms(500);//was 1000
    bsp_board_led_on(BSP_BOARD_LED_0);
    init_uart();
    uart_external_init (0);
    zigbee_info ();
    zigbee_error_init();

    while(1)
    {
        zboss_main_loop_iteration();
        ZB_SCHEDULE_APP_CALLBACK(get_uart_from_bms, 0); 
        ZB_SCHEDULE_APP_CALLBACK(get_uart_from_request, 0);

        //TODO this is for the wiggle wire error method, this is not built out
          //should be put into a callback shc
        if(nrf_gpio_pin_read(SIGNAL1)){
          //Error_fet
        }else{
          //No error      
        }
    }

}


/**
 * @}
 */
