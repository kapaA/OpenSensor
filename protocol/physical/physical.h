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
        File name: physical.h
        Version:   v0.0
        Date:      2012-08-09
    =========================================================================
*/

/*
** ==========================================================================
**                        INCLUDE STATEMENTS
** ==========================================================================
*/


GLOB_RET transmitFrame(frame tempFrame, char pw);
int receiveFrame(frame *tempFrame);
int receiveFrameLinkTest(frame *tempFrame);

/**
 * Description: external interrupt connected to DR pin
*/
int __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void);
