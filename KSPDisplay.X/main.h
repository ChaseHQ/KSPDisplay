/* 
 * File:   main.h
 * Author: Craig Vella
 *
 * Created on July 24, 2013, 2:46 PM
 */

#ifndef MAIN_H
#define	MAIN_H

// Main start here
#include "HardwareConfig.h"

// Usb Includes

#include <USB/usb.h>
#include "HardwareProfile.h"
#include <USB/usb_function_hid.h>
#include <math.h>


// Define the globals for the USB data in the USB RAM of the PIC18F*550
#pragma udata
#pragma udata USB_VARIABLES=0x500
unsigned char ReceivedDataBuffer[64];
unsigned char ToSendDataBuffer[64];
#pragma udata

USB_HANDLE USBOutHandle = 0;
USB_HANDLE USBInHandle = 0;

#define BFWAIT() while (!SSPSTATbits.BF)
#define AN_BANK0 PORTBbits.RB2
    
void initialize(void);
void processUsbCommands(void);
void cycleDisplay(void);

#endif	/* MAIN_H */

