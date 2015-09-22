/*  ============================================================================
    Copyright (C) 2012 - 2014 Mobile Devices AAU.
    All rights reserved.
    ============================================================================
    This document contains proprietary information belonging to Achuthan.
    Passing on and copying of this document, use and
    communication of its contents is not permitted without prior written
    authorisation.
    ============================================================================
    Revision Information:
        File name: spi.c
        Version:   v0.0
        Date:      2012-08-09
    ============================================================================
*/

/*============================================================================*/
/*                           INCLUDE STATEMENTS
==============================================================================*/

#include "spi.h"

/*==============================================================================
** Function...: spi
** Return.....: GLOB_RET
** Description: initializes the spi module
** Created....: 09.08.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/

GLOB_RET spi(initModul *im)
{
    SPI1CON1bits.DISSCK = 0; //Internal Serial Clock is Enabled.
    SPI1CON1bits.DISSDO = 0; //SDO1 pin is controlled by the module.
    SPI1CON1bits.MODE16 = 0; //Communication is word-wide (16 bits).
    SPI1CON1bits.SMP    = 0; //Input Data is sampled at the middle of data
                             //output time.
    SPI1CON1bits.CKE    = 1; //Serial output data changes on transition from
                             //Idle clock state to active clock state
    SPI1CON1bits.CKP    = 0; //Idle state for clock is a low level; active
                             //state is a high level
    SPI1CON1bits.SSEN   = 0;
    SPI1CON1bits.MSTEN  = 1; //Master Mode Enabled
    SPI1CON1bits.SPRE   = 0x07;
    SPI1CON1bits.PPRE   = 2;

    SPI1STATbits.SPIEN  = 1; //Enable SPI Module

    RPOR7bits.RP15R     = 7; // SDO1 uses pin RP15
    RPOR4bits.RP8R      = 8; // SCK1 uses pin RP8
    RPINR20bits.SDI1R   = 9; // SDI1 uses pin RP9

    im->spi = 1;

    return GLOB_SUCCESS;
}

/*------------------------------------------------------------------------------
** Function...: Spi1WriteRead
** Return.....: unsigned char uppon read
** Description: readwrite to nrf register with SPI
** Created....: 25.06.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/
uint8_t SPI_wr(uint8_t data){

    #if defined (SIMULATION)
        return 0;
    #endif

    uint8_t SPI_response;
    SPI1STATbits.SPIROV = 0; // Reset overflow bit
    SPI1BUF = data; //Write character to SPI buffer

    while(!SPI1STATbits.SPIRBF); // Wait for character

    SPI_response = SPI1BUF;    //Read and discard data to avoid overflow
    return SPI_response;
}
