/*
 * File:   servos.c
 * Author: Alex
 *
 * Created on February 28, 2017, 1:08 AM
 */


#include <xc.h>
#include "configBits.h"

void delay_us(unsigned int useconds){
    while(useconds > 0){
        __delay_us(1);
        useconds--;
    }
}

void servoRotate1(int angle) {
    unsigned int i;
    unsigned long first = (unsigned long) (800 + ((unsigned long) (((float) angle)*(70.0 / 9.0))));
    unsigned long second = (unsigned long) (19200 - ((unsigned long) (((float) angle)*(70.0 / 9.0))));
    for (i = 0; i < 30; i++) {
        LATAbits.LATA4= 1;
        delay_us(first);
        LATAbits.LATA4 = 0;
        delay_us(second);
    }

}

void servoRotate2(int angle) {
    unsigned int i;
    unsigned long first = (unsigned long) (800 + ((unsigned long) (((float) angle)*(70.0 / 9.0))));
    unsigned long second = (unsigned long) (19200 - ((unsigned long) (((float) angle)*(70.0 / 9.0))));
    for (i = 0; i < 50; i++) {
        LATAbits.LATA5 = 1;
        delay_us(first);
        LATAbits.LATA5 = 0;
        delay_us(second);
    }

}

void servoRotate3(int angle) {
    unsigned int i;
    unsigned long first = (unsigned long) (800 + ((unsigned long) (((float) angle)*(70.0 / 9.0))));
    unsigned long second = (unsigned long) (19200 - ((unsigned long) (((float) angle)*(70.0 / 9.0))));
    for (i = 0; i < 25; i++) {
        LATEbits.LATE0 = 1;
        delay_us(first);
        LATEbits.LATE0 = 0;
        delay_us(second);
    }

}

void servoRotate4(int angle) {
    unsigned int i;
    unsigned long first = (unsigned long) (800 + ((unsigned long) (((float) angle)*(70.0 / 9.0))));
    unsigned long second = (unsigned long) (19200 - ((unsigned long) (((float) angle)*(70.0 / 9.0))));
    for (i = 0; i < 50; i++) {
        LATEbits.LATE1 = 1;
        delay_us(first);
        LATEbits.LATE1 = 0;
        delay_us(second);
    }
}

void servoRotate5(int angle) {
    unsigned int i;
    unsigned long first = (unsigned long) (800 + ((unsigned long) (((float) angle)*(70.0 / 9.0))));
    unsigned long second = (unsigned long) (19200 - ((unsigned long) (((float) angle)*(70.0 / 9.0))));
    for (i = 0; i < 50; i++) {
        LATBbits.LATB2 = 1;
        delay_us(800);
        LATBbits.LATB2 = 0;
        delay_us(19200);
    }
}


void servoRotate0(int servo) //0 Degree
{
    unsigned int i;
    for (i = 0; i < 30; i++) {
        switch (servo) {
            case 0:
                LATAbits.LATA4 = 1;
                break;
            case 1:
                LATAbits.LATA5 = 1;
                break;
            case 2:
                LATEbits.LATE0 = 1;
                break;
            case 3:
                LATEbits.LATE1 = 1;
                break;
            case 4:
                LATBbits.LATB2 = 1;
                break;
        }
        __delay_us(1500);
        switch (servo) {
            case 0:
                LATAbits.LATA4 = 0;
                break;
            case 1:
                LATAbits.LATA5 = 0;
                break;
            case 2:
                LATEbits.LATE0 = 0;
                break;
            case 3:
                LATEbits.LATE1 = 0;
                break;
            case 4:
                LATBbits.LATB2 = 0;
                break;
        }
        __delay_us(18500);
    }
}

void servoRotate90(int servo) //0 Degree
{
    unsigned int i;
    for (i = 0; i < 30; i++) {
        switch (servo) {
            case 0:
                LATAbits.LATA4 = 1;
                break;
            case 1:
                LATAbits.LATA5 = 1;
                break;
            case 2:
                LATEbits.LATE0 = 1;
                break;
            case 3:
                LATEbits.LATE1 = 1;
                break;
            case 4:
                LATBbits.LATB2 = 1;
                break;
        }
        __delay_us(2400);
        switch (servo) {
            case 0:
                LATAbits.LATA4 = 0;
                break;
            case 1:
                LATAbits.LATA5 = 0;
                break;
            case 2:
                LATEbits.LATE0 = 0;
                break;
            case 3:
                LATEbits.LATE1 = 0;
                break;
            case 4:
                LATBbits.LATB2 = 0;
                break;
        }
        __delay_us(17600);
    }
}

void servoRotate90n(int servo) //0 Degree
{
    unsigned int i;
    for (i = 0; i < 30; i++) {
        switch (servo) {
            case 0:
                LATAbits.LATA4 = 1;
                break;
            case 1:
                LATAbits.LATA5 = 1;
                break;
            case 2:
                LATEbits.LATE0 = 1;
                break;
            case 3:
                LATEbits.LATE1 = 1;
                break;
            case 4:
                LATBbits.LATB2 = 1;
                break;
        }
        __delay_us(600);
        switch (servo) {
            case 0:
                LATAbits.LATA4 = 0;
                break;
            case 1:
                LATAbits.LATA5 = 0;
                break;
            case 2:
                LATEbits.LATE0 = 0;
                break;
            case 3:
                LATEbits.LATE1 = 0;
                break;
            case 4:
                LATBbits.LATB2 = 0;
                break;
        }
        __delay_us(19400);
    }
}

int servoRotateArm(){
    unsigned int i;
    int res = -1;
    for(int i = 0; i < 60; i++){
        LATBbits.LATB2 = 1;
        __delay_us(1800);
        LATBbits.LATB2 = 0;
        __delay_us(18200);
        if(PORTCbits.RC1 == 1){
            res = 1;
        }else{
            res = 0;
        }
    }
    return res;
}

void dispenseAlCan(){
    servoRotate90(1);
    __delay_ms(800);
    servoRotate0(1);
}

void dispenseSnCan(){
    servoRotate90(0);
    __delay_ms(800);
    servoRotate0(0);
}

void moveArm(){
    servoRotate5(90);
    __delay_ms(1000);
    servoRotate5(0);
}


