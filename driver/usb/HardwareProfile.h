#ifndef HARDWARE_PROFILE_PIC24FJ64GB002_H
#define HARDWARE_PROFILE_PIC24FJ64GB002_H

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
    //#define tris_self_power     TRISAbits.TRISA2    // Input
    #define self_power          1

    //#define USE_USB_BUS_SENSE_IO
    //#define tris_usb_bus_sense  TRISBbits.TRISB5      // Input
    //#define USB_BUS_SENSE       1

    /*******************************************************************/
    /*******************************************************************/
    /*******************************************************************/
    /******** Application specific definitions *************************/
    /*******************************************************************/
    /*******************************************************************/
    /*******************************************************************/

    #define CLOCK_FREQ 32000000
    #define GetSystemClock() CLOCK_FREQ
    #define GetPeripheralClock() CLOCK_FREQ

    /** UART ***********************************************************/
    #define BAUDRATE2       9600UL
    #define BRG_DIV2        16
    #define BRGH2           0

#endif  //HARDWARE_PIC24FJ64GB002_H
