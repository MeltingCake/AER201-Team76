/*
 * File:   main.c
 * Author: True Administrator
 *
 * Created on July 18, 2016, 12:11 PM
 */

#include <xc.h>
#include <stdio.h>
#include "configBits.h"
#include "constants.h"
#include "lcd.h"
#include "I2C.h"
#include "macros.h"
#include "eeprom.h"
#include "RTC.h"
#include "servos.h"
#include "sensors.h"
//#include "solenoids.h" //deprecated as solenoids are not being used
#include "motor.h"

enum states
{
    MAIN,
    EXECUTING,
    FINISH,
    LOG,
    SAVE,
    DEBUG
} state = MAIN;

const char keys[] = "123A456B789C*0#D";

int time[7];    //format [6] = year, [5] = month, [4] = day, [2] = hour, [1] = minute, [0] = time.
int starttime[7];
uint8_t logdata;
int runResult = 0;

int alNoTab = 0;
int alTab = 0;
int snNoLabel = 0;
int snLabel = 0;

void update_lcd()
{
    __lcd_home();

    switch (state)
    {
    case MAIN:
        printf("%02x/%01x/%02x ", time[6], time[5], time[4]); //Print date in YY/MM/DD
        printf("%02x:%02x:%02x", time[2], time[1], time[0]);  //HH:MM:SS
        //printf(value);
        __lcd_newline();
        printf("1:Start 2:Logs  ");
        break;
    case LOG:
        //logdata = eeprom_ReadByte(0x00FF);
        printf("%03d              ", logdata);
        __lcd_newline()
            printf("1:Prev 0:Exit   ");
        break;
    case EXECUTING:
        printf("Executing...    ");
        __lcd_newline();
        printf("aN%d/a%d/sN%d/S%d",alNoTab, alTab, snNoLabel, snLabel);
        //printf("0:Emergency Stop");
        break;
    case FINISH:
        //DEFINE GLOBAL VARIABLE TO HANDLE PRINTING RESULTS. 
        if(runResult == 1){
            printf("Finished        ");
        }
        else
        {
            printf("Terminated      ");
        }
        __lcd_newline();
        printf("0:Menu          ");
        break;
    default:
        printf("error");
        break;
    }

    __lcd_home();
    __lcd_newline();
}

int readKey()
{
    for (int i = 0; i < 200; i++)
    {
        if (PORTBbits.RB1 != 0)
        {
            unsigned char keypress = (PORTB & 0xF0) >> 4;
            while (PORTBbits.RB1 == 1)
            {
            }
            Nop(); //Apply breakpoint here because of compiler optimizations
            Nop();
            return keypress;
        }
    }
    return -1;
}

int get_timeDif(int start){
    get_time(time);
    int h_ten = (time[2] & 0b11110000)>>4;
    int h_one = (time[2] & 0b00001111);
    int m_ten = (time[1] & 0b11110000)>>4;
    int m_one = (time[1] & 0b00001111);
    int s_ten = (time[0] & 0b11110000)>>4;
    int s_one = (time[0] & 0b00001111);
    int start_s = (h_ten*10+h_one)*3600 + (m_ten*10 + m_one) * 60 + (s_ten * 10 + s_one);

    return start_s - start;
}

void mainState()
{
    LATBbits.LATB0 = 0;
    LATCbits.LATC6 = 0;
    while (1)
    {
        //performs all the tasks expected of main
        get_time(time);
        update_lcd();

        int keypress = readKey(); //reads key for any state changes.
        if(keypress == 0b0000){
            //EXECUTION START CODE  {enabling interrupts etc}
            //enable interrupts
            //flip master bit
            //printf executing
            
            state = EXECUTING;
            return;
        }else if(keypress == 0b0001){
            //LOGS START CODE
            state = LOG;
            return;
        }else if(keypress == 0b0011){
            state = DEBUG;
            return;
        }
    }
}

