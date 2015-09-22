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
        File name: coding.c
        Version:   v0.0
        Date:      2012-09-11
    ==========================================================================*/

/*
** =============================================================================
**                                  INCLUDE STATEMENTS
** =============================================================================
*/

#include "datalink.h"

/*============================================================================*/
/*                           LOCAL VARIABLES
==============================================================================*/
static int sendToFlowA = 0;
static int sendToFlowB = 0;
unsigned int pCnt = 0;

/*==============================================================================
** Function...: coding
** Return.....: GLOB_RET
** Description: Inter session coding
** Created....: 12.09.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
GLOB_RET coding(frame *f, unsigned long *readTime)
{
    GLOB_RET ret = CODING_FAILURE;

    ret = coding_opportunity(f, readTime);

    return ret;
}

/*==============================================================================
** Function...: coding_opportunity
** Return.....: GLOB_RET
** Description: Find coding opportunity in keep buffer
** Created....: 12.09.2012 by Achuthan
** Modified...: 20.07.2013 by Achuthan: new coding strategy ( one buffer )
==============================================================================*/
GLOB_RET coding_opportunity(frame *f, unsigned long *rT)
{
    //CHANGE! 25-07-2013 No input for coding anymore
    GLOB_RET ret = CODING_FAILURE;

    // variable i is the number of first packet in keep buffer we whant to
    // look in to.
    int i = keepBufPar.read_pointer+1;
    int j = 0;
    unsigned long readTimeA=0, readTimeB = 0;
    frame tempFrameB;   // placeholder frame we read out from keep buffer
    frame tempFrameA;  // placeholder frame we would like to code
    frame frameCoded;
    unsigned long elapsedTime = 0; // used to holding time controll

    if(i >= BUFFER_SIZE)
    {
        i = 0;
    }

    do
    {
        read_buffer(&tempFrameA, &keepBufPar, 2, &readTimeA);
        for(j = 0; j < BUFFER_SIZE; j++)
        {
            ret = get_frame(&tempFrameB, &readTimeB , j, &keepBufPar);
            if(GLOB_SUCCESS == ret)
            {
                if(tempFrameA.destAddr == tempFrameB.payload.isncHeader.originalSrcAddr &&
                tempFrameA.payload.isncHeader.originalSrcAddr == tempFrameB.destAddr )
                {
                    // Found coding opportunity
                    xore(&tempFrameA,&tempFrameB, &frameCoded);

                    remove_frame(j, &keepBufPar);
                    remove_frame(i, &keepBufPar);
                    
                    // ---> IMPORTENT!!!! ENABLE INTERRUPT
                    frameCoded.payload.isncHeader.optimalCWmin_A = Log.nodeCWminUpdate_A;
                    frameCoded.payload.isncHeader.optimalCWmin_B = Log.nodeCWminUpdate_B;
                    ret = sendPacket(frameCoded);
                    j = 0;
                    break;
                }
            }
        }
    }while(j < BUFFER_SIZE);
        // the drawen packet from keep buffer was not codeable, so
        // we see if the holding time is reached
    elapsedTime = timer_ms - readTimeA;
    if( elapsedTime > Nclme.holdingTime) // Holding time exceeted
    {
        elapsedTime = elapsedTime;

        ret = remove_frame(i, &keepBufPar);

        if(DEFAULT_TEST == Sme.tc.cType)
        {
            frameCoded.payload.isncHeader.optimalCWmin_A = Log.nodeCWminUpdate_A;
            frameCoded.payload.isncHeader.optimalCWmin_B = Log.nodeCWminUpdate_B;
            ret = sendPacket(tempFrameA);
        }
        else
        {
            tempFrameA.destAddr = Sme.tc.cLinkTarget;
            frameCoded.payload.isncHeader.optimalCWmin_A = Log.nodeCWminUpdate_A;
            frameCoded.payload.isncHeader.optimalCWmin_B = Log.nodeCWminUpdate_B;
            ret = sendPacket(tempFrameA);
        }

        return CODING_FWD;
    }
    ret = GLOB_SUCCESS;
    return ret;
}

void xore(frame *frameX, frame *frameY, frame *frameCoded)
{
    int uniCastTo = 0; // destanation mac address to the node we unicast to
    uniCastTo = ( rand() % 2) + 1;

    if(1 == uniCastTo)
    {
        frameCoded->srcAddr  = frameY->srcAddr;
        frameCoded->destAddr = frameY->destAddr;
        frameCoded->seqNr    = pCnt;
        frameCoded->payload.isncHeader.originalSrcAddr = frameY->payload.isncHeader.originalSrcAddr;
        frameCoded->payload.isncHeader.secondDestAddr  = frameX->payload.isncHeader.secondDestAddr;
        frameCoded->payload.isncHeader.firstSeqNr      = frameY->payload.isncHeader.firstSeqNr;
        frameCoded->payload.isncHeader.secondSeqNr     = frameX->payload.isncHeader.firstSeqNr;
    }
    else if(2 == uniCastTo)
    {
        frameCoded->srcAddr  = frameX->srcAddr;
        frameCoded->destAddr = frameX->destAddr;
        frameCoded->seqNr    = pCnt;
        frameCoded->payload.isncHeader.originalSrcAddr = frameX->payload.isncHeader.originalSrcAddr;
        frameCoded->payload.isncHeader.secondDestAddr  = frameY->payload.isncHeader.secondDestAddr;
        frameCoded->payload.isncHeader.firstSeqNr      = frameX->payload.isncHeader.firstSeqNr;
        frameCoded->payload.isncHeader.secondSeqNr     = frameY->payload.isncHeader.firstSeqNr;
    }

    frameCoded->payload.isncHeader.codedPacket = CODING;
    frameCoded->type = NC_PAY;

    pCnt++;
    if(127<pCnt) pCnt = 0;
}

















