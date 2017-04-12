#include <xc.h>
#include "I2C.h"

const char statictime[7] = {0x00, //45 Seconds 
    0x01, //Minutes
    0x01, //24 hour mode
    0x03, //Saturday 
    0x12, //31st
    0x04, //Month
    0x17}; //2017


void set_time(void) {
    I2C_Master_Start(); //Start condition
    I2C_Master_Write(0b11010000); //7 bit RTC address + Write
    I2C_Master_Write(0x00); //Set memory pointer to seconds
    for (char i = 0; i < 7; i++) {
        I2C_Master_Write(statictime[i]);
    }
    I2C_Master_Stop(); //Stop condition
}

void get_time(int* time) {
    //Reset RTC memory pointer 
    I2C_Master_Start(); //Start condition
    I2C_Master_Write(0b11010000); //7 bit RTC address + Write
    I2C_Master_Write(0x00); //Set memory pointer to seconds
    I2C_Master_Stop(); //Stop condition

    //Read Current Time
    I2C_Master_Start();
    I2C_Master_Write(0b11010001); //7 bit RTC address + Read
    for (unsigned char i = 0; i < 0x06; i++) {
        time[i] = I2C_Master_Read(1);
    }
    time[6] = I2C_Master_Read(0); //Final Read without ack
    I2C_Master_Stop();
}
