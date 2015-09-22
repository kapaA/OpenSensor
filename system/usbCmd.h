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
        File name: usbCMD.h
        Version:   v0.0
        Date:      2012-08-09
    =========================================================================
*/


/*
** ==========================================================================
**                        INCLUDE STATEMENTS
** ==========================================================================
*/
#include "../commonVar.h"
#include "../commonDef.h"

/**
 * Enumeration containing the commands recieved from the PC
*/
typedef enum
{    
    SET_DEVICE_MAC_ADDR = 1,
    GET_DEVICE_MAC_ADDR = 2,
    SET_DEST_MAC_ADDR   = 3,
    GET_DEST_MAC_ADDR   = 4,
    SET_DATA_RATE       = 5,
    SET_DATARATE_TIKS   = 6,
    CW_ALGO             = 7,
    RESET_SPI_NRF       = 8,
    ASYM_RATE           = 9,
    START_DATA_TX       = 10,
    STOP_DATA_TX        = 11,
    START_CODING        = 12,
    STOP_CODING         = 13,
    SET_MIN_CW          = 14,
    SET_MAX_CW          = 15,
    SET_MAC_RE          = 16,
    SET_TEST_CASE       = 17,
    SET_HOLD_TIME       = 18,
    SET_KB_OP           = 19,
    SET_CW_UPDAT_mCWmin = 20,
    SET_CW_UPDAT_minCWmin = 21,
    SET_CW_UPDAT_maxCWmin = 22,
    DR_CAPA_TEST          = 23,
    GET_CONF_DATA       = 24
    
}UBS_CMD;



GLOB_RET usbCMD(char *buffer);

void printConfData(void);
