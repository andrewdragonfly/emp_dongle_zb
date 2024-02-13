#ifndef _ZB_CLUSTER_SENDS_H_
#define _ZB_CLUSTER_SENDS_H_

#include "zb_multi_sensor.h"

#include "sdk_config.h"
#include "zboss_api_addons.h"
#include "zb_error_handler.h"
#include "zb_nrf52_internal.h"

void zigbee_error_init (){
zb_buf_get_out_delayed_ext(broadcast_pan_alert,0,0);
  Zigbee_uart.error_state_val = false;
  error_messages(0,true);
  warning_messages(0,true);
  ZB_SCHEDULE_APP_CALLBACK(zigbee_error_transmit, 0);
}

static sensor_device_ctx_t m_dev_ctx;

ZB_ZCL_DECLARE_IDENTIFY_CLIENT_ATTRIB_LIST(identify_client_attr_list);

ZB_ZCL_DECLARE_IDENTIFY_SERVER_ATTRIB_LIST(identify_server_attr_list, &m_dev_ctx.identify_attr.identify_time);

ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT(
  basic_attr_list,
  &m_dev_ctx.basic_attr.zcl_version,
  &m_dev_ctx.basic_attr.app_version,
  &m_dev_ctx.basic_attr.stack_version,
  &m_dev_ctx.basic_attr.hw_version,
  m_dev_ctx.basic_attr.mf_name,
  m_dev_ctx.basic_attr.model_id,
  m_dev_ctx.basic_attr.date_code,
  &m_dev_ctx.basic_attr.power_source,
  m_dev_ctx.basic_attr.location_id,
  &m_dev_ctx.basic_attr.ph_env,
  m_dev_ctx.basic_attr.sw_ver
  );

ZB_ZCL_DECLARE_PRES_MEASUREMENT_ATTRIB_LIST(
  pressure_attr_list, 
  &m_dev_ctx.pres_attr.Time,
  &m_dev_ctx.pres_attr.Time_day,
  &m_dev_ctx.pres_attr.SOC,
  &m_dev_ctx.pres_attr.M1, 
  &m_dev_ctx.pres_attr.M1, 
  &m_dev_ctx.pres_attr.M2,
  &m_dev_ctx.pres_attr.M3,
  &m_dev_ctx.pres_attr.M4,
  &m_dev_ctx.pres_attr.SV,
  &m_dev_ctx.pres_attr.CC,
  &m_dev_ctx.pres_attr.TT,
  &m_dev_ctx.pres_attr.AC,
  &m_dev_ctx.pres_attr.DC,
  &m_dev_ctx.pres_attr.INFO,
  &m_dev_ctx.pres_attr.ee_status
  );

ZB_ZCL_DECLARE_PRES2_MEASUREMENT_ATTRIB_LIST(
  pressure2_attr_list, 
  &m_dev_ctx.pres2_attr.status,
  &m_dev_ctx.pres2_attr.error1, 
  &m_dev_ctx.pres2_attr.error2, 
  &m_dev_ctx.pres2_attr.warning1,
  &m_dev_ctx.pres2_attr.warning2,
  &m_dev_ctx.pres2_attr.time
  );

ZB_ZCL_DECLARE_PRES3_MEASUREMENT_ATTRIB_LIST(
  pressure3_attr_list, 
  &m_dev_ctx.pres3_attr.CC_coef,
  &m_dev_ctx.pres3_attr.SV_coef, 
  &m_dev_ctx.pres3_attr.S_cap,
  &m_dev_ctx.pres3_attr.S_volt,
  &m_dev_ctx.pres3_attr.C1,
  &m_dev_ctx.pres3_attr.C2,
  &m_dev_ctx.pres3_attr.C3,
  &m_dev_ctx.pres3_attr.C4,
  &m_dev_ctx.pres3_attr.C5,
  &m_dev_ctx.pres3_attr.C6,
  &m_dev_ctx.pres3_attr.C7,
  &m_dev_ctx.pres3_attr.C8,
  &m_dev_ctx.pres3_attr.V1,
  &m_dev_ctx.pres3_attr.V2,
  &m_dev_ctx.pres3_attr.V3,
  &m_dev_ctx.pres3_attr.V4,
  &m_dev_ctx.pres3_attr.V5,
  &m_dev_ctx.pres3_attr.V6,
  &m_dev_ctx.pres3_attr.V7,
  &m_dev_ctx.pres3_attr.V8
  );

