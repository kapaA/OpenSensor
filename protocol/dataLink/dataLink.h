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
        File name: datalink.h
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
#include "../../driver/nrf/nrfbits.h"

GLOB_RET sendPacket(frame tempFrame);
void rxFrameHandler(frame *tempFrame);
GLOB_RET channelAccess(frame tempFrame, unsigned char retCode);
GLOB_RET prtocolNoRTSCTS(frame tempFrame);
GLOB_RET prtocolRTSCTS(frame tempFrame);

void xore(frame *frameX, frame *frameY, frame *frameCoded);
GLOB_RET packetController(frame *tempFrame);

void contUpdaterEndNodes( void );
GLOB_RET coding(frame *f, unsigned long *readTime);
GLOB_RET coding_opportunity(frame *f, unsigned long *readTime);







