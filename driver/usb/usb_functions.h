/*  =========================================================================
    Copyright (C) 2012 - 2014 Mobile Devices AAU.
    All rights reserved...sounds awsome
    =========================================================================
    This document contains proprietary information belonging to Simon.
    Passing on and copying of this document, use and
    communication of its contents is not permitted without prior written
    authorisation.
    =========================================================================
    Revision Information:
        File name: time.h
        Version:   v0.0
        Date:      2012-08-09
    =========================================================================
*/

/*
** ==========================================================================
**                        INCLUDE STATEMENTS
** ==========================================================================
*/

#include "../../commonVar.h"
#include "../../commonDef.h"
#include "../third_party_sw/usb_stack/usb_function_cdc.h"
/*
** =============================================================================
**                       EXPORTED FUNCTION DECLARATION
** =============================================================================
*/

/**
 * Description: initializes usb serial communication
 * @param: dependency init module validation
 * @return: global return code
*/
GLOB_RET usb(initModul *im);
void resetInBuffer(void);
void resetOutBuffer(void);

//extern unsigned char DR_usb;
