#ifndef ZB_DONGLE_GLOBALS__
#define ZB_DONGLE_GLOBALS__


//function declare
uint32_t time_nvmc_update (int value);
uint32_t time_init();
void error_messages(uint8_t value, bool reset);
void warning_messages(uint8_t value, bool reset);

extern uint8_t request_buffer [13];
extern int send_len;

extern void uart_internal_init (uint8_t param);
extern void uart_external_init (uint8_t param);

extern bool uart_enable;

#endif