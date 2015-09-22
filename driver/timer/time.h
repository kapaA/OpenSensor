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
#include "../third_party_sw/usb_stack/usb_function_cdc.h"
#include "../../commonVar.h"
#include "../../commonDef.h" 

/*
** =============================================================================
**                       EXPORTED FUNCTION DECLARATION
** =============================================================================
*/

/**
 * Description: initializes the timer
 * @param: dependency init module validation
 * @return: global return code
*/
GLOB_RET timer(initModul *im);

/**
 * Description: timer interrupt
*/
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void);
