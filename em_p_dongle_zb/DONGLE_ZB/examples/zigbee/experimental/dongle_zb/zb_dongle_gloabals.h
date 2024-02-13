#ifndef ZB_DONGLE_GLOBALS__
#define ZB_DONGLE_GLOBALS__

#include "zb_dev_defines.h"


//function declare
uint32_t time_nvmc_update (int value);
uint32_t time_init();
void error_messages(uint8_t value, bool reset);
void warning_messages(uint8_t value, bool reset);
void refresh_zb();
void rethink(uint8_t cmd);
void led_state(bool state, int8_t led_num, bool override_state);
void init_header();
void zigbee_info();
void chip_check();

extern uint8_t request_buffer [32];
extern int send_len;

extern void uart_internal_init (uint8_t param);
extern void uart_external_init (uint8_t param);
extern void request_builder (uint8_t param);

extern bool uart_enable;
bool led_states = true;
uint8_t reset_counter = 0;


extern uint32_t MOD1_CAP,MOD2_CAP,MOD3_CAP,MOD4_CAP,MOD5_CAP,MOD6_CAP,MOD7_CAP,MOD8_CAP,
                MOD1_VOLT,MOD2_VOLT,MOD3_VOLT,MOD4_VOLT,MOD5_VOLT,MOD6_VOLT,MOD7_VOLT,MOD8_VOLT,
                CC_COEF, SV_COEF;

#endif

