#ifndef ZB_ZCL_BMS_MEASUREMENT_H__
#define ZB_ZCL_BMS_MEASUREMENT_H__

#include "zcl/zb_zcl_common.h"

typedef struct
{
    zb_uint32_t  Time;
    zb_uint16_t  Time_day;
    zb_uint32_t  Time_sec;
    zb_uint16_t  M1;
    zb_uint16_t  M2;
    zb_uint16_t  M3;
    zb_uint16_t  M4;
    zb_uint16_t  SV;
    zb_uint16_t  CC;
    zb_int32_t   TT;
    zb_bool_t    AC;
    zb_bool_t    DC;
    zb_uint16_t  SOC;
    zb_uint32_t  SOH;
    zb_bool_t    ee_status;
    zb_char_t  INFO[12];


} zb_zcl_pres_measurement_attrs_t;

enum zb_zcl_pres_measurement_attr_e
{
    ZB_ZCL_ATTR_TIME_VALUE_ID      = 0xa,
    ZB_ZCL_ATTR_SOC_VALUE_ID       = 0x9,
    ZB_ZCL_ATTR_TIME_DAY_VALUE_ID  = 0xc,//not used in clusty
    ZB_ZCL_ATTR_TIME_SEC_VALUE_ID  = 0xd,//not used in clusty
    ZB_ZCL_ATTR_M1_VALUE_ID        = 0x0,
    ZB_ZCL_ATTR_M2_VALUE_ID        = 0x1,
    ZB_ZCL_ATTR_M3_VALUE_ID        = 0x2,
    ZB_ZCL_ATTR_M4_VALUE_ID        = 0x3,
    ZB_ZCL_ATTR_SV_VALUE_ID        = 0x4,
    ZB_ZCL_ATTR_CC_VALUE_ID        = 0x5,
    ZB_ZCL_ATTR_TT_VALUE_ID        = 0x6,
    ZB_ZCL_ATTR_AC_VALUE_ID        = 0x7,
    ZB_ZCL_ATTR_DC_VALUE_ID        = 0x8,
    ZB_ZCL_ATTR_INFO_VALUE_ID      = 0xe,//not used in clusty
    ZB_ZCL_ATTR_EE_STATUS_VALUE_ID = 0xb,


};

#define ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT                          0x0413U

#define ZB_ZCL_ATTR_PRES_TIME_VALUE                ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_PRES_TIME_RTC_DAY_VALUE        ((zb_uint16_t)0x0)
#define ZB_ZCL_ATTR_PRES_TIME_RTC_SECOND_VALUE     ((zb_uint32_t)0x0)
#define ZB_ZCL_ATTR_PRES_M1_VALUE                  ((zb_uint16_t)0x0)
#define ZB_ZCL_ATTR_PRES_M2_VALUE                  ((zb_uint16_t)0x0)
#define ZB_ZCL_ATTR_PRES_M3_VALUE                  ((zb_uint16_t)0x0)
#define ZB_ZCL_ATTR_PRES_M4_VALUE                  ((zb_uint16_t)0x0)
#define ZB_ZCL_ATTR_PRES_SV_VALUE                  ((zb_uint16_t)0x0)
#define ZB_ZCL_ATTR_PRES_CC_VALUE                  ((zb_uint16_t)0x0)
#define ZB_ZCL_ATTR_PRES_SOC_VALUE                 ((zb_uint16_t)0x0)
#define ZB_ZCL_ATTR_PRES_TT_VALUE                  ((zb_int16_t)0x0)
#define ZB_ZCL_ATTR_PRES_AC_VALUE                  ((zb_bool_t)false)
#define ZB_ZCL_ATTR_PRES_DC_VALUE                  ((zb_bool_t)false)
#define ZB_ZCL_ATTR_PRES_INFO_VALUE                ((zb_char_t)0x0)
#define ZB_ZCL_ATTR_PRES_EE_STATUS_VALUE           ((zb_bool_t)false)


