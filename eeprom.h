#include <stdint.h>
#include <xc.h>

uint8_t eeprom_ReadByte(uint16_t address);
void eeprom_WriteByte(uint16_t address, uint8_t data);
uint8_t newestAddress();
void eeprom_initialize();
void eeprom_LogResult(uint8_t * data);
void eeprom_GetLog(int back, uint8_t * res);