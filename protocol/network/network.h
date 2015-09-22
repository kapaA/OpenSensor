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
        File name: network.h
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

GLOB_RET network_layer(frame tempFrame, unsigned long readTime );
GLOB_RET kbBufOptimizer(frame *f, unsigned long readTimeA);

GLOB_RET network_outgoing( frame *f);
GLOB_RET network_incoming( frame *f);
char findNextHop(void);
void relaying(frame *f);

