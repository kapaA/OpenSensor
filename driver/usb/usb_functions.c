/*  ============================================================================
    Copyright (C) 2012 - 2014 Mobile Devices AAU.
    All rights reserved.
    ============================================================================
    This document contains proprietary information belonging to Simon.
    Passing on and copying of this document, use and
    communication of its contents is not permitted without prior written
    authorisation.
    ============================================================================
    Revision Information:
        File name: usb.c
        Version:   v0.0
        Date:      2012-08-09
    ============================================================================
*/

/*============================================================================*/
/*                           INCLUDE STATEMENTS
==============================================================================*/

#include "usb_functions.h"


/*
** =============================================================================
**                                   GLOBAL VARIABLES
** =============================================================================
*/

unsigned int cdcTxServiceTimer = 0;
char INbuffer[CDC_DATA_IN_EP_SIZE];
char OUTbuffer[CDC_DATA_OUT_EP_SIZE];
volatile unsigned char semph = 0; // semaphor for the output
//unsigned char DR_usb;

/*------------------------------------------------------------------------------
** Function...: usbInit
** Return.....: GLOB_RET
** Description: Initialize usb
** Created....: 14.08.2012 by Simon
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/
GLOB_RET usb(initModul *im)
{
    GLOB_RET ret = GLOB_ERROR_INIT;
    resetOutBuffer();
    resetInBuffer();
    CLKDIVbits.CPDIV = 0; //Set the system clock to 32mhz
    CLKDIVbits.RCDIV = 0; //make sure we send a 8mhz clock to PLLDIV

    if(im->spi == 0 || im->timer == 0 || im->nrf == 0) return ret;
    InitializeSystem(); //Initializin usb
    CDCSetLineCoding(19200, NUM_STOP_BITS_1, PARITY_NONE, 8); // Configuring the communication
    CDCInitEP(); //Initializing the cdc usb-serial emulator
    USBDeviceAttach(); //Called in interrupt mode
    START_USB_TX_TIMER;
    im->usb = 1;
    return GLOB_SUCCESS;
}

/*------------------------------------------------------------------------------
** Function...: resetInBuffer
** Return.....: GLOB_RET
** Description: Resets INbuffer so it wont send same message twice
** Created....: 14.08.2012 by Simon
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/

void resetInBuffer(void) // This function reset the output buffer
{
    USBMaskInterrupts();
    int i;
    for (i = 0; i < sizeof (INbuffer); i++) //This for-loop loops same amount of times as the size of the inputbuffer.
    {
        INbuffer[i] = 0; //Zero is inserted in every
    }
    USBUnmaskInterrupts();
}

void resetOutBuffer(void) // This function reset the output buffer
{
    //USBMaskInterrupts();
    int i;
    for (i = 0; i < sizeof (OUTbuffer); i++) //This for-loop loops same amount of times as the size of the inputbuffer.
    {
        OUTbuffer[i] = 0; //Zero is inserted in every
    }
    //USBUnmaskInterrupts();
}

int output_serial(char *buf, int length)
{
    semph = 1;
    int d = 0;
    int i = 0;
    int e = 0;
    
    while((OUTbuffer[i] != 0) && (i <= sizeof(OUTbuffer))) //CHANGE!
    {
        i = i + 1;
        /*if(i == sizeof(OUTbuffer))
        {
            semph = 0;
            return 1;
        }*/
    }
    //printf("output_buf: value of i: %d\n", i); //REMOVE!

    
    while((buf[d] != 0) && (d <= length))
    {
        d = d + 1;
        /*if(d == length)
        {
            semph = 0;
            return 1;
        }*/
    }
      

    //printf("test: value of d: %d\n", d); //REMOVE!

    if(i > (sizeof(OUTbuffer) - (d+1))) //CHANGE!
    {
        semph = 0;
        return 2;
    }

    for(e = 0; e < d; e++)
    {
        OUTbuffer[i] = buf[e];
        i++;
    }
    semph = 0;
    return 0;
}