ZB_DECLARE_MULTI_SENSOR_CLUSTER_LIST(
  multi_sensor_clusters,
  basic_attr_list,
  identify_client_attr_list,
  identify_server_attr_list,
  pressure_attr_list,
  pressure2_attr_list,
  pressure3_attr_list
  );

ZB_ZCL_DECLARE_MULTI_SENSOR_EP(
  multi_sensor_ep,
  MULTI_SENSOR_ENDPOINT,
  multi_sensor_clusters
  );

  /**@brief Function for initializing all clusters attributes.
 */
static void multi_sensor_clusters_attr_init(void)
{
    /* Basic cluster attributes data */
    m_dev_ctx.basic_attr.zcl_version   = ZB_ZCL_VERSION;
    m_dev_ctx.basic_attr.app_version   = SENSOR_INIT_BASIC_APP_VERSION;
    m_dev_ctx.basic_attr.stack_version = SENSOR_INIT_BASIC_STACK_VERSION;
    m_dev_ctx.basic_attr.hw_version    = SENSOR_INIT_BASIC_HW_VERSION;

    ZB_ZCL_SET_STRING_VAL(
      m_dev_ctx.basic_attr.mf_name,
      SENSOR_INIT_BASIC_MANUF_NAME,
      ZB_ZCL_STRING_CONST_SIZE(SENSOR_INIT_BASIC_MANUF_NAME)
      );

    ZB_ZCL_SET_STRING_VAL(
      m_dev_ctx.basic_attr.model_id,
      SENSOR_INIT_BASIC_MODEL_ID,
      ZB_ZCL_STRING_CONST_SIZE(SENSOR_INIT_BASIC_MODEL_ID)
      );

    ZB_ZCL_SET_STRING_VAL(
      m_dev_ctx.basic_attr.date_code,
      SENSOR_INIT_BASIC_DATE_CODE,
      ZB_ZCL_STRING_CONST_SIZE(SENSOR_INIT_BASIC_DATE_CODE)
      );

    m_dev_ctx.basic_attr.power_source = SENSOR_INIT_BASIC_POWER_SOURCE;

    ZB_ZCL_SET_STRING_VAL(
      m_dev_ctx.basic_attr.location_id,
      SENSOR_INIT_BASIC_LOCATION_DESC,
      ZB_ZCL_STRING_CONST_SIZE(SENSOR_INIT_BASIC_LOCATION_DESC)
      );

    m_dev_ctx.basic_attr.ph_env = SENSOR_INIT_BASIC_PH_ENV;

    /* Identify cluster attributes data */
    m_dev_ctx.identify_attr.identify_time        = ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE;

    /* Perodic measurement cluster attributes data */
    m_dev_ctx.pres_attr.Time          = ZB_ZCL_ATTR_PRES_TIME_VALUE;
    m_dev_ctx.pres_attr.Time_day      = ZB_ZCL_ATTR_PRES_TIME_RTC_DAY_VALUE;
    m_dev_ctx.pres_attr.Time_sec      = ZB_ZCL_ATTR_PRES_TIME_RTC_SECOND_VALUE;
    m_dev_ctx.pres_attr.SOC           = ZB_ZCL_ATTR_PRES_SOC_VALUE;
    m_dev_ctx.pres_attr.M1            = ZB_ZCL_ATTR_PRES_M1_VALUE;
    m_dev_ctx.pres_attr.M2            = ZB_ZCL_ATTR_PRES_M2_VALUE;
    m_dev_ctx.pres_attr.M3            = ZB_ZCL_ATTR_PRES_M3_VALUE;
    m_dev_ctx.pres_attr.M4            = ZB_ZCL_ATTR_PRES_M4_VALUE;
    m_dev_ctx.pres_attr.SV            = ZB_ZCL_ATTR_PRES_SV_VALUE;
    m_dev_ctx.pres_attr.CC            = ZB_ZCL_ATTR_PRES_CC_VALUE;
    m_dev_ctx.pres_attr.TT            = ZB_ZCL_ATTR_PRES_TT_VALUE;
    m_dev_ctx.pres_attr.AC            = ZB_ZCL_ATTR_PRES_AC_VALUE;
    m_dev_ctx.pres_attr.DC            = ZB_ZCL_ATTR_PRES_AC_VALUE;
    //m_dev_ctx.pres_attr.INFO          = ZB_ZCL_ATTR_PRES_INFO_VALUE;
        ZB_ZCL_SET_STRING_VAL(
      m_dev_ctx.pres_attr.INFO,
      serial_def,
      ZB_ZCL_STRING_CONST_SIZE(serial_def)
      );


        /* Error and warning cluster attributes data */
    m_dev_ctx.pres2_attr.error1        = ZB_ZCL_ATTR_PRES2_ERROR1_VALUE;
    m_dev_ctx.pres2_attr.error2        = ZB_ZCL_ATTR_PRES2_ERROR2_VALUE;
    m_dev_ctx.pres2_attr.warning1      = ZB_ZCL_ATTR_PRES2_WARNING1_VALUE;
    m_dev_ctx.pres2_attr.warning2      = ZB_ZCL_ATTR_PRES2_WARNING2_VALUE;
    m_dev_ctx.pres2_attr.time         = ZB_ZCL_ATTR_PRES2_ERROR_TIME_VALUE;

        /* INFO cluster attributes data */
    m_dev_ctx.pres3_attr.CC_coef      = ZB_ZCL_ATTR_CC_COEF_VALUE_ID;
    m_dev_ctx.pres3_attr.SV_coef      = ZB_ZCL_ATTR_SV_COEF_VALUE_ID;
    m_dev_ctx.pres3_attr.S_cap        = ZB_ZCL_ATTR_S_CAP_VALUE_ID;
    m_dev_ctx.pres3_attr.C1           = ZB_ZCL_ATTR_C1_VALUE_ID;
    m_dev_ctx.pres3_attr.C2           = ZB_ZCL_ATTR_C2_VALUE_ID;
    m_dev_ctx.pres3_attr.C3           = ZB_ZCL_ATTR_C3_VALUE_ID;
    m_dev_ctx.pres3_attr.C4           = ZB_ZCL_ATTR_C4_VALUE_ID;
    m_dev_ctx.pres3_attr.C5           = ZB_ZCL_ATTR_C5_VALUE_ID;
    m_dev_ctx.pres3_attr.C6           = ZB_ZCL_ATTR_C6_VALUE_ID;
    m_dev_ctx.pres3_attr.C7           = ZB_ZCL_ATTR_C7_VALUE_ID;
    m_dev_ctx.pres3_attr.C8           = ZB_ZCL_ATTR_C8_VALUE_ID;
    m_dev_ctx.pres3_attr.V1           = ZB_ZCL_ATTR_V1_VALUE_ID;
    m_dev_ctx.pres3_attr.V2           = ZB_ZCL_ATTR_V2_VALUE_ID;
    m_dev_ctx.pres3_attr.V3           = ZB_ZCL_ATTR_V3_VALUE_ID;
    m_dev_ctx.pres3_attr.V4           = ZB_ZCL_ATTR_V4_VALUE_ID;
    m_dev_ctx.pres3_attr.V5           = ZB_ZCL_ATTR_V5_VALUE_ID;
    m_dev_ctx.pres3_attr.V6           = ZB_ZCL_ATTR_V6_VALUE_ID;
    m_dev_ctx.pres3_attr.V7           = ZB_ZCL_ATTR_V7_VALUE_ID;
    m_dev_ctx.pres3_attr.V8           = ZB_ZCL_ATTR_V8_VALUE_ID;
}