#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_TIME_VALUE_ID(data_ptr) \
{                                                                                   \
    ZB_ZCL_ATTR_TIME_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_U32,                                                           \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                    \
    (void*) data_ptr                                                                \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_SOC_VALUE_ID(data_ptr)  \
{                                                                                   \
    ZB_ZCL_ATTR_SOC_VALUE_ID,                                                       \
    ZB_ZCL_ATTR_TYPE_U16,                                                           \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                    \
    (void*) data_ptr                                                                \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_TIME_RTC_DAY_VALUE_ID(data_ptr) \
{                                                                                           \
    ZB_ZCL_ATTR_TIME_DAY_VALUE_ID,                                                          \
    ZB_ZCL_ATTR_TYPE_U16,                                                                   \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                            \
    (void*) data_ptr                                                                        \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_TIME_RTC_SECOND_VALUE_ID(data_ptr) \
{                                                                                              \
    ZB_ZCL_ATTR_TIME_SEC_VALUE_ID,                                                             \
    ZB_ZCL_ATTR_TYPE_U32,                                                                      \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                               \
    (void*) data_ptr                                                                           \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_M1_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_M1_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_U16,                                                         \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
    (void*) data_ptr                                                              \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_M2_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_M2_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_U16,                                                         \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
    (void*) data_ptr                                                              \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_M3_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_M3_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_U16,                                                         \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
    (void*) data_ptr                                                              \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_M4_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_M4_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_U16,                                                         \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
    (void*) data_ptr                                                              \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_SV_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_SV_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_U16,                                                         \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
    (void*) data_ptr                                                              \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_CC_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_CC_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_U16,                                                         \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
    (void*) data_ptr                                                              \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_TT_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_TT_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_S16,                                                         \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
    (void*) data_ptr                                                              \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_AC_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_AC_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_BOOL,                                                        \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
    (void*) data_ptr                                                              \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_DC_VALUE_ID(data_ptr) \
{                                                                                 \
    ZB_ZCL_ATTR_DC_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_BOOL,                                                        \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                  \
    (void*) data_ptr                                                              \
}
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_INFO_VALUE_ID(data_ptr) \
{                                                                                   \
    ZB_ZCL_ATTR_INFO_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_U8,                                                            \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                    \
    (void*) data_ptr                                                                \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_PRES_MEASUREMENT_EE_STATUS_VALUE_ID(data_ptr) \
{                                                                                        \
    ZB_ZCL_ATTR_EE_STATUS_VALUE_ID,                                                      \
    ZB_ZCL_ATTR_TYPE_BOOL,                                                               \
    ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,                         \
    (void*) data_ptr                                                                     \
}

#define ZB_ZCL_PRES_MEASUREMENT_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0002u)

#define ZB_ZCL_DECLARE_PRES_MEASUREMENT_ATTRIB_LIST(attr_list,Time, Time_day, Time_sec, M1, M2, M3, M4, SV, CC, TT, AC, DC, INFO, SOC, ee_status) \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_PRES_MEASUREMENT)                  \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_TIME_VALUE_ID, (Time))                               \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_TIME_RTC_DAY_VALUE_ID, (Time_day))                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_TIME_RTC_SECOND_VALUE_ID, (Time_sec))                \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_SOC_VALUE_ID, (SOC))                                 \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_M1_VALUE_ID, (M1))                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_M2_VALUE_ID, (M2))                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_M3_VALUE_ID, (M3))                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_M4_VALUE_ID, (M4))                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_SV_VALUE_ID, (SV))                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_CC_VALUE_ID, (CC))                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_TT_VALUE_ID, (TT))                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_AC_VALUE_ID, (AC))                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_DC_VALUE_ID, (DC))                                   \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_INFO_VALUE_ID, (INFO))                               \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_PRES_MEASUREMENT_EE_STATUS_VALUE_ID, (ee_status))                     \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

void zb_zcl_pres_measurement_init_server(void);
void zb_zcl_pres_measurement_init_client(void);

#define ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT_SERVER_ROLE_INIT zb_zcl_pres_measurement_init_server
#define ZB_ZCL_CLUSTER_ID_PRES_MEASUREMENT_CLIENT_ROLE_INIT zb_zcl_pres_measurement_init_client

#endif /* ZB_ZCL_BMS_MEASUREMENT_H__ */
