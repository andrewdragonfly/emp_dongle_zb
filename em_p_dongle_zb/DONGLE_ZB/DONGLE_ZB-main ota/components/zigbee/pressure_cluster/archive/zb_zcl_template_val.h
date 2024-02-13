
#ifndef ZB_ZCL_TEMPLATE_val_H__
#define ZB_ZCL_TEMPLATE_val_H__

#include "zcl/zb_zcl_common.h"

typedef struct
{
    zb_int32_t  measure_value;
    zb_int32_t  min_measure_value;
    zb_int32_t  max_measure_value;
    zb_uint32_t tolerance;
} zb_zcl_template_measurement_attrs_t;


enum zb_zcl_pretemplate_measurement_attr_e
{

    ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_VALUE_ID = 0x0000,
    /*@brief MinMeasuredValue, ZCL spec 4.5.2.2.1.2 */
    ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MIN_VALUE_ID = 0x0001,
    /*@brief MaxMeasuredValue, ZCL spec 4.5.2.2.1.3 */
    ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MAX_VALUE_ID = 0x0002,
    /*@brief Tolerance, ZCL spec 4.5.2.2.1.4 */
#ifndef ZB_DISABLE_PRESSURE_MEASUREMENT_TOLERANCE_ID
    ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_TOLERANCE_ID = 0x0003,
#else
    ZB_ZCL_ATTR_PRES_MEASUREMENT_TOLERANCE_ID = 0xF003,
#endif
};

/**@brief Pressure measurement cluster ID. */
#define ZB_ZCL_CLUSTER_ID_TEMPLATE_MEASUREMENT                          0x0415U

/**@brief MeasuredValue attribute unknown value. */
#define ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_VALUE_UNKNOWN                  ((zb_int32_t)0x8000)

/**@brief MinMeasuredValue attribute minimum value. */
#define ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MIN_VALUE_MIN_VALUE            ((zb_int32_t)0x8001)

/**@brief MinMeasuredValue attribute maximum value. */
#define ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MIN_VALUE_MAX_VALUE            0x7FFE

/**@brief MinMeasuredValue attribute invalid value. */
#define ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MIN_VALUE_INVALID              ((zb_int32_t)0x8000)

/**@brief MaxMeasuredValue attribute minimum value. */
#define ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MAX_VALUE_MIN_VALUE            ((zb_int32_t)0x8002)

/**@brief MaxMeasuredValue attribute maximum value. */
#define ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MAX_VALUE_MAX_VALUE            0x7FFF

/**@brief MaxMeasuredValue attribute invalid value. */
#define ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MAX_VALUE_INVALID              ((zb_int32_t)0x8000)


 /* (See: Table 4.13 Temperature Measurement Information Attribute Set) */

/**@brief Tolerance attribute minimum value. */
#define ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_TOLERANCE_MIN_VALUE            0x0000

/**@brief Tolerance attribute maximum value. */
#define ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_TOLERANCE_MAX_VALUE            0x0800

/**@brief Default value for Value attribute. */
#define ZB_ZCL_TEMPLATE_MEASUREMENT_VALUE_DEFAULT_VALUE ((zb_int32_t)0x0)

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_VALUE_ID(data_ptr) \
{                                                                              \
    ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_VALUE_ID,                                     \
    ZB_ZCL_ATTR_TYPE_U32,                                                      \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,               \
    (void*) data_ptr                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MIN_VALUE_ID(data_ptr) \
{                                                                                  \
    ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MIN_VALUE_ID,                                     \
    ZB_ZCL_ATTR_TYPE_S16,                                                          \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                  \
    (void*) data_ptr                                                               \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MAX_VALUE_ID(data_ptr) \
{                                                                                  \
    ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MAX_VALUE_ID,                                     \
    ZB_ZCL_ATTR_TYPE_S16,                                                          \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY,                                                  \
    (void*) data_ptr                                                               \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_TOLERANCE_ID(data_ptr) \
{                                                                                  \
    ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_TOLERANCE_ID,                                     \
    ZB_ZCL_ATTR_TYPE_U16,                                                          \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                   \
    (void*) data_ptr                                                               \
}

/** @brief Default value for Pressure Measurement cluster revision global attribute */
#define ZB_ZCL_TEMPLATE_MEASUREMENT_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0002u)

/**@brief Declares attribute list for the Pressure Measurement cluster on the server side.
 *
 * @param attr_list    Attribute list name.
 * @param value        Pointer to the variable to store the MeasuredValue attribute.
 * @param min_value    Pointer to the variable to store the MinMeasuredValue attribute.
 * @param max_value    Pointer to the variable to store the MaxMeasuredValue attribute.
 * @param tolerance    Pointer to the variable to store the Tolerance attribute.
 */
#define ZB_ZCL_DECLARE_TEMPLATE_MEASUREMENT_ATTRIB_LIST(attr_list,                          \
    value, min_value, max_value, tolerance)                                             \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_PRES_MEASUREMENT) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_VALUE_ID, (value))                  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MIN_VALUE_ID, (min_value))          \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_MAX_VALUE_ID, (max_value))          \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_TEMPLATE_MEASUREMENT_TOLERANCE_ID, (tolerance))          \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/**@brief Function initialising the server side of Pressure Measurement Cluster. */
void zb_zcl_TEMPLATE_measurement_init_server(void);
/**@brief Function initialising the client side of Pressure Measurement Cluster. */
void zb_zcl_TEMPLATE_measurement_init_client(void);

/**@brief Defines needed for the stack to initialise the cluster correctly. */
#define ZB_ZCL_CLUSTER_ID_TEMPLATE_MEASUREMENT_SERVER_ROLE_INIT zb_zcl_pres_measurement_init_server
#define ZB_ZCL_CLUSTER_ID_TEMPLATE_MEASUREMENT_CLIENT_ROLE_INIT zb_zcl_pres_measurement_init_client

#endif /* ZB_ZCL_PRES_MEASUREMENT_H__ */
