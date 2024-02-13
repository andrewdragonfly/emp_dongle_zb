
#ifndef ZB_DEV_DEFINES_H__
#define ZB_DEV_DEFINES_H__

#include "zb_store.h"


#define  SERIAL  "221BBA100002"
#define  DATE   "20230608"
#define FW_VERSION  "2.3.4"
#define HW_VERSION "5.6"

//uint32_t MOD1_CAP = 112272;
//uint32_t MOD2_CAP = 112242;
//uint32_t MOD3_CAP = 112224;
//uint32_t MOD4_CAP = 112144;
//uint32_t MOD5_CAP = 0;
//uint32_t MOD6_CAP = 0;
//uint32_t MOD7_CAP = 0;
//uint32_t MOD8_CAP = 0;

//uint32_t MOD1_VOLT = 365010;
//uint32_t MOD2_VOLT = 364980;
//uint32_t MOD3_VOLT = 364980;
//uint32_t MOD4_VOLT = 364980;
//uint32_t MOD5_VOLT = 0;
//uint32_t MOD6_VOLT = 0;
//uint32_t MOD7_VOLT = 0;
//uint32_t MOD8_VOLT = 0;

//uint32_t CC_COEF = 719000;
//uint32_t SV_COEF = 867400;


//#define  SERIAL     "221BBA100002"//batt_data.serial
//#define  DATE       batt_data.date
//#define  FW_VERSION batt_data.fw_version
//#define  HW_VERSION batt_data.hw_version

uint32_t MOD1_CAP, MOD2_CAP, MOD3_CAP, MOD4_CAP, MOD5_CAP, MOD6_CAP, MOD7_CAP, MOD8_CAP,
         MOD1_VOLT, MOD2_VOLT, MOD3_VOLT, MOD4_VOLT, MOD5_VOLT, MOD6_VOLT, MOD7_VOLT, MOD8_VOLT,
         CC_COEF, SV_COEF;

void init_header (){
  MOD1_VOLT = *(p_dev_id_addr + 8);
  MOD2_VOLT = *(p_dev_id_addr + 9);
  MOD3_VOLT = *(p_dev_id_addr + 10);
  MOD4_VOLT = *(p_dev_id_addr + 11);

  MOD5_VOLT = *(p_dev_id_addr + 12);
  MOD6_VOLT = *(p_dev_id_addr + 13);
  MOD7_VOLT = *(p_dev_id_addr + 14);
  MOD8_VOLT = *(p_dev_id_addr + 15);

  MOD1_CAP = *(p_dev_id_addr + 16);
  MOD2_CAP = *(p_dev_id_addr + 17);
  MOD3_CAP = *(p_dev_id_addr + 18);
  MOD4_CAP = *(p_dev_id_addr + 19);

  MOD5_CAP = *(p_dev_id_addr + 20);
  MOD6_CAP = *(p_dev_id_addr + 21);
  MOD7_CAP = *(p_dev_id_addr + 22);
  MOD8_CAP = *(p_dev_id_addr + 23);

  //from programing station
  CC_COEF = *(p_dev_id_addr + 7);
  SV_COEF = *(p_dev_id_addr + 8);

  MOD1_CAP = 112272;
  MOD2_CAP = 112242;
  MOD3_CAP = 112224;
  MOD4_CAP = 112144;
  MOD5_CAP = 0;
  MOD6_CAP = 0;
  MOD7_CAP = 0;
  MOD8_CAP = 0;

  MOD1_VOLT = 365010;
  MOD2_VOLT = 364980;
  MOD3_VOLT = 364980;
  MOD4_VOLT = 364980;
  MOD5_VOLT = 0;
  MOD6_VOLT = 0;
  MOD7_VOLT = 0;
  MOD8_VOLT = 0;

  CC_COEF = 719000;
  SV_COEF = 867400;

  zigbee_info ();
}





#endif  /* _ZB_DEV_DEFINES_H__ */
