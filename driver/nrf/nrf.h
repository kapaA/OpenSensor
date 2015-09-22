/*  =========================================================================
    Copyright (C) 2012 - 2014 Mobile Devices AAU.
    All rights reserved...sounds awsome
    =========================================================================
    This document contains proprietary information belonging to Achuthan.
    Passing on and copying of this document, use and
    communication of its contents is not permitted without prior written
    authorisation.
    =========================================================================
    Revision Information:
        File name: nrf.h
        Version:   v0.0
        Date:      2012-08-09
    =========================================================================
*/

/*
** ==========================================================================
**                        INCLUDE STATEMENTS
** ==========================================================================
*/

#include "nrfbits.h"
#include "nrfcfg.h"

#include "../spi/spi.h"


/*
** =============================================================================
**                       EXPORTED FUNCTION DECLARATION
** =============================================================================
*/



GLOB_RET TXPacket(void);

/**
 * Description: initializes the nrf module
 * @param: dependency init module validation
 * @return: global return code
*/
GLOB_RET nrf(initModul *im);


/**
 * Description: configuring the nrf chip
 * @param: void
 * @return: global return code
*/
GLOB_RET initNRF(void);


/**
 * Description: set payload width
 * @param: type: TX or RX, pw: size
 * @return: void
*/
void setNRFpw(int type, char pw);


/**
 * Description: clock out the frame
 * @param: type: 
 * @return: void
*/
frame clockOut(int size);



