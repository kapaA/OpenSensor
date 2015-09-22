/*  =========================================================================
    Copyright (C) 2012 - 2014 Mobile Devices AAU.
    All rights reserved.
    =========================================================================
    This document contains proprietary information belonging to Achuthan.
    Passing on and copying of this document, use and
    communication of its contents is not permitted without prior written
    authorisation.
    =========================================================================
    Revision Information:
        File name: backOfAlgo.c
        Version:   v0.0
        Date:      2012-07-31
    =========================================================================
*/

/*============================================================================*/
/*                           INCLUDE STATEMENTS
==============================================================================*/
#include "datalink.h"
#include <math.h>
#include <stdlib.h>  

/*============================================================================*/
/*                           LOCAL VARIABLES
==============================================================================*/




int backOff(void)
{
    int backofSlot = SLOT_TIME; 

    START_DIFS_TIMER;

    // countdown to 0 while no CD if CD return 1 (CD detected)
    while(!CD)
    {
        if(difsTimer >= backofSlot)
        {
            backoffTimer--;
            START_DIFS_TIMER;
        }
        if(backoffTimer == 0)  // FIXME Magic number
        {
            return 0; // FIXME Magic number
        }
    }
    return 1;
}

void selectBackoff(void)
{
  backoffTimer = ( rand() % cw ) + 1; // get a random number within [0-cw]
}

void selectCW(void)
{
  if(cw<cw_max)
  {
    cw = 2*cw; // FIXME Magic number
  }

}

