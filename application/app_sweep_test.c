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
        File name: app_sweep_test.c
        Version:   v0.0
        Date:      2013-04-1
    ==========================================================================*/

/*
** =============================================================================
**                                  INCLUDE STATEMENTS
** =============================================================================
*/

#include "app_sweep_test.h"


/*============================================================================*/
/*                           LOCAL VARIABLES
==============================================================================*/


/*==============================================================================
** Function...: dataRateSweep_app_interface
** Return.....: GLOB_RET
** Description: main application layer
** Created....: 01.05.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
GLOB_RET dataRateSweep_app_interface(int osTask)
{
    GLOB_RET ret = APP_CONF_FAILURE;
    ret = confControl();


    if(oneSecTimer > 10000 && RELAY != deviceMacAddr)
    {
        START_ONESEC_TIMER;
        Sme.tc.cReport.cIncPck   = Log.totalRx;
        Sme.tc.cReport.cTxReqest = Sme.tc.cDataRate;
        printStats();
    }
    

    if(APP_CONF_SUCCESS == ret)
    {
        switch(osTask)
        {
            case INCOMING:
                Log.inBufferHandlerRuns++;
                ret = app_incomming();
                break;

            case  OUTGOING:
                Log.outBufferHandlerRuns++;
                ret = app_outgoing();
                break;

            default:
                break;
        }
    }

    return ret;
}

/*==============================================================================
** Function...: app_incomming
** Return.....: GLOB_RET
** Description: private function that handles all incomming packets
** Created....: 01.05.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
GLOB_RET app_incomming(void)
{
    frame tempFrame;
    GLOB_RET ret = GLOB_FAILURE;
    unsigned long readTime = 0;
    unsigned long readTimeKB = 0;
    
    // Relay
    if(RELAY == deviceMacAddr)
    {
        // Emptying inbuffer for packets and push it to network layer
        while (! buffer_empty(&inBufPar))
        {
            // Relay read out one packet from inbuffer
            read_buffer(&tempFrame, &inBufPar, 1, &readTime);

            // For statistic
            if(0x01 == tempFrame.srcAddr)
            {
                Log.aRcvCnt_app++;
                Log.aRxTot = tempFrame.payload.isncHeader.rxTotal;
                Log.aTxReq = tempFrame.payload.isncHeader.txReq;
            }
            if(0x02 == tempFrame.srcAddr)
            {
                Log.bRcvCnt_app++;
                Log.bRxTot = tempFrame.payload.isncHeader.rxTotal;
                Log.bTxReq = tempFrame.payload.isncHeader.txReq;
            }
  
            if(Alme.drCapaTest)
            {
                return 0;
            }

            tempFrame.payload.isncHeader.testCaseType = Sme.tc.cType;

            // The packet is passed down to the network layer
            ret = network_layer(tempFrame, readTime);
        }

        if (! buffer_empty(&keepBufPar))
        {
            ret = coding(&tempFrame, &readTimeKB);
        }  
    }
    // End Devices
    else
    {
        if (! buffer_empty(&inBufPar))
        {
            // Reading out the recieved packet
            read_buffer(&tempFrame, &inBufPar,1, &readTime);

            // Contention window size update request from the relay
            if( 0x01 == deviceMacAddr)
            {
                Sme.tc.cCWminUpdater = tempFrame.payload.isncHeader.optimalCWmin_A;
            }
            else if (0x02 == deviceMacAddr)
            {
                Sme.tc.cCWminUpdater = tempFrame.payload.isncHeader.optimalCWmin_B;  
            }

            if ( 0 != Sme.tc.cCWminUpdater && Sme.tc.cCWminUpdater < 64)
            {
                cw_min = Sme.tc.cCWminUpdater;
                Log.avrCWmin = Log.avrCWmin + cw_min;
            }
            else
            {
                Log.avrCWmin = Log.avrCWmin + cw_min;
            }

            // For statistic
            if(CODING == tempFrame.payload.isncHeader.codedPacket)
            {
                Log.codingCntAtED++;
                if(deviceMacAddr == tempFrame.destAddr)
                {
                    Log.forMeRx++;
                }
                else
                {
                    Log.overHearingRx++;
                }
                //DO DECODING HERE!
            }
            else if(NO_CODING == tempFrame.payload.isncHeader.codedPacket)
            {
                Log.fwdCntAtED++;
            }
        }
        ret = GLOB_SUCCESS;        
    }
    return ret;
}

/*==============================================================================
** Function...: app_outgoing
** Return.....: GLOB_RET
** Description: private function that handles all outgoing packets
** Created....: 01.05.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/

GLOB_RET app_outgoing(void)
{
    frame tempFrame;
    GLOB_RET ret = GLOB_FAILURE;
    unsigned long readTime=0;

    // End Devices & the data rate sweep test is enabled (from the user via USB)
    if(RELAY != deviceMacAddr && Sme.tc.cDRSweepE == YES)
    {
        if (!buffer_empty(&outBufPar))
        {
            // Read from outgoing buffer
            read_buffer(&tempFrame, &outBufPar,1, &readTime);

            // push it down to network layer interface
            ret = network_interface(OUTGOING, &tempFrame);
         // if got ACK then save the tempFrame in a new buffer her! for decoding
        }
    }
    return ret;
}

/*==============================================================================
** Function...: dataRateSweep
** Return.....: GLOB_RET
** Description: function that generates packets for output. This function is
 * called from time interrupt accoring to the user sat data rate.
** Created....: 01.05.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
void dataRateSweep(void)
{
    frame tempFrame;
    frameInit(&tempFrame);

    if(0x60 >= deviceMacAddr )
    {
        Log.totalApTx++;
        if(Sme.tc.cDRSweepE == YES)
        {
            tempFrame.seqNr = Alme.aSeq;
            tempFrame.type = PAY;
            tempFrame.payload.isncHeader.rxTotal = Sme.tc.cReport.cIncPck;
            tempFrame.payload.isncHeader.txReq   = Sme.tc.cReport.cTxReqest;
            Alme.aSeq++;
            if(127<Alme.aSeq)
            {
                Alme.aSeq = 0;
            }
        }
        if (! buffer_full(&outBufPar))
        write_buffer(&tempFrame, &outBufPar, 0);
    }
}

/*==============================================================================
** Function...: frameInit
** Return.....: void
** Description: initialize the frame
** Created....: 01.05.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
void frameInit(frame *tempFrame)
{
    int i;

    tempFrame->destAddr = 0x00;
    tempFrame->srcAddr  = 0x00;
    tempFrame->seqNr    = 0x00;
    tempFrame->type     = 0x00;
    tempFrame->payload.isncHeader.secondDestAddr = 0x00;
    tempFrame->payload.isncHeader.originalSrcAddr  = 0x00;
    tempFrame->payload.isncHeader.firstSeqNr     = 0x00;
    tempFrame->payload.isncHeader.secondSeqNr    = 0x00;
    tempFrame->payload.isncHeader.codedPacket    = 0x00;
    tempFrame->payload.isncHeader.optimalCWmin_A      = 0x00;
    tempFrame->payload.isncHeader.optimalCWmin_B      = 0x00;

    tempFrame->payload.isncHeader.testCaseType   = DEFAULT_TEST;

    tempFrame->payload.isncHeader.rxTotal = 0x00;
    tempFrame->payload.isncHeader.txReq   = 0x00;

    for(i=0;i<sizeof(tempFrame->payload.reserved);i++)
    {
        tempFrame->payload.reserved[i] = deviceMacAddr;
    }
}

/*==============================================================================
** Function...: constructData
** Return.....: int
** Description: 
** Created....: 01.05.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
int constructData(frame *f, unsigned long readTime, char target)
{
    if (readTime <=0) return 0;
    else if (readTime > 16000)
    {
        f->payload.d.chA = 127;
        f->payload.d.chB = 127;
        return 0;
    }
    
    char chSize = 127;
    char i = 0;
    double j = 0.0;
    double chSizeD = 127.00;

    i = readTime/chSize;
    j = readTime/chSizeD;

    j=j-i;
    j=chSize*j;

    if(f->destAddr == 0x01)
    {
        f->payload.d.chA = (char)i;
        f->payload.d.chB = (char)j;
    }
    else if(f->destAddr == 0x02)
    {
        f->payload.d.chC = (char)i;
        f->payload.d.chD = (char)j;
    }
}

/*==============================================================================
** Function...: confControl
** Return.....: GLOB_RET
** Description:
** Created....: 01.05.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
GLOB_RET confControl(void)
{
    GLOB_RET ret = APP_CONF_NOT_SET;
    if(Alme.aID) ret = APP_CONF_SUCCESS;  
}

GLOB_RET copyPacketIBtoKB( void )
{
    frame inbufFrame;
    GLOB_RET ret = GLOB_FAILURE;
    unsigned long readTime = 0;

    if(YES == Nclme.codingEnabled)
    {
        if(! buffer_empty(&inBufPar)) //CHANGE! 24-07-13
        {
            if(RELAY == deviceMacAddr)
            {
                // Relay read out one packet from inbuffer

                read_buffer(&inbufFrame, &inBufPar, 1, &readTime);

                // For statistic
                if(0x01 == inbufFrame.srcAddr)
                {
                    Log.aRcvCnt_app++;
                    Log.aRxTot = inbufFrame.payload.isncHeader.rxTotal;
                    Log.aTxReq = inbufFrame.payload.isncHeader.txReq;
                }
                if(0x02 == inbufFrame.srcAddr)
                {
                    Log.bRcvCnt_app++;
                    Log.bRxTot = inbufFrame.payload.isncHeader.rxTotal;
                    Log.bTxReq = inbufFrame.payload.isncHeader.txReq;
                }
                inbufFrame.payload.isncHeader.testCaseType = Sme.tc.cType;

                // The packet is passed down to the network layer
                ret = network_layer(inbufFrame, readTime);
            }
        }
    }
}
