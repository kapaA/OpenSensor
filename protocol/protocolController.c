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
        File name: protocolController.c
        Version:   v0.0
        Date:      2013-09-24
    ==========================================================================*/

/*
** =============================================================================
**                                  INCLUDE STATEMENTS
** =============================================================================
*/

#include "../commonVar.h"
#include "../commonDef.h"

/*============================================================================*/
/*                           LOCAL VARIABLES
==============================================================================*/
GLOB_RET pc_relay_bi_control_1(void);
GLOB_RET update_ed_cw(void);
int findMeadian(int a, int b, int r);
int compare (const void * a, const void * b);

#define ELEMENTS 3
int values[]={0,0,0};

/*==============================================================================
** Function...: protocolController
** Return.....: GLOB_RET
** Description: Protocol controller interface
** Created....: 24.09.2013 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
GLOB_RET  protocolController( void )
{
    GLOB_RET ret = GLOB_SUCCESS;

    if(  RELAY == deviceMacAddr  )
    {
        ret = pc_relay_bi_control_1();
    }
    else if(  RELAY != deviceMacAddr  )
    {
        ret = update_ed_cw();
    }
    else
    {
        ret = GLOB_ERROR_INVALID_PARAM;
    }
    return ret;
}

/*==============================================================================
** Function...: pc_relay_bi_control_1
** Return.....: GLOB_RET
** Description: Relay controlles both flows A and B by CW adaptation
** Created....: 24.09.2013 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
GLOB_RET pc_relay_bi_control_1(void)
{
    char s[96];
    char cwA= 0, cwB = 0;
    unsigned char r_i = 0, r_o = 0;

    int median=0, alpha=0, cwSum =0;

    int minLim = Nclme.algoMinCWmin, maxLim=Nclme.algoMaxCWmin;
    int defaultCW = Nclme.medianCWmin;

    r_i = Log.totalRx;
    r_o = Log.servRate;

    // If flow A->R is below B->R
    if( Log.aRcvCnt_app <  Log.bRcvCnt_app )
    {
        if ( maxLim > Log.nodeCWminUpdate_B ) cwB++;
        else cwA--;
    }
    // If flow B->R is below A->R
    else if( Log.aRcvCnt_app >  Log.bRcvCnt_app )
    {
        if ( maxLim > Log.nodeCWminUpdate_A ) cwA++;
        else cwB--;
    }
    // If all incomming traffic is greater than the outgoing flow at the raly
    if(r_i > r_o)
    {
        // If flow A->R is below B->R
        if(0!=cwB)
        {

            if ( maxLim > Log.nodeCWminUpdate_B ) cwB++;
            else cwA--;
        }
        else if (0!=cwA)
        {
            if ( maxLim > Log.nodeCWminUpdate_A ) cwA++;
            else cwB--;
        }

        // If CW of R is larger than the limit
        if(cw_min > minLim) cw_min --;
    }
    else
    {
        cw_min++;
    }

    Log.nodeCWminUpdate_A = Log.nodeCWminUpdate_A + cwA;
    Log.nodeCWminUpdate_B = Log.nodeCWminUpdate_B + cwB;


    // Rescale the CW around the median
    median =  findMeadian(Log.nodeCWminUpdate_A,Log.nodeCWminUpdate_B,cw_min);
    cwSum = cw_min + Log.nodeCWminUpdate_A + Log.nodeCWminUpdate_B;
    if(median!=0)
    {
        alpha=(defaultCW*cwSum)/median;
    }
    else
    {
         sprintf(s,"LOG_ERROR 15\n");
         output_serial(s, sizeof(s));
    }

    if(cwSum!=0)
    {
        Log.nodeCWminUpdate_A = (Log.nodeCWminUpdate_A*alpha)/(cwSum);
        Log.nodeCWminUpdate_B = (Log.nodeCWminUpdate_B*alpha)/(cwSum);
        cw_min                = (cw_min*alpha)/(cwSum);
    }
    else
    {
       sprintf(s,"LOG_ERROR 16\n");
       output_serial(s, sizeof(s));
    }
    

    if(Log.nodeCWminUpdate_A > maxLim) Log.nodeCWminUpdate_A = maxLim;
    if(Log.nodeCWminUpdate_B > maxLim) Log.nodeCWminUpdate_B = maxLim;
    if(cw_min > maxLim) cw_min = maxLim;

    if(Log.nodeCWminUpdate_A < minLim)  Log.nodeCWminUpdate_A = minLim;
    if(Log.nodeCWminUpdate_B < minLim)  Log.nodeCWminUpdate_B = minLim;
    if(cw_min < minLim)  cw_min = minLim;

}


/*==============================================================================
** Function...: update_ed_cw
** Return.....: GLOB_RET
** Description: Relay controlles both flows A and B by CW adaptation, here the
 *              ED's updates thier CW
** Created....: 24.09.2013 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
GLOB_RET update_ed_cw(void){

    if(Sme.tc.cCWminUpdater > 8 )
    {
        Sme.tc.cCWminUpdater = 8;
    }
    else if(Sme.tc.cCWminUpdater < 2 )
    {
        Sme.tc.cCWminUpdater = 2;
    }


    cw_min = Sme.tc.cCWminUpdater;

}

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}


int findMeadian(int a, int b, int r)
{
    values[0]=a;
    values[1]=b;
    values[2]=r;

    qsort(values, ELEMENTS, sizeof(values[0]), compare);
    return values[ELEMENTS/2];
}