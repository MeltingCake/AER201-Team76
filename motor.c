/*
 * File:   motor.c
 * Author: Alex
 *
 * Created on February 28, 2017, 1:22 AM
 */


#include <xc.h>

void setMotor(int i ){
    LATBbits.LATB0 = i;
}