void executingState()
{
    //all sensor reading and actuator control will be done here..
    //define many local variables.
    //store results in a global var.
    
    int runCanSn, runCanAl;
    runCanSn = 0;
    runCanAl = 0;
    
    get_time(starttime);
    int h_ten = (starttime[2] & 0b11110000)>>4;
    int h_one = (starttime[2] & 0b00001111);
    int m_ten = (starttime[1] & 0b11110000)>>4;
    int m_one = (starttime[1] & 0b00001111);
    int s_ten = (starttime[0] & 0b11110000)>>4;
    int s_one = (starttime[0] & 0b00001111);
    int start_s = (h_ten*10+h_one)*3600 + (m_ten*10 + m_one) * 60 + (s_ten * 10 + s_one);

    __lcd_home();
    printf("initializing    ");
    __lcd_newline();
    printf("                ");
    
    
    
    LATBbits.LATB0 = 1; 
    __delay_ms(300);
    LATCbits.LATC6 = 1;
    
    servoRotate90n(4);
    servoRotate2(1);
    servoRotate0(2);
    servoRotate1(0);
    servoRotate4(3);
    di();
    update_lcd();
    int timedif = get_timeDif(start_s);


    int emptyCount;

    int snLoaded, alLoaded, snInSensor, alInSensor;
    snLoaded = 1;
    alLoaded = 1;
    snInSensor = 1;
    alInSensor = 1;
    emptyCount = 0;
    
    
    
    while(timedif < 180 && PORTBbits.RB0 == 1){
        update_lcd();
        __lcd_home();
        printf("Executing    %03d", emptyCount);
        //printf("%d, %d, %d, %d", PORTAbits.RA0, PORTAbits.RA1, PORTAbits.RA2, PORTAbits.RA3);
        
        if(readSnTop() == 0){
            snLoaded = 0;
        }
        if(readAlTop() == 0){
            alLoaded = 0;
        }
        if(readSnBot() == 0){
            snInSensor = 0;
        }
        if(readAlBot() == 0){
            alInSensor = 0;
        }
        
        if(snLoaded == 0){
            emptyCount = 0;
            if(snInSensor == 1){
                __delay_ms(400);
                dispenseSnCan();
                runCanSn = 1;
                snLoaded = 1;
            }
        }
        if(alLoaded == 0){
            emptyCount = 0;
            if(alInSensor == 1){
                __delay_ms(400);
                dispenseAlCan();
                runCanAl = 1;
                alLoaded = 1;
            }
        }
        
        if(snLoaded == 1 && alLoaded ==1){
            emptyCount++;
        }


        if(snInSensor == 0){
            int hasLabel;
            __delay_ms(800);
            emptyCount = 0;
            if(readSnSensor() == 1){
                hasLabel = 0;
            }else{
                hasLabel = 1;
            }
            if(hasLabel == 0){
                servoRotate90n(2);
                servoRotate0(2);
                runCanSn = 0;
                snNoLabel++;
            }else if(hasLabel == 1){
                servoRotate90(2);
                servoRotate0(2);
                runCanSn = 0;
                snLabel++;
            }
            snInSensor = 1;
        }

        if(alInSensor == 0){
            emptyCount = 0;
            int hasTab;
            __delay_ms(800);
            hasTab = servoRotateArm();
            servoRotate90n(4);
            if(hasTab){
                servoRotate90n(3);
                runCanAl = 0;
                alTab++;
            }else if (hasTab == 0){
                servoRotate90(3);
                runCanAl = 0;
                alNoTab++;
            }
            //__delay_ms(800);
            servoRotate4(3);
            alInSensor = 1;
        }

        timedif = get_timeDif(start_s);
        if(emptyCount > 50){
            state = FINISH;
            runResult = 1;
            return;
        }else if(timedif > 180){
            state = FINISH;
            runResult = 0;
            return;
        }
        __delay_ms(300);
    }
    
    LATCbits.LATC6 = 0;
    int res[4];
    res[0] = alTab;
    res[1] = alNoTab;
    res[2] = snLabel;
    res[3] = snNoLabel;
    eeprom_LogResult(res);
    state = FINISH;
}

void finishState()
{
    di();
    __delay_ms(1000);
    update_lcd();
    LATBbits.LATB0 = 0;
    LATCbits.LATC6 = 0;
    while(1){
        int keypress = readKey();
        if(keypress == 0b1101){
            state = MAIN;
            return;
        }
    }
}

void logState()
{
    update_lcd();
    int log = 0;
    while(1){
        int keypress = readKey();
        if(keypress == 0b0001){
            log++;
            eeprom_GetLog(log, &logdata);
            update_lcd();
            return;
        }else if(keypress == 0b1101){
            state = MAIN;
            return;
        }
    }
}

