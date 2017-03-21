/*
 * File:   solenoid.c
 * Author: Alex
 *
 * Created on February 28, 2017, 1:10 AM
 */


#include <xc.h>

void setSolenoid1(int i) {
    LATBbits.LATB2 = i;
}

void setSolenoid2(int i) {
    LATBbits.LATB3 = i;
}