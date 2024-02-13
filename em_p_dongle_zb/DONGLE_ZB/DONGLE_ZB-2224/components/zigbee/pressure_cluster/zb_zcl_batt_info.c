#include "zboss_api.h"
#include "zb_zcl_bms_measurement.h"
#include "app_error.h"
#include "nrf_log.h"

/**@brief Function which pre-validates the value of attributes before they are written.
 * 
 * @param [in] attr_id  Attribute ID
 * @param [in] endpoint Endpoint
 * @param [in] p_value  Pointer to the value of the attribute which is to be validated
 * 
 * @return ZB_TRUE if the value is valid, ZB_FALSE otherwise.
 */


static zb_ret_t check_value_pres_measurement(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t * p_value)
{
    zb_ret_t ret = ZB_FALSE;
    zb_int16_t val = ZB_ZCL_ATTR_GETS16(p_value);

    switch(attr_id)
    {
        case ZB_ZCL_ATTR_TIME_VALUE_ID:
            /* Check the invalid value */
            if (val != NULL)
            {ret = ZB_TRUE;}
                     break;
        
        case ZB_ZCL_ATTR_M1_VALUE_ID:
            /* Check the invalid value */
            if (val != NULL)
            {ret = ZB_TRUE;}
                     break;

        case ZB_ZCL_ATTR_M2_VALUE_ID:
            /* Check the invalid value */
            if (val != NULL)
            {ret = ZB_TRUE;}
                     break;

        case ZB_ZCL_ATTR_M3_VALUE_ID:
            /* Check the invalid value */
            if (val != NULL)
            {ret = ZB_TRUE;}
                     break;

        case ZB_ZCL_ATTR_M4_VALUE_ID:
            /* Check the invalid value */
            if (val != NULL)
            {ret = ZB_TRUE;}
                     break;

        case ZB_ZCL_ATTR_SV_VALUE_ID:
            /* Check the invalid value */
            if (val != NULL)
            {ret = ZB_TRUE;}
                     break;

        case ZB_ZCL_ATTR_CC_VALUE_ID:
            /* Check the invalid value */
            if (val != NULL)
            {ret = ZB_TRUE;}
                     break;

       case ZB_ZCL_ATTR_TT_VALUE_ID:
            /* Check the invalid value */
            if (val != NULL)
            {ret = ZB_TRUE;}
                     break;

        default:
           break;
    }

    return ret;
}

/**@brief Hook which is being called whenever a new value of attribute is being written.
 * 
 * @param [in] endpoint Endpoint
 * @param [in] attr_id Attribute ID
 * @param [in] new_value Pointer to the new value of the attribute
 */
static void zb_zcl_pres_measurement_write_attr_hook(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t * new_value)
{
    UNUSED_PARAMETER(new_value);

    NRF_LOG_DEBUG("Writing attribute %d of Pressure Measurement Cluster on endpoint %d", attr_id, endpoint);

    if (attr_id == ZB_ZCL_ATTR_TIME_VALUE_ID)
    {
	      /* Implement your own write attributes hook if needed. */
    }
}

/**@brief Function which initialises the server side of Pressure Measurement Cluster. */
void zb_zcl_pres_measurement_init_server()
{
    zb_ret_t ret = zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT,
                                               ZB_ZCL_CLUSTER_SERVER_ROLE,
                                               NULL,//check_value_pres_measurement,
                                               zb_zcl_pres_measurement_write_attr_hook,
                                               (zb_zcl_cluster_handler_t)NULL);
    ASSERT(ret == RET_OK);
}

/**@brief Function which initialises the client side of Pressure Measurement Cluster. */
void zb_zcl_pres_measurement_init_client()
{
    zb_ret_t ret = zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT,
                                               ZB_ZCL_CLUSTER_CLIENT_ROLE,
                                              NULL,//check_value_pres_measurement,
                                               zb_zcl_pres_measurement_write_attr_hook,
                                               (zb_zcl_cluster_handler_t)NULL);
    ASSERT(ret == RET_OK);
}
