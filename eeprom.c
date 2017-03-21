#include <stdint.h>
#include <xc.h>


//SAVED DATA WILL INCLUDE
//1x RUNTIME (int)
//4x CAN COUNTS (int)

//! @brief      Reads a single byte of data from the EEPROM.
//! @param      address     The EEPROM address to write the data to (note that not all
//!                         16-bits of this variable may be supported).
//! @returns    The byte of data read from EEPROM.
//! @warning    This function does not return until read operation is complete.
uint8_t eeprom_ReadByte(uint16_t address)
{

    // Set address registers
    EEADRH = (uint8_t)(address >> 8);
    EEADR = (uint8_t)address;

    EECON1bits.EEPGD = 0;       // Select EEPROM Data Memory
    EECON1bits.CFGS = 0;        // Access flash/EEPROM NOT config. registers
    EECON1bits.RD = 1;          // Start a read cycle

    // A read should only take one cycle, and then the hardware will clear
    // the RD bit
    while(EECON1bits.RD == 1);

    return EEDATA;              // Return data
}

//! @brief      Writes a single byte of data to the EEPROM.
//! @param      address     The EEPROM address to write the data to (note that not all
//!                         16-bits of this variable may be supported).
//! @param      data        The data to write to EEPROM.
//! @warning    This function does not return until write operation is complete.
void eeprom_WriteByte(uint16_t address, uint8_t data)
{    
    // Set address registers
    EEADRH = (uint8_t)(address >> 8);
    EEADR = (uint8_t)address;

    EEDATA = data;          // Write data we want to write to SFR
    EECON1bits.EEPGD = 0;   // Select EEPROM data memory
    EECON1bits.CFGS = 0;    // Access flash/EEPROM NOT config. registers
    EECON1bits.WREN = 1;    // Enable writing of EEPROM (this is disabled again after the write completes)

    // The next three lines of code perform the required operations to
    // initiate a EEPROM write
    EECON2 = 0x55;          // Part of required sequence for write to internal EEPROM
    EECON2 = 0xAA;          // Part of required sequence for write to internal EEPROM
    EECON1bits.WR = 1;      // Part of required sequence for write to internal EEPROM

    // Loop until write operation is complete
    while(PIR2bits.EEIF == 0)
    {
        continue;   // Do nothing, are just waiting
    }

    PIR2bits.EEIF = 0;      //Clearing EEIF bit (this MUST be cleared in software after each write)
    EECON1bits.WREN = 0;    // Disable write (for safety, it is re-enabled next time a EEPROM write is performed)
}


/*
 Reads a static address in the EEPROM to obtain the current head (most recent log entry) of the current eeprom. 
 Returns the START address of the newest entry.
 */
uint8_t newestAddress(){
    return eeprom_ReadByte(0x0000);
}

void eeprom_initialize(){
    if(newestAddress() == 0b11111111){
        eeprom_WriteByte(0x0000, 0x00);
    }
}

/*
 Obtains the newest address (which is the start of the previous entry) and increments by 40 to reach clear memory, then stores the data.
 */
void eeprom_LogResult(uint8_t * data){
    uint8_t address = newestAddress() + 5;
    eeprom_WriteByte(0x0000, address);
    for(int i = 0; i < 5; i++){
        eeprom_WriteByte(address + i, 0xFF);
    }
}

void eeprom_GetLog(int back, int * res){
    uint8_t address = newestAddress();
    address = address - back*5;
    for(int i = 0; i < 5; i++){
        res[i] = eeprom_ReadByte(address + i);
    }
}

