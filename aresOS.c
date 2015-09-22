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
        File name: aresOS.c
        Version:   v1.0
        Date:      2013-22-04
    =========================================================================
*/

/*
** ==========================================================================
**                        INCLUDE STATEMENTS
** ==========================================================================
*/
#include "commonVar.h"
#include "commonDef.h"
#include <p24Fxxxx.h>


/*
** =============================================================================
**                   LOCAL EXPORTED FUNCTION DECLARATIONS
** =============================================================================
*/

/**
 * Description: Main Application layer , all calls must go thou this interface
 * @param:      OS task: incoming and out going taks
 * @return:     If MAC and APP id is given we are ready to run in this case
 *             this function will return APP_CONF_SUCCESS, else APP_CONF_NOT_SET
*/
GLOB_RET dataRateSweep_app_interface(int osTask);

/*============================================================================*/
/*                           MCU CONFIGURATION                                */
/*============================================================================*/

_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_NONE & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_ON)
_CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_LPRC & DSBOREN_OFF & DSWDTEN_OFF)

/*
_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_XT & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV4 & IESO_ON)
_CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_LPRC & DSBOREN_OFF & DSWDTEN_OFF)
*/
/*==============================================================================
** Function...: main
** Return.....: int
** Description: Main function (OS)
** Created....: 22.04.2013 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
int main(void) {
    GLOB_RET ret      = GLOB_FAILURE;  // Return codes
    static int osTask = 1;

    ret = init();

    
    #if defined (SIMULATION)
        deviceMacAddr = RELAY;
        Alme.aID = RELAY;
        Alme.aDest = 0x0;
        Nclme.codingEnabled = YES;
        Sme.tc.cTicksTx = 100;
        Sme.tc.cDRSweepE = NO;
        cw_algo = YES;

        /*
        Sme.tc.cType = BEST_LINK;
        Sme.tc.cAsymDropNode = NODE_A;
        Sme.tc.cLinkTarget   = NODE_B;
        Sme.tc.cLinkOh       = NODE_A;
         */

    #endif

    if(GLOB_SUCCESS == ret)
    {
        while(ACTIVE)
        {
            Log.whileRuns++;
            dataRateSweep_app_interface(osTask);
            osTask++;
            if(osTask>2) osTask = 0;
        }
    }
    return (ret);
}
