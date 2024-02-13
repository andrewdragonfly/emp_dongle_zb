#ifndef ZB_ZCL_BATT_INFO_H__
#define ZB_ZCL_BATT_INFO_H__

#include "zcl/zb_zcl_common.h"

typedef struct
{
    zb_uint32_t  CC_coef;
    zb_uint32_t  SV_coef;

    zb_uint32_t  S_cap;
    zb_uint32_t  S_volt;

    zb_uint32_t  C1;
    zb_uint32_t  C2;
    zb_uint32_t  C3;
    zb_uint32_t  C4;
    zb_uint32_t  C5;
    zb_uint32_t  C6;
    zb_uint32_t  C7;
    zb_uint32_t  C8;

    zb_uint32_t  V1;
    zb_uint32_t  V2;
    zb_uint32_t  V3;
    zb_uint32_t  V4;
    zb_uint32_t  V5;
    zb_uint32_t  V6;
    zb_uint32_t  V7;
    zb_uint32_t  V8;



} zb_zcl_pres3_measurement_attrs_t;

enum zb_zcl_pres3_measurement_attr_e
{
    ZB_ZCL_ATTR_CC_COEF_VALUE_ID  = 0x0000,
    ZB_ZCL_ATTR_SV_COEF_VALUE_ID  = 0x0001,
    ZB_ZCL_ATTR_S_CAP_VALUE_ID    = 0x0002,
    ZB_ZCL_ATTR_S_VOLT_VALUE_ID   = 0x0003,
    ZB_ZCL_ATTR_C1_VALUE_ID       = 0x0004,
    ZB_ZCL_ATTR_C2_VALUE_ID       = 0x0005,
    ZB_ZCL_ATTR_C3_VALUE_ID       = 0x0006,
    ZB_ZCL_ATTR_C4_VALUE_ID       = 0x0007,
    ZB_ZCL_ATTR_C5_VALUE_ID       = 0x0008,
    ZB_ZCL_ATTR_C6_VALUE_ID       = 0x0009,
    ZB_ZCL_ATTR_C7_VALUE_ID       = 0x000a,
    ZB_ZCL_ATTR_C8_VALUE_ID       = 0x000b,
    ZB_ZCL_ATTR_V1_VALUE_ID       = 0x000c,
    ZB_ZCL_ATTR_V2_VALUE_ID       = 0x000d,
    ZB_ZCL_ATTR_V3_VALUE_ID       = 0x000e,
    ZB_ZCL_ATTR_V4_VALUE_ID       = 0x000f,
    ZB_ZCL_ATTR_V5_VALUE_ID       = 0x0010,
    ZB_ZCL_ATTR_V6_VALUE_ID       = 0x0011,
    ZB_ZCL_ATTR_V7_VALUE_ID       = 0x0012,
    ZB_ZCL_ATTR_V8_VALUE_ID       = 0x0013
    };

#define ZB_ZCL_CLUSTER_ID_PRES3_MEASUREMENT                          0x0415U

#define ZB_ZCL_ATTR_INFO_CC_COEF_VALUE             ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_SV_COEF_VALUE             ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_S_CAP_VALUE               ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_S_VOLT_VALUE              ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_C1_VALUE                  ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_C2_VALUE                  ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_C3_VALUE                  ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_C4_VALUE                  ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_C5_VALUE                  ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_C6_VALUE                  ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_C7_VALUE                  ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_C8_VALUE                  ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_V1_VALUE                  ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_V2_VALUE                  ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_V3_VALUE                  ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_V4_VALUE                  ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_V5_VALUE                  ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_V6_VALUE                  ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_V7_VALUE                  ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_INFO_V8_VALUE                  ((zb_uint32_t)0x0)


#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_CC_COEF_VALUE_ID(data_ptr)      \
{                                                                          \
    ZB_ZCL_ATTR_CC_COEF_VALUE_ID,                                          \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_SV_COEF_VALUE_ID(data_ptr)      \
{                                                                          \
    ZB_ZCL_ATTR_SV_COEF_VALUE_ID,                                          \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_S_CAP_VALUE_ID(data_ptr)        \
{                                                                          \
    ZB_ZCL_ATTR_S_CAP_VALUE_ID,                                            \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_S_VOLT_VALUE_ID(data_ptr)       \
{                                                                          \
    ZB_ZCL_ATTR_S_VOLT_VALUE_ID,                                           \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_C1_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_C1_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_C2_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_C2_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_C3_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_C3_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_C4_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_C4_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_C5_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_C5_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_C6_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_C6_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_C7_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_C7_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_C8_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_C8_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_V1_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_V1_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_V2_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_V2_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_V3_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_V3_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_V4_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_V4_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_V5_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_V5_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_V6_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_V6_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_V7_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_V7_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_V8_VALUE_ID(data_ptr)           \
{                                                                          \
    ZB_ZCL_ATTR_V8_VALUE_ID,                                               \
    ZB_ZCL_ATTR_TYPE_U32,                                                  \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,           \
    (void*) data_ptr                                                       \
}


#define ZB_ZCL_PRES3_MEASUREMENT_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0002u)

#define ZB_ZCL_DECLARE_PRES3_MEASUREMENT_ATTRIB_LIST(attr_list, CC_coef, SV_coef, S_cap, S_volt, C1, C2, C3, C4, C5, C6, C7, C8, V1, V2, V3, V4, V5, V6, V7, V8) \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_PRES3_MEASUREMENT)      \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_CC_COEF_VALUE_ID, (CC_coef))                               \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_SV_COEF_VALUE_ID, (SV_coef))                               \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_S_CAP_VALUE_ID, (S_cap))                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_S_VOLT_VALUE_ID, (S_volt))                                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_C1_VALUE_ID, (C1))                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_C2_VALUE_ID, (C2))                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_C3_VALUE_ID, (C3))                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_C4_VALUE_ID, (C4))                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_C5_VALUE_ID, (C5))                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_C6_VALUE_ID, (C6))                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_C7_VALUE_ID, (C7))                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_C8_VALUE_ID, (C8))                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_V1_VALUE_ID, (V1))                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_V2_VALUE_ID, (V2))                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_V3_VALUE_ID, (V3))                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_V4_VALUE_ID, (V4))                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_V5_VALUE_ID, (V5))                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_V6_VALUE_ID, (V6))                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_V7_VALUE_ID, (V7))                                         \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_V8_VALUE_ID, (V8))                                         \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

void zb_zcl_pres3_measurement_init_server(void);
void zb_zcl_pres3_measurement_init_client(void);

#define ZB_ZCL_CLUSTER_ID_PRES3_MEASUREMENT_SERVER_ROLE_INIT zb_zcl_pres_measurement_init_server
#define ZB_ZCL_CLUSTER_ID_PRES3_MEASUREMENT_CLIENT_ROLE_INIT zb_zcl_pres_measurement_init_client

#endif /* ZB_ZCL_BATT_INFO_H__ */
