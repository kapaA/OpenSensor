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
        File name: usbCmd.c
        Version:   v0.0
        Date:      2012-08-08
    ============================================================================
*/

/*============================================================================*/
/*                           INCLUDE STATEMENTS
==============================================================================*/

#include "config.h"


void config(int confData, char id)
{
    if(0==id)
    {
        destMacAddr = (unsigned char)confData;
        Alme.aDest = (unsigned char)confData;
    }
    else if(1==id)
    {
        deviceMacAddr = (unsigned char)confData;
        Alme.aID = (unsigned char)confData;
        srand(longCounter);
    }
    else if(2==id)
    {
        deviceMacAddr; //= (unsigned char)DataEERead(eeDeviceId);
    }
    else if (3==id)
    {
        if(DEFAULT_TEST == Sme.tc.cType)
        {
            Sme.tc.cAsymDropNode = NODE_A;
        }
        else if (BEST_LINK == Sme.tc.cType)
        {
            Sme.tc.cAsymDropNode = NODE_A;
            Sme.tc.cLinkTarget   = NODE_B;
            Sme.tc.cLinkOh       = NODE_A;
        }
        else if (WORST_LINK == Sme.tc.cType)
        {
            Sme.tc.cAsymDropNode = NODE_A;
            Sme.tc.cLinkTarget   = NODE_A;
            Sme.tc.cLinkOh       = NODE_B;
        }
    }
}
