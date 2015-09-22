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
        File name: spi.h
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

/*
** =============================================================================
**                       EXPORTED FUNCTION DECLARATION
** =============================================================================
*/

/**
 * Description: initializes the spi module
 * @param: dependency init module validation
 * @return: global return code
*/
GLOB_RET spi(initModul *im);

/**
 * Description: Read/Write data from/to SPI
 * @param: data to write to the SPI
 * @return: data read from the SPI
*/
uint8_t SPI_wr(uint8_t data);
