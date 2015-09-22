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
        File name: pysical.c
        Version:   v0.0
        Date:      2012-08-08
    ============================================================================
*/

/*============================================================================*/
/*                           INCLUDE STATEMENTS
==============================================================================*/


#include "../../driver/nrf/nrf.h"
#include "physical.h"

char frameDubManager[100]; // FIXME Magic number


/*==============================================================================
** Function...: external interrupt for data ready
** Return.....: void
** Description:
** Created....: 19.07.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
 void __attribute__((__interrupt__, auto_psv)) _INT1Interrupt(void)
{

    IFS1bits.INT1IF = 0;    //Clear the INT1 interrupt flag

    frame tempFrame;

    tempFrame = clockOut(rxClockOutSize);
    if(tempFrame.srcAddr == 0x01) Log.aRcvCnt_phy++;
    else if(tempFrame.srcAddr == 0x02) Log.bRcvCnt_phy++;

    if( DEFAULT_TEST == tempFrame.payload.isncHeader.testCaseType ||
        RELAY == deviceMacAddr   )
    {
        receiveFrame(&tempFrame); 
    }
    else
    {
        if( RELAY != deviceMacAddr)
        {
            receiveFrameLinkTest(&tempFrame); 
        }
    }
    
}

/*==============================================================================
** Function...: transmitFrame
** Return.....: GLOB_RET
** Description: session
** Created....: 09.08.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/

GLOB_RET transmitFrame(frame tempFrame, char pw)
{
    GLOB_RET ret = GLOB_FAILURE;

    #if defined (SIMULATION)
        return GLOB_SUCCESS;
    #endif

    setNRFpw(TX, pw);
    clockIn(tempFrame, pw); // Clock data in with SPI
    IEC1bits.INT1IE = 0; //Disabling external interrupt before tx
    ret = TXPacket();       // Transmit
    IEC1bits.INT1IE = 1; //Enabling external interrupt after tx

    Log.totalPhyTx++;

    return GLOB_SUCCESS;
}


/*==============================================================================
** Function...: receiveFrame
** Return.....: void
** Description: received a frame from a device, process if it is for me
** Created....: 09.08.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
int receiveFrame(frame *rxFrame)
{
    frame txFrame;
    int r = 0;


    if(rxFrame->destAddr == deviceMacAddr)
    {
        
        /* Special TEST-CASE
         * Asym Drop Test case Incomming packets will be dropped if
         * R is lower than dropRate
         */
        if(deviceMacAddr == RELAY   && rxFrame->srcAddr == Sme.tc.cAsymDropNode||
           deviceMacAddr == Sme.tc.cAsymDropNode && rxFrame->srcAddr == RELAY )
        {
            r = (rand() % 101); // get a random num. between 1-100
            if(r<dropRate) // if true drop the packet
            {
                Log.asymDrop++;
                return 0;
            }
        }
        /**********************************************************************/

        // Incomming packets of type PAY or NC_PAY will be responded with an ACK
        if(rxFrame->type == PAY || rxFrame->type == NC_PAY)
        {
             // Flipping the sender addr to dest add.
            txFrame.destAddr = rxFrame->srcAddr;
            // My own MAC addr
            txFrame.srcAddr = deviceMacAddr;
            // Create a respons(ACK) for the recieved PAYLOAD
            txFrame.type = ACK;
            //START the SIFS timer
            START_SIFS_TIMER;
            #ifndef SIMULATION
                setNRFpw(TX, HEADER_SIZE);
                clockIn(txFrame, HEADER_SIZE);
                while(sifsTimer<SIFS);
                // Transmit the packet. Send to next layer
                TXPacket();
            #endif

            if(rxFrame->seqNr^frameDubManager[rxFrame->srcAddr])
            {
                if(rxFrame->srcAddr == 0x01) Log.aRcvCnt_mac++;
                else if(rxFrame->srcAddr == 0x02) Log.bRcvCnt_mac++;
                rxFrameHandler(rxFrame);
                frameDubManager[rxFrame->srcAddr] = rxFrame->seqNr;
            }
        }
        // If incomming packet is an ACK we put a flag high and return
        else if(expType.expectACK && rxFrame->type == ACK)
        {
            gotACK = YES;
            return 0;
        }
    }
    /* The incomming packet is not for me! but the secoundSrcAddr is my MAC add
     * and the packet is coded and from relay, so this packet is for me, so I
     * will overhear this packet and take it in without sending an ACK
     */
    else if(rxFrame->payload.isncHeader.secondDestAddr == deviceMacAddr && //If the packet is coded.
            rxFrame->type == NC_PAY && rxFrame->srcAddr == RELAY)
    {
      // If the seq. num. is different from the previous packet of the same type
        if(rxFrame->seqNr^frameDubManager[rxFrame->srcAddr])
        {
            rxFrameHandler(rxFrame);
            frameDubManager[rxFrame->srcAddr] = rxFrame->seqNr;
        }
    }
    // Incomming packet is a ghost packet
    else
    {
        return 0;
    }
}


