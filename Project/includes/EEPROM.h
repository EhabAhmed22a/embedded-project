#ifndef EEPROM_H_

#define EEPROM_H_
#include "tm4c123gh6pm.h"
#include "stdint.h"
#include <string.h> // For strlen function
//-----------------------------------------------------------------------------

// Subroutines

//-----------------------------------------------------------------------------

void initEeprom(void);

void writeEeprom(uint16_t add, uint32_t data);

uint32_t readEeprom(uint16_t add);

#endif

