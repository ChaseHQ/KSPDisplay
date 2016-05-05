/* 
 * File:   HardwareProfile.h
 * Author: Craig Vella
 *
 * Created on July 24, 2013, 4:40 PM
 */

#ifndef HARDWAREPROFILE_H
#define	HARDWAREPROFILE_H

    /*******************************************************************/
    /******** USB stack hardware selection options *********************/
    /*******************************************************************/
    //This section is the set of definitions required by the MCHPFSUSB
    //  framework.  These definitions tell the firmware what mode it is
    //  running in, and where it can find the results to some information
    //  that the stack needs.
    //These definitions are required by every application developed with
    //  this revision of the MCHPFSUSB framework.  Please review each
    //  option carefully and determine which options are desired/required
    //  for your application.

    //#define USE_SELF_POWER_SENSE_IO
    #define tris_self_power     TRISCbits.TRISC2    // Input
    #define self_power          1

    //#define USE_USB_BUS_SENSE_IO
    #define tris_usb_bus_sense  TRISDbits.TRISD2    // Input
    #define USB_BUS_SENSE       TRISDbits.RD2

// Device Vendor Indentifier (VID) (0x04D8 is Microchip's VID)
#define USB_VID	0x04D8

// Device Product Indentifier (PID) (0x0042)
#define USB_PID	0x0042

// Manufacturer string descriptor
#define MSDLENGTH	11
#define MSD		'C','r','a','i','g',' ','V','e','l','l','a'

// Product String descriptor
#define PSDLENGTH	11
#define PSD		'K','S','P',' ','D','i','s','p','l','a','y'

// Device serial number string descriptor
#define DSNLENGTH	7
#define DSN		'K','S','P','D','1','.','0'

#endif	/* HARDWAREPROFILE_H */

