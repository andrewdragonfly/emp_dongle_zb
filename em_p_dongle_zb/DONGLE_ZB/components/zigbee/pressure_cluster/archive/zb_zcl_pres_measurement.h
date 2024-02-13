#ifndef ZB_ZCL_PRES_MEASUREMENT_H__
#define ZB_ZCL_PRES_MEASUREMENT_H__

#include "zcl/zb_zcl_common.h"

typedef struct
{
    zb_uint32_t  measure_value;
    zb_uint32_t  M1;
    zb_uint32_t  M2;
    zb_uint32_t M3;
    zb_uint32_t M4;
    zb_uint32_t SV;
    zb_uint32_t CC;
    zb_uint32_t TT;
            
} zb_zcl_pres_measurement_attrs_t;

enum zb_zcl_pres_measurement_attr_e
{
    ZB_ZCL_ATTR_PRES_MEASUREMENT_VALUE_ID = 0x0000,
    ZB_ZCL_ATTR_M1_VALUE_ID = 0x0001,
    ZB_ZCL_ATTR_M2_VALUE_ID = 0x0002,
    ZB_ZCL_ATTR_M3_VALUE_ID = 0x0003,
    ZB_ZCL_ATTR_M4_VALUE_ID = 0x0004,
    ZB_ZCL_ATTR_SV_VALUE_ID = 0x0005,
    ZB_ZCL_ATTR_CC_VALUE_ID = 0x0006,
    ZB_ZCL_ATTR_TT_VALUE_ID = 0x0007,
};

#define ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT                          0x0413U

#define ZB_ZCL_ATTR_PRES_MEASUREMENT_VALUE_UNKNOWN                  ((zb_int32_t)0x8000)

#define ZB_ZCL_ATTR_PRES_M1_VALUE            ((zb_int32_t)0x8001)
#define ZB_ZCL_ATTR_PRES_M2_VALUE            ((zb_int32_t)0x8001)
#define ZB_ZCL_ATTR_PRES_M3_VALUE            ((zb_int32_t)0x8001)
#define ZB_ZCL_ATTR_PRES_M4_VALUE            ((zb_int32_t)0x8001)
#define ZB_ZCL_ATTR_PRES_SV_VALUE            ((zb_int32_t)0x8001)
#define ZB_ZCL_ATTR_PRES_CC_VALUE            ((zb_int32_t)0x8001)
#define ZB_ZCL_ATTR_PRES_TT_VALUE            ((zb_int32_t)0x8001)


#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_VALUE_ID(data_ptr) \
{                                                                              \
    ZB_ZCL_ATTR_PRES_MEASUREMENT_VALUE_ID,                                     \
    ZB_ZCL_ATTR_TYPE_U32,                                                      \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,               \
    (void*) data_ptr                                                           \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_M1_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_M1_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_U32,                                                         \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
      (void*) data_ptr                                                            \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_M2_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_M2_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_U32,                                                         \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
    (void*) data_ptr                                                              \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_M3_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_M3_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_U32,                                                         \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
    (void*) data_ptr                                                              \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_M4_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_M4_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_U32,                                                         \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
    (void*) data_ptr                                                              \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_SV_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_SV_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_U32,                                                         \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
    (void*) data_ptr                                                              \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_CC_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_CC_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_U32,                                                         \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
    (void*) data_ptr                                                              \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_TT_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_TT_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_U32,                                                         \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
    (void*) data_ptr                                                              \
}

#define ZB_ZCL_PRES_MEASUREMENT_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0002u)

#define ZB_ZCL_DECLARE_PRES_MEASUREMENT_ATTRIB_LIST(attr_list,value, M1, M2, M3, M4, SV, CC, TT) \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_PRES_MEASUREMENT)          \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_VALUE_ID, (value))                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_M1_VALUE_ID, (M1))                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_M2_VALUE_ID, (M2))                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_M3_VALUE_ID, (M3))                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_M4_VALUE_ID, (M4))                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_SV_VALUE_ID, (SV))                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_CC_VALUE_ID, (CC))                           \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_TT_VALUE_ID, (TT))                           \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

void zb_zcl_pres_measurement_init_server(void);
void zb_zcl_pres_measurement_init_client(void);

#define ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT_SERVER_ROLE_INIT zb_zcl_pres_measurement_init_server
#define ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT_CLIENT_ROLE_INIT zb_zcl_pres_measurement_init_client

#endif /* ZB_ZCL_PRES_MEASUREMENT_H__ */
