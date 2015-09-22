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
        File name: datalink.c
        Version:   v0.0
        Date:      2012-08-08
    ============================================================================
*/

/*============================================================================*/
/*                           INCLUDE STATEMENTS
==============================================================================*/

#include "datalink.h"

/*============================================================================*/
/*                           LOCAL DEFINES
==============================================================================*/

/**
 * Enumeration containing the commands for channel access
*/
typedef enum
{
    WAIT_DIFS = 0,
    PERFORM_BACKOFF = 1,
    SEND_RTS = 2,
    WAIT_FOR_CTS = 3,
    SEND_PAY = 4,
    WAIT_FOR_ACK = 5
}CSMA_CMD;

/*
** ==========================================================================
**                       Layer specific variables
** ==========================================================================
*/

char gotCTS = 0;


expectedTypes expType;
char rxClockOutSize = 0;
int reTry = 0;

/*==============================================================================
** Function...: channelAccess
** Return.....: GLOB_RET
** Description: dataLink
** Created....: 24.08.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
GLOB_RET channelAccess(frame tempFrame, unsigned char retCode)
{
    GLOB_RET ret = GLOB_FAILURE;

    switch (retCode)
    {
        //Wait DIFS period, if there is CD then freez the coutner
        case WAIT_DIFS:
            START_DIFS_TIMER;
            ret = DATA_LINK_DIFS_WAIT_OK;
            while(difsTimer<DIFS)
            {
                if(CD)
                {
                    while(CD)
                    {
                        //copyPacketIBtoKB();
                    } //
                    START_DIFS_TIMER;
                }
            }
            break;

        case PERFORM_BACKOFF:
            if(backOff())
            {
                //if CD then return:
                return DATA_LINK_CARRIER_DETECTED;
            }
            else
            {
                 //else no CD then do the transmit 
                ret = DATA_LINK_BACKOFF_OK;
            }
            break;

        case SEND_PAY:
            // if it is NC we set the NC flag
            if(tempFrame.payload.isncHeader.codedPacket == CODING)
            {
                tempFrame.type = NC_PAY;    
            }
            // else we set the normal flag
            else
            {
                tempFrame.type = PAY;
            }
            // signal the Driver to clock in and tx the payload
            tempFrame.payload.isncHeader.optimalCWmin_A = Log.nodeCWminUpdate_A;
            tempFrame.payload.isncHeader.optimalCWmin_B = Log.nodeCWminUpdate_B;

            Log.avrCWmin_R2A = Log.avrCWmin_R2A + Log.nodeCWminUpdate_A;
            Log.avrCWmin_R2B = Log.avrCWmin_R2B + Log.nodeCWminUpdate_B;

            ret = transmitFrame(tempFrame, PACKET_SIZE);
            
            break;

        case WAIT_FOR_ACK:
            START_ACK_TIMER;
            //Block untill ACK is received or timeout
            while( ackTimer < ACK_TIMEOUT && gotACK == NO );
            // ACK received
            if(gotACK)
            {
                ret = DATA_LINK_GOT_ACK;
                 Log.ackRcv_cnt++;
            }
            else
            {
                ret = DATA_LINK_ACK_TIMEOUT;
            }
            break;

        default:
            ret = GLOB_ERROR_INVALID_PARAM;
            break;
    }
    return ret;
}

GLOB_RET prtocolNoRTSCTS(frame tempFrame)
{
    GLOB_RET ret = GLOB_FAILURE;
    // Wait DIFS

    #if defined (SIMULATION)
        return 0;
    #endif

    ret = channelAccess(tempFrame, WAIT_DIFS);
    if(DATA_LINK_DIFS_WAIT_OK == ret)
    {
        // Select a new backoff window if the backofftimer is == 0
        if(backoffTimer == 0) selectBackoff();
        ret = channelAccess(tempFrame, PERFORM_BACKOFF);
        if(DATA_LINK_BACKOFF_OK==ret)
        {
            // send the payload
            channelAccess(tempFrame, SEND_PAY);
            // now we have transmitted a PAYload and we dont expect a payload
            expType.expectPAY = NO;
            // we  expect an ack
            expType.expectACK = YES;
            // reconf. the nrf driver to recieve an ACK packet (size of header)
            setNRFpw(RX, HEADER_SIZE);
            // wait for ACK
            ret = channelAccess(tempFrame, WAIT_FOR_ACK);

            // we got ack
            if(DATA_LINK_GOT_ACK == ret)
            {
                LED_GREEN_1_TOGGLE;
                cw = MIN_CW; 
                gotACK = NO;
                ret = GLOB_SUCCESS;
                Log.successSentPkt++;
            }
            //no ack/ack timed out
            else
            {
                // increase the contention window size
                LED_YELLOW_1_TOGGLE;
                selectCW();
                ret = DATA_LINK_ACK_TIMEOUT;
            }
        }
        else
        {
            ret = DATA_LINK_CARRIER_DETECTED;
        }
    }
    
    expType.expectPAY = YES;
    expType.expectACK = NO;
    setNRFpw(RX, PACKET_SIZE);
    
    return ret;
}

/*==============================================================================
** Function...: sendPacket
** Return.....: GLOB_RET
** Description: session
** Created....: 09.08.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
GLOB_RET sendPacket(frame tempFrame)
{
    GLOB_RET ret;
    int i = 0; // MAC retrans. counter
    // Validate the packet before sending
    ret = packetController(&tempFrame);

    if(GLOB_SUCCESS==ret)
    {
        /* Special TEST-CASE
        * Relay channel access prio
        */
        if(CODING == tempFrame.payload.isncHeader.codedPacket)
        {
            Log.servRate = Log.servRate + 2;
        }
        else if(NO_CODING == tempFrame.payload.isncHeader.codedPacket)
        {
           Log.servRate++;
        }
        /***********************************************************************/
       
        while(i<macDropRate) //re-Send the packet i'th times before dropping it
        {
            ret = prtocolNoRTSCTS(tempFrame); // Simple CSMA/CA MAC
            // If Carrier detect wait untill !CD
            if(DATA_LINK_CARRIER_DETECTED == ret) while(CD);
            // if ret = no ack then we do retransmission
            else if(DATA_LINK_ACK_TIMEOUT == ret)
            {
                i++; // MAC retrans. counter
                Log.retransCnt++;
            }
            // if ret = success, we reset the contention window cw to initial val.
            else if(GLOB_SUCCESS == ret)
            {
                cw=cw_min; // Setting contention window to initial val.
                if(PAY == tempFrame.type)
                {
                    Log.fwdCnt++;
                    if( 0x01 == tempFrame.destAddr) Log.fwdCntA++;
                    else if( 0x02 == tempFrame.destAddr) Log.fwdCntB++;
                }
                else if(NC_PAY == tempFrame.type)
                {
                    Log.codingCnt++;
                    Log.codingCntA++;
                    Log.codingCntB++;
                    if( 0x01 == tempFrame.destAddr) Log.ncUnicastTargetA++;
                    else if( 0x02 == tempFrame.destAddr) Log.ncUnicastTargetB++;
                }


                return GLOB_SUCCESS; // return to the caller with sucess
            }
        }
       // Logging parameters
        if(NO_CODING == tempFrame.payload.isncHeader.codedPacket)
        {
            if(0x01 == tempFrame.destAddr) Log.fwdMacDropA++;
            else if(0x02 == tempFrame.destAddr) Log.fwdMacDropB++;
            else if(0x03 == tempFrame.destAddr) Log.fwdMacDropC++;
            else if(0x04 == tempFrame.destAddr) Log.fwdMacDropD++;
        }
        else if(CODING == tempFrame.payload.isncHeader.codedPacket)
        {
           if(0x01 == tempFrame.destAddr) Log.ncMacDropA++;
           else if(0x02 == tempFrame.destAddr) Log.ncMacDropB++;
           else if(0x03 == tempFrame.destAddr) Log.ncMacDropC++;
           else if(0x04 == tempFrame.destAddr) Log.ncMacDropD++;
        }
        // The tranmission is not a success we reset the contention window
        cw=cw_min;
   }
   return ret;
}

