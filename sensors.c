/*
 * File:   breakbeam.c
 * Author: Alex
 *
 * Created on February 28, 2017, 1:01 AM
 */


#include <xc.h>

int readSnBot(){//tin top
    if(PORTAbits.RA0 == 1){
        return 1;
    }
    return 0;
}

int readAlBot(){//tin bot
    if(PORTAbits.RA1 == 1){
        return 1;
    }
    return 0;
}

int readSnTop(){
    if(PORTAbits.RA2 == 1){
        return 1;
    }
    return 0;
}

int readAlTop(){
    if(PORTAbits.RA3 == 1){
        return 1;
    }
    return 0;
}

int readSnSensor(){
    if(PORTCbits.RC0 == 1){
        return 1;
    }
    return 0;
}

int readAlSensor(){
    return PORTCbits.RC1;
}