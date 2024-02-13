
#include "zboss_api.h"
#include "zb_zcl_TEMPLATE_val.h"
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


static zb_ret_t check_value_template_measurement(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t * p_value)
{
    zb_ret_t ret = ZB_FALSE;
    zb_int16_t val = ZB_ZCL_ATTR_GETS16(p_value);
    switch(attr_id)
    {
        case ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_VALUE_ID:
            /* Check if the value is unknown. */
            if (val != ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_VALUE_UNKNOWN)
            {      
                /* Check if the value is higher than the minimal allowed. */
                zb_zcl_attr_t * p_attr_min_desc = zb_zcl_get_attr_desc_a(endpoint,
                                                                         ZB_ZCL_CLUSTER_ID_TEMPLATE_MEASUREMENT,
                                                                         ZB_ZCL_CLUSTER_SERVER_ROLE,
                                                                         ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MIN_VALUE_ID);
                ASSERT(p_attr_min_desc);

                zb_int16_t minimal_value = ZB_ZCL_GET_ATTRIBUTE_VAL_S16(p_attr_min_desc);

                if ((minimal_value != ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MIN_VALUE_INVALID) && 
                    (minimal_value > val))
                {
                    break;
                }

                /* Check if the value is lower than the maximal allowed. */
                zb_zcl_attr_t * p_attr_max_desc = zb_zcl_get_attr_desc_a(endpoint,
                                                                         ZB_ZCL_CLUSTER_ID_TEMPLATE_MEASUREMENT,
                                                                         ZB_ZCL_CLUSTER_SERVER_ROLE,
                                                                         ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MAX_VALUE_ID);
                ASSERT(p_attr_max_desc);

                zb_int16_t maximal_value = ZB_ZCL_GET_ATTRIBUTE_VAL_S16(p_attr_max_desc);

                if ((maximal_value != ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MAX_VALUE_INVALID) &&
                    (val > maximal_value))
                {
                    break;
                }
            }

            ret = ZB_TRUE;
            break;

        case ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MIN_VALUE_ID:
            /* Check the invalid value */
            if (val != ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MIN_VALUE_INVALID)
            {
                /* Check the value is in bounds */
                if ((val < ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MIN_VALUE_MIN_VALUE) ||
                    (val > ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MIN_VALUE_MAX_VALUE))
                {
                    break;
                }
            }

            ret = ZB_TRUE;
            break;

        case ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MAX_VALUE_ID:
            /* Check the invalid value */
            if (val != ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MAX_VALUE_INVALID)
            {
                /* Check the value is in bounds */
                if ((val < ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MAX_VALUE_MIN_VALUE) ||
#if ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MAX_VALUE_MAX_VALUE != 0x7FFF       
                    /* Avoid compiler warning about always false condition */
                    (val > ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MAX_VALUE_MAX_VALUE))
#else
                    (0))
#endif
                {
                    break;
                }
            }

            ret = ZB_TRUE;
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
static void zb_zcl_template_measurement_write_attr_hook(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t * new_value)
{
    UNUSED_PARAMETER(new_value);

    NRF_LOG_DEBUG("Writing attribute %d of Pressure Measurement Cluster on endpoint %d", attr_id, endpoint);

    if (attr_id == ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_VALUE_ID)
    {
	      /* Implement your own write attributes hook if needed. */
    }
}

/**@brief Function which initialises the server side of Pressure Measurement Cluster. */
void zb_zcl_template_measurement_init_server()
{
    zb_ret_t ret = zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_TEMPLATE_MEASUREMENT,
                                               ZB_ZCL_CLUSTER_SERVER_ROLE,
                                               NULL,//check_value_pres_measurement,
                                               zb_zcl_template_measurement_write_attr_hook,
                                               (zb_zcl_cluster_handler_t)NULL);
    ASSERT(ret == RET_OK);
}

/**@brief Function which initialises the client side of Pressure Measurement Cluster. */
void zb_zcl_template_measurement_init_client()
{
    zb_ret_t ret = zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_TEMPLATE_MEASUREMENT,
                                               ZB_ZCL_CLUSTER_CLIENT_ROLE,
                                              NULL,//check_value_pres_measurement,
                                               zb_zcl_template_measurement_write_attr_hook,
                                               (zb_zcl_cluster_handler_t)NULL);
    ASSERT(ret == RET_OK);
}
