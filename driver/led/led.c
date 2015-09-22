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
        File name: led.c
        Version:   v0.0
        Date:      2012-08-09
    ============================================================================
*/

/*============================================================================*/
/*                           INCLUDE STATEMENTS
==============================================================================*/

#include "led.h"


/*------------------------------------------------------------------------------
** Function...: led
** Return.....: GLOB_RET
** Description: Initialize the debugging led
** Created....: 09.01.2012 by Simon
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/

GLOB_RET led(void)
{

    TRISAbits.TRISA1 = OUTPUT;
    TRISBbits.TRISB0 = OUTPUT;
    TRISBbits.TRISB1 = OUTPUT;

    return GLOB_SUCCESS;
}