/*==============================================================================
** Function...: receiveFrameLinkTest
** Return.....: void
** Description: received a frame from a device, process if it is for me
** Created....: 09.08.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
int receiveFrameLinkTest(frame *rxFrame)
{
    frame txFrame;
    int r = 0;

    /* Special TEST-CASE
     * Asym Drop Test case Incomming packets will be dropped if
     * R is lower than dropRate
     */

    if(deviceMacAddr == Sme.tc.cAsymDropNode && rxFrame->srcAddr == RELAY &&
       rxFrame->type != ACK )
    {
        r = (rand() % 101); // get a random num. between 1-100
        if(r<dropRate) // if true drop the packet
        {
            Log.asymDrop++;
            return 0;
        }
    }

    if(rxFrame->destAddr == deviceMacAddr)
    {
        
        // Incomming packets of type PAY or NC_PAY will be responded with an ACK
        if(rxFrame->type == PAY || rxFrame->type == NC_PAY)
        {
             // Flipping the sender addr to dest add.
            txFrame.destAddr = rxFrame->srcAddr;
            // My own MAC addr
            txFrame.srcAddr = deviceMacAddr;
            // Create a respons(ACK) for the recieved PAYLOAD
            txFrame.type = ACK;
            //START the SIFS timer
            START_SIFS_TIMER;
            setNRFpw(TX, HEADER_SIZE);
            clockIn(txFrame, HEADER_SIZE);
            #ifndef SIMULATION
                while(sifsTimer<SIFS);
                // Transmit the packet. Send to next layer
                TXPacket();
            #endif

            // The frame is transmitted from this device
            if(rxFrame->type == PAY &&
               rxFrame->payload.isncHeader.originalSrcAddr == deviceMacAddr)
            {
                return 0;
            }

            if(rxFrame->seqNr^frameDubManager[rxFrame->srcAddr])
            {
                //if(rxFrame->srcAddr == 0x01) Log.aRcvCnt_mac++;
                //else if(rxFrame->srcAddr == 0x02) Log.bRcvCnt_mac++;
                
                rxFrameHandler(rxFrame);
                frameDubManager[rxFrame->srcAddr] = rxFrame->seqNr;
                return 0;
            }
        }
        // If incomming packet is an ACK we put a flag high and return
        else if(expType.expectACK && rxFrame->type == ACK)
        {
            gotACK = YES;
            return 0;
        }
    }
    else if(rxFrame->type == NC_PAY &&
            rxFrame->payload.isncHeader.secondDestAddr == deviceMacAddr &&
            rxFrame->srcAddr == RELAY ||

            rxFrame->type == PAY &&
            rxFrame->payload.isncHeader.originalSrcAddr != deviceMacAddr &&
            rxFrame->payload.isncHeader.secondDestAddr == deviceMacAddr &&
            rxFrame->srcAddr == RELAY)
    {
            if(rxFrame->seqNr^frameDubManager[rxFrame->srcAddr])
            {
                rxFrameHandler(rxFrame);
                frameDubManager[rxFrame->srcAddr] = rxFrame->seqNr;
                return 0;
            }
    }   
    return 0;
}
