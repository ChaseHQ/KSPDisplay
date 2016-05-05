/* 
 * File:   main.c
 * Author: Craig Vella
 *
 * Created on July 24, 2013, 2:46 PM
 */

#ifndef MAIN_C
#define	MAIN_C

#include "main.h"

long int _LEDDisplay[8];
unsigned char _AnalogMeter[6];

unsigned char _AlarmCondition = 0;
static const unsigned long int powerArray[8] = {1,10,100,1000,10000,100000,1000000,10000000};
unsigned char _cycleWaitMax = 10;
unsigned char _activeDisplays = 8;

void main(void) {
    unsigned char cycleWait = 0;
    unsigned char cBuffer = 0;

    cycleWait = 0;

    // Set up Microcontroller
    initialize();

    AN_BANK0 = 0; // Turn on Bank 0 Analog
    SSPBUF = cBuffer; // Ask what Meter is next to Get Data For
    BFWAIT();

    while (1) {
        // We're polling so handle this
        USBDeviceTasks();
        // Process Commands
        processUsbCommands();

        if (cycleWait >= _cycleWaitMax) { // Cycle wait
            cycleDisplay();
            cycleWait = 0;
        } else
            ++cycleWait;

        CCPR1L = _AnalogMeter[0]; // Set AnalogMeter0 0-255
        CCPR2L = _AnalogMeter[1]; // Set AnalogMeter1 0-255

        PORTCbits.RC0 = _AlarmCondition;
        
        // Add-on PWM Microcontroller
        cBuffer = SSPBUF; // This Contains the Meter we need the data for
        SSPBUF = _AnalogMeter[cBuffer + 2];

        BFWAIT(); // This doesn't make too much sense, i should be able to wait for data in the loop,
                  // instead of halting all of the code, but it seems to not want me to do that? Almost like
                  // USB is interfering with This, We waste cycles but it doesnt seem to hurt the Display

        ClrWdt();
    }
}

void cycleDisplay(void) {

    static unsigned char currentLitLED = 0;
    static unsigned char currentBank = 0;

    PORTA = 0xFF; // Blank for Switch
    PORTD = ((currentLitLED << 4) + (currentBank)); // Select Bank and LED
    PORTA = (_LEDDisplay[currentBank] / powerArray[currentLitLED]) % 10;

    currentLitLED++;

    if (currentLitLED >= 8) {
        currentLitLED = 0;
        currentBank++;
        if (currentBank >= _activeDisplays) {
            currentBank = 0;
        }
    }
        
}

void initialize(){
    unsigned char x = 0;
    // Microcontroller setup
    OSCCON = 0b01110000; // NO IDLE, 16MHZ, INTERNAL OSC, FREQ STABLE, SYSTEM CLOCK INTERNAL
    OSCCON2bits.INTSRC = 1; // HFINTOSC used - High Accuracy
    OSCCON2bits.PLLEN = 1; // PLL Enabled
    OSCCON2bits.SOSCGO = 0; // Secondary OSC shutdown
    OSCCON2bits.PRISD = 0; // Primary OSC Circuit Shutdown (Using Internal)
    OSCTUNE = 0x80; //3X PLL ratio mode selected
    while(OSCCON2bits.PLLRDY != 1);   //Wait for PLL lock

    ACTCON = 0b10010000; // Active Tuning of Clock by USB Host

    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;

    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;

    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

    // Set Everything Digital
    //TRISDbits.TRISD2 = 1; // Enable Input on D2

    USBOutHandle = 0;
    USBInHandle = 0;

    USBDeviceInit();

    // PWM Setup
    CCP1CON = 0b00001100;
    T2CON = 0b00000101; // 4x Prescale, 11.63KHz
    PR2 = 0xFF; // Fill Timer 0-5Volts
    CCPR1L = 0x00; // Load to 0

    // PWM 2 Setup
    CCP2CON = 0b00001100;
    CCPR2L = 0x00;

    //SPI Setup
    TRISBbits.TRISB0 = 1; // SDI (Input)
    SSPSTAT = 0b01000000; // Clock Edge Input
    SSP1CON = 0b00100001; // /16 mode 48/16 (3MHZ)

    AN_BANK0 = 1; // Turn  Bank 0 Analog Off

    // Clear out Initial Values
    for (x = 0; x < 8; ++x)
        _LEDDisplay[x] = 0;
    for (x = 0; x < 6; ++x)
        _AnalogMeter[x] = 0;
}

