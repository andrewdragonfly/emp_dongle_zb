
#ifndef ZB_ZCL_M1_val_H__
#define ZB_ZCL_M1_val_H__

#include "zcl/zb_zcl_common.h"

typedef struct
{
    zb_bool_t   status;
    zb_int32_t  error;
    zb_int32_t  warning;
} zb_zcl_pres2_measurement_attrs_t;


enum zb_zcl_pres2_measurement_attr_e
{
    ZB_ZCL_ATTR_ERROR_STATUS_ID = 0x0000,
    ZB_ZCL_ATTR_ERROR_VALUE_ID = 0x0001,
    ZB_ZCL_ATTR_WARNING_VALUE_ID = 0x0002

};

#define ZB_ZCL_CLUSTER_ID_PRES2_MEASUREMENT                          0x0414U

#define ZB_ZCL_ATTR_PRES2_STATUS_VALUE                  ((zb_bool_t)false)
#define ZB_ZCL_ATTR_PRES2_ERROR_VALUE                  ((zb_int32_t)0x8000)
#define ZB_ZCL_ATTR_PRES2_WARNING_VALUE                ((zb_int32_t)0x8001)

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_STATUS_ID(data_ptr)                  \
{                                                                               \
    ZB_ZCL_ATTR_ERROR_STATUS_ID,                                                \
    ZB_ZCL_ATTR_TYPE_BOOL,                                                      \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                \
    (void*) data_ptr                                                            \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ERROR_ID(data_ptr)                   \
{                                                                               \
    ZB_ZCL_ATTR_ERROR_VALUE_ID,                                                 \
    ZB_ZCL_ATTR_TYPE_U32,                                                       \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                \
    (void*) data_ptr                                                            \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_WARNING_ID(data_ptr)                 \
{                                                                               \
    ZB_ZCL_ATTR_WARNING_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                       \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                \
    (void*) data_ptr                                                            \
}

#define ZB_ZCL_PRES2_MEASUREMENT_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0002u)

#define ZB_ZCL_DECLARE_PRES2_MEASUREMENT_ATTRIB_LIST(attr_list, status, error, warning)  \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_PRES2_MEASUREMENT) \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_STATUS_ID, (status))                                  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ERROR_ID, (error))                                    \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_WARNING_ID, (warning))                                \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

void zb_zcl_pres2_measurement_init_server(void);
void zb_zcl_pres2_measurement_init_client(void);

#define ZB_ZCL_CLUSTER_ID_PRES2_MEASUREMENT_SERVER_ROLE_INIT zb_zcl_pres_measurement_init_server
#define ZB_ZCL_CLUSTER_ID_PRES2_MEASUREMENT_CLIENT_ROLE_INIT zb_zcl_pres_measurement_init_client

#endif /* ZB_ZCL_PRES_MEASUREMENT_H__ */
