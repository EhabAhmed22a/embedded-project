#include "../includes/EEPROM.h"
#define EEPROM_CLOCK_ENABLE_BIT 0 // Hypothetical bit for enabling EEPROM clock#define RCGCEEPROM (*((volatile uint32_t *)0x400FE500)) // Base address of RCGCEEPROM register
#define RCGCEEPROM (*((volatile uint32_t *)0x400FE658)) // Absolute address of RCGCEEPROM register
void initEeprom(void)

{

SYSCTL_RCGCEEPROM_R = 1;

//_delay_cycles(3);

while (EEPROM_EEDONE_R & EEPROM_EEDONE_WORKING);

}

void writeEeprom(uint16_t add, uint32_t data)

{

EEPROM_EEBLOCK_R = add >> 4;

EEPROM_EEOFFSET_R = add & 0xF;

EEPROM_EERDWR_R = data;

while (EEPROM_EEDONE_R & EEPROM_EEDONE_WORKING);

}

uint32_t readEeprom(uint16_t add)

{

EEPROM_EEBLOCK_R = add >> 4;

EEPROM_EEOFFSET_R = add & 0xF;

return EEPROM_EERDWR_R;

}