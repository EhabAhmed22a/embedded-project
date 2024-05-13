#include "tm4c123gh6pm.h"
#include "stdint.h"
void uart5_init(void);
void UART2_init(void);
void uart2_send(uint8_t character);
void uart2_send_string(char * string);
char uart2_receive(void);
void uart2_receive_string(char * string, uint32_t len);
void uart5_send(uint8_t character);
void uart5_send_string(char * string);
char uart5_receive(void);
void uart5_receive_string(char * string, uint32_t len);