GLOB_RET packetController(frame *tempFrame)
{
    unsigned char src  = 0x00,
                  dest = 0x00,
                  type = 0x00,
                  code = 0x00;
    
    GLOB_RET ret = GLOB_FAILURE;
    
    src  = tempFrame->srcAddr;
    dest = tempFrame->destAddr;
    type = tempFrame->type;
    code = tempFrame->payload.isncHeader.codedPacket;

 
    if(0x00 >= src  || 0x7E <= src) ret  = GLOB_ERROR_INVALID_PARAM;
    else if(0x00 >= dest || 0x7E <= src) ret  = GLOB_ERROR_INVALID_PARAM;
    else if(0x00 >= type || 6 <= type)   ret  = GLOB_ERROR_INVALID_PARAM;
    //else if(2 < code) ret  = GLOB_ERROR_INVALID_PARAM;
    else
    {
        ret = GLOB_SUCCESS;
    }

    return ret;
}

/*==============================================================================
** Function...: rxFrameHandler
** Return.....: void
** Description: handles the incomming frame
** Created....: 09.08.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
void rxFrameHandler(frame *tempFrame)
{

    GLOB_RET ret = GLOB_FAILURE;
    ret=packetController(tempFrame);
    
    if(ret==GLOB_SUCCESS)
    {
        switch(tempFrame->type)
        {
            case NC_PAY:
                if(expType.expectPAY)
                {
                    network_interface(INCOMING, tempFrame);
                }
                else
                {
                    #if defined (DEBUG)
                        //putsUSBUSART("I Dont expect Payload\n");
                    #endif
                }
                break;

            case PAY:
                if(expType.expectPAY)
                {
                   network_interface(INCOMING, tempFrame);
                }
                else
                {
                    #if defined (DEBUG)
                        putsUSBUSART("I Dont expect Payload\n");
                    #endif
                }
                break;

            case ACK:
                if(expType.expectACK)
                {
                    gotACK = YES;
                }
                break;

            

            default:
                break;
        }
    }
    else
    {
        #if defined (DEBUG)
            putsUSBUSART("Invalid Rx-Frame parameters\n");
        #endif
    }
}
/*==============================================================================
** Function...: rxFrameHandler
** Return.....: void
** Description: handles the incomming frame
** Created....: 09.08.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