void debugState(){
    LATBbits.LATB0 = 1;
    __delay_ms(300);
    //LATCbits.LATC6 = 1;
    
    __lcd_home();
    printf("going debug");
    __lcd_newline();
    
    //servoRotate90n(4);
    //servoRotate4(3);
    //while(1){
    //servoRotateArm();
    //__delay_ms(300);
    //servoRotate90n(4);
    //}
    int keypress = readKey();
    if(keypress == 0b1101){
        state = MAIN;
    }
}

    void fixLCD(void){
    //LMAO
    OSCCON = 0xF0;  //8Mhz
    // RTC uses RC3 and RC4
    // LCD uses RD2 - RD7
    // KEYPAD uses RB4 - RB7 and RB1
    TRISA = 0x00; // Set Port A as all output
    TRISB = 0b11110010; // Set Keypad Pins as input, rest are output
    TRISC = 0x00011000; // Set I2C pins as input, rest are output
    TRISD = 0x00;   //All output mode
    TRISE = 0x00;   //RE0 and RE1 output
    LATA = 0x00;
    LATB = 0x00; 
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;
    ADCON0 = 0x00;  //Disable ADC
    ADCON1 = 0x00;  //Set PORTB to be digital instead of analog default  
    // nRBPU = 0;
    initLCD(); // Initialize LCD
    I2C_Master_Init(10000); //Initialize I2C Master with 100KHz clock
    di(); // Disable all interrupts
    unsigned char time[7];
    // Set the output ports to have alternating outputs
    LATA = 0b01010101;
    LATB = 0b01010101;
    LATC = 0b01010101;
    LATDbits.LATD0 = 1;
    LATDbits.LATD1 = 0;
    LATEbits.LATE0 = 1;
    LATEbits.LATE1 = 0;
    int i = 0; // Loop counter i
    int j = 0;
    while(j < 10){
        //Reset RTC memory pointer 
        I2C_Master_Start(); //Start condition
        I2C_Master_Write(0b11010000); //7 bit RTC address + Write
        I2C_Master_Write(0x00); //Set memory pointer to seconds
        I2C_Master_Stop(); //Stop condition

        //Read Current Time
        I2C_Master_Start();
        I2C_Master_Write(0b11010001); //7 bit RTC address + Read
        for(unsigned char i=0;i<0x06;i++){
            time[i] = I2C_Master_Read(1);
        }
        time[6] = I2C_Master_Read(0);       //Final Read without ack
        I2C_Master_Stop();
        __lcd_home();
        printf("%02x%02x/%02x ", time[6],time[5],time[4]);    //Print date in YY/MM/DD
        printf("%02x:%02x:%02x", time[2],time[1],time[0]);    //HH:MM:SS
        __lcd_newline();
        printf("              ");
        __lcd_home();
        __lcd_newline();
        // Flip State of all unused pins
        LATA = LATA ^ 0xFF;
        LATB = LATB ^ 0xFF;
        LATC = LATC ^ 0xFF;
        LATDbits.LATD0 = !LATDbits.LATD0;
        LATDbits.LATD1 = !LATDbits.LATD1;
        LATEbits.LATE0 = !LATEbits.LATE0;
        LATEbits.LATE1 = !LATEbits.LATE1;
        j++;
    }
}

void main(void)
{
    
    fixLCD();
    
    OSCCON = 0xF0; //8Mhz

    // RTC uses RC3 and RC4
    // LCD uses RD2 - RD7
    // KEYPAD uses RB4 - RB7 and RB1

    TRISA = 0b00001111;       // Upper bits as output SERVOS || Lower bits as input SENSORS
    TRISB = 0b11110010; // Set Keypad Pins as input, rest are output for DC motors and Solenoids
    TRISC = 0b00011011; // Set I2C pins as input, rest are output
    TRISD = 0x00;       //All output mode (LCD))
    TRISE = 0x00;       //RE0 and RE1 output  (Also used for SERVO)

    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;

    ADCON0 = 0x00; //Disable ADC
    ADCON1 = 0x0F; //Set PORTB to be digital instead of analog default
    CMCON = 0x07;

    // nRBPU = 0;
    initLCD(); // Initialize LCD
    eeprom_initialize();
    I2C_Master_Init(10000); //Initialize I2C Master with 100KHz clock
    
    INT1IE = 1;
    di();
    
    state = MAIN;
    
    /*
     Main loop to repeatedly poll for values. Establishes current state and executes functions based off of it, also reads for state changes based on key presses.
     */

    while (1)
    {
        switch (state)
        {
        case MAIN:
            mainState();
            break;
        case EXECUTING:
            executingState();
            break;
        case FINISH:
            finishState();
            break;
        case LOG:
            logState();
            break;
        case DEBUG:
            debugState();
            break;
        }
    }
    // This statement is here incase the main loop breaks, the return
    //statement will initiate a soft reset
    return;
}

void interrupt keypressed(void)
{
    if (INT1IF)
    {
        while(1)
        {
            unsigned char keypress = (PORTB & 0xF0) >> 4;
            if (keypress == 0b1101)
            {
                LATA = 0x00;
                LATB = 0x00;
                LATC = 0x00;
                LATD = 0x00;
                LATE = 0x00;
                LATCbits.LATC6 = 0;
                LATBbits.LATB0 = 0;
                state = FINISH;
                runResult = 0;
                di();
                break;
            }
        }
        INT1IF = 0; //Clear flag bit
    }
}
