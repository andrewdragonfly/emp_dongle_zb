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
#include "zboss_api.h"
#include "zb_zcl_pres_measurement.h"
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

    NRF_LOG_DEBUG("Pre-validating value %hi of Pressure attribute %d", val, attr_id);

    switch(attr_id)
    {
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

    if (attr_id == ZB_ZCL_ATTR_PRES_MEASUREMENT_VALUE_ID)
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
