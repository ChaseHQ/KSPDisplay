/* 
 * File:   KSPDisplayAnalogExt.c
 * Author: CraigVella
 *
 * Created on August 15, 2013, 5:12 PM
 */

#include "KSPDisplayAnalogExt.h"

/*
 * 
 */
void main() {
    // Startup and Initialize Analog module
    initializeModule();

    unsigned char cCurrentModule = 0;
    unsigned char cBuffer = 0;

    while (1) {
        if (cCurrentModule > 3)
            cCurrentModule = 0;
        SSPBUF = (cCurrentModule + 1) % 4; // Request Data for Next Module
        BFWAIT();
        cBuffer = SSPBUF; // cBuffer Contains data for current Module
        switch (cCurrentModule) {
            case 0:
                PWM1DCH = cBuffer;
                break;
            case 1:
                PWM2DCH = cBuffer;
                break;
            case 2:
                PWM3DCH = cBuffer;
                break;
            case 3:
                PWM4DCH = cBuffer;
                break;
        }
        ++cCurrentModule;
    }
}

void initializeModule() {
    OSCCON = 0b01111000; // 16 MHZ and Select From FOSC Config

    // Go All Digital
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;

    TRISCbits.TRISC6 = 1; // Input on SS
    TRISBbits.TRISB6 = 1; // Input on SCK
    TRISBbits.TRISB4 = 1; // Input on SDI

    // Timer for PWM Enable
    T2CON = 0b00000101; // Prescaler to 4x
    PR2 = 0xFF; // Full 255 Fallover
    
    // PWMs Enabled
    PWM1CON = 0b11000000; // Enable PWM1
    PWM1DCH = 0x00; // Duty cycle To 0
    PWM2CON = 0b11000000; // Enable PWM2
    PWM2DCH = 0x00; // Duty cycle To 0
    PWM3CON = 0b11000000; // Enable PWM3
    PWM3DCH = 0x00; // Duty cycle To 0
    PWM4CON = 0b11000000; // Enable PWM4
    PWM4DCH = 0x00; // Duty cycle To 0

    // SPI Configuration (Slave Mode)
    SSPSTAT = 0b01000000; // Data on idle to Active Clock
    SSPCON1 = 0b00100100; // Slave mode with SS enabled
}