ZBOSS_DECLARE_DEVICE_CTX_1_EP(multi_sensor_ctx, multi_sensor_ep);

bool error_status (){
  int buffer = Zigbee_uart.ee1_val + Zigbee_uart.ee2_val + Zigbee_uart.ww1_val + Zigbee_uart.ww2_val;
  if (buffer == 0){
    return false;
    }
    else{
    return true;
  }
}


void zigbee_info (){
    uint32_t STARTING_CAP = ave_cap();
    uint32_t STARTING_VOLT = ave_volt();

    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT,
      ZB_ZCL_CLUSTER_ID_PRES3_MEASUREMENT,
      ZB_ZCL_CLUSTER_SERVER_ROLE,
      ZB_ZCL_ATTR_CC_COEF_VALUE_ID,
      (zb_uint8_t *)&battery_def_int.cc_coef,
      ZB_FALSE);

    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT,
      ZB_ZCL_CLUSTER_ID_PRES3_MEASUREMENT,
      ZB_ZCL_CLUSTER_SERVER_ROLE,
      ZB_ZCL_ATTR_SV_COEF_VALUE_ID,
      (zb_uint8_t *)&battery_def_int.sv_coef,
      ZB_FALSE);

    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT,
      ZB_ZCL_CLUSTER_ID_PRES3_MEASUREMENT,
      ZB_ZCL_CLUSTER_SERVER_ROLE,
      ZB_ZCL_ATTR_S_CAP_VALUE_ID,
      (zb_uint8_t *)&STARTING_CAP,
      ZB_FALSE);

    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT,
      ZB_ZCL_CLUSTER_ID_PRES3_MEASUREMENT,
      ZB_ZCL_CLUSTER_SERVER_ROLE,
      ZB_ZCL_ATTR_S_VOLT_VALUE_ID,
      (zb_uint8_t *)&STARTING_VOLT,
      ZB_FALSE);
    
    for (int i = 0; i<8; i++){//offset the cluster with i and the mod number
      zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT,
        ZB_ZCL_CLUSTER_ID_PRES3_MEASUREMENT,
        ZB_ZCL_CLUSTER_SERVER_ROLE,
        (ZB_ZCL_ATTR_C1_VALUE_ID + i),
        (zb_uint8_t *)&battery_def_int.mod_cap[i],
        ZB_FALSE);

      zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT,
        ZB_ZCL_CLUSTER_ID_PRES3_MEASUREMENT,
        ZB_ZCL_CLUSTER_SERVER_ROLE,
        (ZB_ZCL_ATTR_V1_VALUE_ID + i),
        (zb_uint8_t *)&battery_def_int.mod_volt[i],
        ZB_FALSE);
    }

}

