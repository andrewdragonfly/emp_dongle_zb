
#ifndef ZB_ZCL_ERROR_H__
#define ZB_ZCL_ERROR_H__

#include "zcl/zb_zcl_common.h"

typedef struct
{
    zb_bool_t   status;
    zb_int8_t  error1;
    zb_int8_t  error2;
    zb_int8_t  warning1;
    zb_int8_t  warning2;
    zb_int32_t time;
} zb_zcl_pres2_measurement_attrs_t;

enum zb_zcl_pres2_measurement_attr_e
{
    ZB_ZCL_ATTR_ERROR_STATUS_ID     = 0x0000,
    ZB_ZCL_ATTR_ERROR1_VALUE_ID     = 0x0001,
    ZB_ZCL_ATTR_ERROR2_VALUE_ID     = 0x0002,
    ZB_ZCL_ATTR_WARNING1_VALUE_ID   = 0x0003,
    ZB_ZCL_ATTR_WARNING2_VALUE_ID   = 0x0004,
    ZB_ZCL_ATTR_ERROR_TIME_VALUE_ID = 0x0005
};

#define ZB_ZCL_CLUSTER_ID_PRES2_MEASUREMENT     0x0414U

#define ZB_ZCL_ATTR_PRES2_STATUS_VALUE          ((zb_bool_t)false)
#define ZB_ZCL_ATTR_PRES2_ERROR_TIME_VALUE      ((zb_int32_t)0x0)
#define ZB_ZCL_ATTR_PRES2_ERROR1_VALUE          ((zb_int8_t)0x0)
#define ZB_ZCL_ATTR_PRES2_WARNING1_VALUE        ((zb_int8_t)0x0)
#define ZB_ZCL_ATTR_PRES2_ERROR2_VALUE          ((zb_int8_t)0x0)
#define ZB_ZCL_ATTR_PRES2_WARNING2_VALUE        ((zb_int8_t)0x0)

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_STATUS_ID(data_ptr)                  \
{                                                                               \
    ZB_ZCL_ATTR_ERROR_STATUS_ID,                                                \
    ZB_ZCL_ATTR_TYPE_BOOL,                                                      \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                \
    (void*) data_ptr                                                            \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ERROR1_ID(data_ptr)                  \
{                                                                               \
    ZB_ZCL_ATTR_ERROR1_VALUE_ID,                                                \
    ZB_ZCL_ATTR_TYPE_U8,                                                        \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                \
    (void*) data_ptr                                                            \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ERROR2_ID(data_ptr)                  \
{                                                                               \
    ZB_ZCL_ATTR_ERROR2_VALUE_ID,                                                \
    ZB_ZCL_ATTR_TYPE_U8,                                                        \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                \
    (void*) data_ptr                                                            \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WARNING1_ID(data_ptr)                \
{                                                                               \
    ZB_ZCL_ATTR_WARNING1_VALUE_ID,                                              \
    ZB_ZCL_ATTR_TYPE_U8,                                                        \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                \
    (void*) data_ptr                                                            \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WARNING2_ID(data_ptr)                \
{                                                                               \
    ZB_ZCL_ATTR_WARNING2_VALUE_ID,                                              \
    ZB_ZCL_ATTR_TYPE_U8,                                                        \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                \
    (void*) data_ptr                                                            \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ERROR_TIME_ID(data_ptr)              \
{                                                                               \
    ZB_ZCL_ATTR_ERROR_TIME_VALUE_ID,                                            \
    ZB_ZCL_ATTR_TYPE_U32,                                                       \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                \
    (void*) data_ptr                                                            \
}

#define ZB_ZCL_PRES2_MEASUREMENT_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0002u)

#define ZB_ZCL_DECLARE_PRES2_MEASUREMENT_ATTRIB_LIST(attr_list, status, error1, error2, warning1, warning2, time)  \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_PRES2_MEASUREMENT) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_STATUS_ID, (status))                                  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ERROR1_ID, (error1))                                  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ERROR2_ID, (error2))                                  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WARNING1_ID, (warning1))                              \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WARNING2_ID, (warning2))                              \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ERROR_TIME_ID, (time))                                \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

void zb_zcl_pres2_measurement_init_server(void);
void zb_zcl_pres2_measurement_init_client(void);

#define ZB_ZCL_CLUSTER_ID_PRES2_MEASUREMENT_SERVER_ROLE_INIT zb_zcl_pres2_measurement_init_server
#define ZB_ZCL_CLUSTER_ID_PRES2_MEASUREMENT_CLIENT_ROLE_INIT zb_zcl_pres2_measurement_init_client

#endif /* ZB_ZCL_ERROR_H__ */