// Process USB commands
void processUsbCommands() {
    static unsigned char forLoopCounter = 0;

    // Check if we are in the configured state; otherwise just return
    if((USBDeviceState < CONFIGURED_STATE) || (USBSuspendControl == 1))
    {
        // We are not configured
        return;
    }

    // Check if data was received from the host.
    if(!HIDRxHandleBusy(USBOutHandle))
    {
		// Command mode
        switch(ReceivedDataBuffer[0])
	{
            case 0x80:  // Standard update
                // Recieve Number data
                // Unroll these loops - Gets rid of Perterbuations
                for (forLoopCounter = 0; forLoopCounter < 8; ++forLoopCounter) {
                    _LEDDisplay[forLoopCounter] = *(long int *)(&ReceivedDataBuffer[(forLoopCounter * 4) + 1]);
                }
                for (forLoopCounter = 0; forLoopCounter < 6; ++forLoopCounter) {
                    _AnalogMeter[forLoopCounter] = *(unsigned char *)(&ReceivedDataBuffer[forLoopCounter + 33]);
                }
                _AlarmCondition = *(unsigned char*)(&ReceivedDataBuffer[40]);
          	break;
            case 0x81: // Configuration packet
                _activeDisplays = *(unsigned char *)(&ReceivedDataBuffer[1]);
                _cycleWaitMax = *(unsigned char *)(&ReceivedDataBuffer[2]);
                break;
            case 0x82:  // Read the push switch status // Send Data
		//ToSendDataBuffer[0] = sw0;
		// Transmit the response to the host
                    if(!HIDTxHandleBusy(USBInHandle)){
                        USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
                    }
            	break;
            default:	// Unknown command received
       		break;
	}
        // Re-arm the OUT endpoint for the next packet
        USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
    }
}


// USB Callback handling routines -----------------------------------------------------------

// Call back that is invoked when a USB suspend is detected
void USBCBSuspend(void)
{
}

// This call back is invoked when a wakeup from USB suspend is detected.
void USBCBWakeFromSuspend(void)
{
}

// The USB host sends out a SOF packet to full-speed devices every 1 ms.
void USBCB_SOF_Handler(void)
{
    // No need to clear UIRbits.SOFIF to 0 here. Callback caller is already doing that.
}

// The purpose of this callback is mainly for debugging during development.
// Check UEIR to see which error causes the interrupt.
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.
}

// Check other requests callback
void USBCBCheckOtherReq(void)
{
    USBCheckHIDRequest();
}

// Callback function is called when a SETUP, bRequest: SET_DESCRIPTOR request arrives.
void USBCBStdSetDscHandler(void)
{
    // You must claim session ownership if supporting this request
}

//This function is called when the device becomes initialized
void USBCBInitEP(void)
{
    // Enable the HID endpoint
    USBEnableEndpoint(HID_EP,USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);

    // Re-arm the OUT endpoint for the next packet
    USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
}

// Send resume call-back
void USBCBSendResume(void)
{
    static WORD delay_count;

    // Verify that the host has armed us to perform remote wakeup.
    if(USBGetRemoteWakeupStatus())
    {
        // Verify that the USB bus is suspended (before we send remote wakeup signalling).
        if(USBIsBusSuspended())
        {
            USBMaskInterrupts();

            // Bring the clock speed up to normal running state
            USBCBWakeFromSuspend();
            USBSuspendControl = 0;
            USBBusIsSuspended = 0;

            // Section 7.1.7.7 of the USB 2.0 specifications indicates a USB
            // device must continuously see 5ms+ of idle on the bus, before it sends
            // remote wakeup signalling.  One way to be certain that this parameter
            // gets met, is to add a 2ms+ blocking delay here (2ms plus at
            // least 3ms from bus idle to USBIsBusSuspended() == FLAG_TRUE, yeilds
            // 5ms+ total delay since start of idle).
            delay_count = 3600U;
            do
            {
                delay_count--;
            } while(delay_count);

            // Start RESUME signaling for 1-13 ms
            USBResumeControl = 1;
            delay_count = 1800U;
            do
            {
                delay_count--;
            } while(delay_count);
            USBResumeControl = 0;

            USBUnmaskInterrupts();
        }
    }
}

// USB callback function handler
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch(event)
    {
        case EVENT_TRANSFER:
            // Application callback tasks and functions go here
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_CONFIGURED:
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        default:
            break;
    }
    return 1;
}

#endif