void zigbee_periodic_transmit (uint8_t param){

//zb_buf_get_out_delayed_ext(broadcast_pan_alert,0,0);
   //Periodic Update
   //Time
    //Time.app
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_TIME_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.time_val, ZB_FALSE);
    ////Time.days
    //zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_TIME_DAY_VALUE_ID, (zb_uint8_t *)&rtc_time.days, ZB_FALSE);
    ////Time.sec
    //zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_TIME_SEC_VALUE_ID, (zb_uint8_t *)&rtc_time.seconds, ZB_FALSE);
   //Everything else
    //SOC
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_SOC_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.col_val, ZB_FALSE);
    //M1
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_M1_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.m1_val, ZB_FALSE);
    //M2 
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_M2_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.m2_val, ZB_FALSE);
    //M3
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_M3_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.m3_val, ZB_FALSE);
    //M4
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_M4_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.m4_val, ZB_FALSE);
    //SV
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_SV_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.sv_val, ZB_FALSE);
    //CC
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_CC_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.cc_val, ZB_FALSE);
    //tt
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_TT_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.tt_val, ZB_FALSE);
    //ac
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_AC_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.ac_val, ZB_FALSE);
    //dc
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_DC_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.dc_val, ZB_FALSE);
    //ee status
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_EE_STATUS_VALUE_ID, (zb_uint8_t *)error_status(), ZB_FALSE);
    
    ZB_SCHEDULE_APP_CALLBACK(uart_internal_init, 0);
    
    rethink(1);
    rethink(0);
    refresh_zb();
     }


void zigbee_error_transmit (uint8_t param){
//zb_buf_get_out_delayed_ext(broadcast_pan_alert,0,0);
   //error cluster update
   //error status update
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES2_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ERROR_STATUS_ID, (zb_uint8_t *)&Zigbee_uart.error_state_val, ZB_FALSE);
    //Time.app
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES2_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ERROR_TIME_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.time_val, ZB_FALSE);
    //error update
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES2_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ERROR1_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.ee1_val, ZB_FALSE);
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES2_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_ERROR2_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.ee2_val, ZB_FALSE);
    //warning update
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES2_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_WARNING1_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.ww1_val, ZB_FALSE);
    zb_zcl_set_attr_val(MULTI_SENSOR_ENDPOINT, ZB_ZCL_CLUSTER_ID_PRES2_MEASUREMENT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_WARNING2_VALUE_ID, (zb_uint8_t *)&Zigbee_uart.ww2_val, ZB_FALSE);


    //zb_buf_get_out_delayed_ext(broadcast_pan_alert,0,0);
    ZB_SCHEDULE_APP_CALLBACK(uart_internal_init, 0);
}



#endif /* _ZB_CLUSTER_SENDS_H_ */
