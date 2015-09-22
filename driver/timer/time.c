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
        File name: time.c
        Version:   v0.0
        Date:      2012-08-09
    ============================================================================
*/

/*============================================================================*/
/*                           INCLUDE STATEMENTS
==============================================================================*/

#include "time.h"

/*
** =============================================================================
**                                   GLOBAL VARIABLES
** =============================================================================
*/
unsigned int oneSecTimer    = 0;
unsigned int dataRateTimer  = 0;

unsigned int ackTimer       = 0;

unsigned int sifsTimer      = 0;
unsigned int difsTimer      = 0;

unsigned int cwAlgoTimer    = 0; // Special TEST-CASE VAR

unsigned int time_tik = 0;
unsigned long timer_ms = 0;

unsigned long longCounter = 0;

unsigned int simuTimer = 0;


/*------------------------------------------------------------------------------
** Function...: timer
** Return.....: GLOB_RET
** Description: Initialize timer interrupt
** Created....: 09.01.2012 by Simon
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/

GLOB_RET timer(initModul *im)
{
   if(!im->spi) return GLOB_ERROR_INIT; // If spi was not initialized then error

   PR1 = 0x19;
                           
   T1CONbits.TSIDL = 0;  //Continue module operation in Idle mode
   T1CONbits.TGATE = 0;  //Gated time accumulation is disabled
   T1CONbits.TCKPS = 0b10; //scale 1:1
   T1CONbits.TSYNC = 0;    //Do not synchronize external clock input
   T1CONbits.TCS   = 0;  //Internal clock (FOSC/2)

   IPC0bits.T1IP   = 5;	 //set interrupt priorit (Between 1 and 7)

   IFS0bits.T1IF   = 0;	 //reset interrupt flag
   IEC0bits.T1IE   = 1;	 //turn on the timer1 interrupt

   T1CONbits.TON   = 1;  //Start timer

   im->timer = 1;

   return GLOB_SUCCESS;
}


/*------------------------------------------------------------------------------
** Function...: timer ISR
** Return.....: void
** Description: ISR
** Created....: 09.08.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
    oneSecTimer++;
    dataRateTimer++;
    cdcTxServiceTimer++;

    ackTimer++;
    sifsTimer++;
    difsTimer++;

    cwAlgoTimer++;

    time_tik++;

    if(oneSecTimer > 10000 && RELAY == deviceMacAddr)
    {
        START_ONESEC_TIMER;
        if(YES==cw_algo ) protocolController();
        printStats();
    }


    if(time_tik >= 100)
    {
        timer_ms++;
        time_tik = 0; 
        longCounter++;    //counting ms
    }

    

    if(cwAlgoTimer >= 65000)
    {  // Reset to avoid overflow???
      cwAlgoTimer = 0;
    }
    
    if(Sme.tc.cDRSweepE == YES && dataRateTimer >= Sme.tc.cTicksTx)
    {
       dataRateTimer = 0;
       dataRateSweep();
    }


    if(cdcTxServiceTimer>200) //USB Tx service needs to be called continuesly
    {
	if(cdcTxServiceTimer>65000)
	{
            cdcTxServiceTimer = 0;
	}

        if(semph == 0)
	{
            if(USBUSARTIsTxTrfReady()) //&& (semph == 0))
            {
                    if (OUTbuffer[0] != 0)
                    {
                        putsUSBUSART(OUTbuffer);
                        //resetOutBuffer();
                    }
            }
            CDCTxService();
            //resetOutBuffer();
            cdcTxServiceTimer = 0;
	}
    }
    if(dataRateTimer >= 30000)
    {  // Reset to avoid overflow???
      dataRateTimer = 0;
    }
    if(sifsTimer >= 65000)
    {  // Reset to avoid overflow???
      sifsTimer = 0;
    }
    if(difsTimer >= 65000)
    {
       difsTimer = 0;
    }

    if(oneSecTimer >= 65000)
    {  // Reset to avoid overflow???
      oneSecTimer = 0;
    }

    if(ackTimer >= 65000)
    {  // Reset to avoid overflow???
      ackTimer = 0;
    }
    #if defined (SIMULATION)
        simuTimer++;
        if(simuTimer>10)
        {
            simuTimer=0;
            //simulation_testCases(INCOMING);
            simulation_testCases(INCOMING);
        }
        if(oneSecTimer > 100 )
        {
            START_ONESEC_TIMER; // reset timer
            if(YES==cw_algo) protocolController();
            printStats();
        }
    #endif
    IFS0bits.T1IF = 0;
}
