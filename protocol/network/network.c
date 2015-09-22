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
        File name: network.c
        Version:   v0.0
        Date:      2012-08-08
    ============================================================================
*/

/*============================================================================*/
/*                           INCLUDE STATEMENTS
==============================================================================*/

#include "network.h"
void endDeviceIncommingTEMP(frame *f);

/*==============================================================================
** Function...: network_layer
** Return.....: GLOB_RET
** Description: network layer interface. All call must go thoug this interface
** Created....: 04.04.2013 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
GLOB_RET network_interface(char iKey, frame *f)
{
    GLOB_RET ret = GLOB_SUCCESS;

    if(OUTGOING==iKey)
    {
        ret = network_outgoing(f);
    }
    else if (INCOMING == iKey)
    {
        ret = network_incoming(f);
    }
    else
    {
        ret = GLOB_ERROR_INVALID_PARAM;
    }
    return ret;
}

/*==============================================================================
** Function...: network_outgoing
** Return.....: GLOB_RET
** Description: private function that handles all outgoing packets
** Created....: 01.05.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
GLOB_RET network_outgoing( frame *f)
{
    GLOB_RET ret = GLOB_SUCCESS;
    frame tempFrame;

    // Add MAC / MESH header
    f->destAddr = findNextHop();
    f->srcAddr  = Alme.aID;
    f->payload.isncHeader.secondDestAddr = Alme.aDest;
    f->payload.isncHeader.testCaseType = Sme.tc.cType;
    
    tempFrame = *f;
    // Push down to next layer (MAC)
    ret = sendPacket(tempFrame);

    return ret;
}

/*==============================================================================
** Function...: network_incoming
** Return.....: GLOB_RET
** Description: private function that handles all incomming packets
** Created....: 01.05.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
GLOB_RET network_incoming( frame *f)
{
    GLOB_RET ret = GLOB_SUCCESS;

    // Put the packet into inbuffer
    if (! buffer_full(&inBufPar))
    {
        Log.totalRx++;
        inBufPar.timeSem = 0x01;
        write_buffer(f,&inBufPar,0);
        inBufPar.timeSem = 0x00;
    }
    else
    {
        // if relay then run buf optimization algo
        //if(RELAY == Alme.aID) ret = kbBufOptimizer(f);
        Log.inBuffDrop++;
        if( 0x01 == f->srcAddr) Log.inBufDropA++; // packet rcv from A droped in IB
        else if( 0x02 == f->srcAddr) Log.inBufDropB++; // packet rcv from B droped in IB
        ret = IN_BUFFER_FULL;
    }
    return ret;
}

/*==============================================================================
** Function...: findNextHop
** Return.....: char
** Description: private function that find the next hop.
** Created....: 01.05.2015 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
char findNextHop(void)
{
/* Fixed Routing Table
 * X = My ID, Y = dest
    	A	B	C	D	E	F	G	H	R
 ------------------------------------------------------------------------------
   A |	A	R	R	R	B	D	A	C	A
   B |	R	B	R	R	B	D	A	C	B
   C |	R	R	C	R	B	D	A	C	C
   D |	R	R	R	D	B	D	A	C	D
   E |	R	E	R	R	E	D	A	C	B
   F |	R	R	R	F	B	F	A	C	D
   G |	G	R	R	R	B	D	G	C	A
   H |	R	R	H	R	B	D	A	H	C
   R |	R	R	R	R	B	D	A	C	R
*/

    int rTable[NETWORK_SIZE][NETWORK_SIZE] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 99, 99, 99, 2, 4, 1, 3, 1 },
    { 0, 99, 99, 99, 99, 2, 4, 1, 3, 2 },
    { 0, 99, 99, 3, 99, 2, 4, 1, 3, 3 },
    { 0, 99, 5, 99, 4, 2, 4, 1, 3, 4 },
    { 0, 99, 99, 99, 99, 5, 4, 1, 3, 2 },
    { 0, 99, 99, 99, 6, 2, 6, 1, 3, 4 },
    { 0, 7, 99, 99, 99, 2, 4, 7, 3, 1 },
    { 0, 99, 99, 8, 99, 2, 4, 1, 8, 3 },
    { 0, 99, 99, 99, 99, 2, 4, 1, 3, 99 },
    };

    int myI = Alme.aID, nH = Alme.aDest;
    int temp;

    temp = rTable[nH][myI];
    return temp;
}

/*==============================================================================
** Function...: network_layer
** Return.....: GLOB_RET
** Description: only used by relay. Here we build the heaer and put the packet
 *              into the keep buffer
** Created....: 09.08.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/

GLOB_RET network_layer(frame tempFrame, unsigned long readTime )
{
    GLOB_RET ret = GLOB_FAILURE;

    if(RELAY == deviceMacAddr)
    {  
       // Adding the MAC/MESH header
       tempFrame.payload.isncHeader.originalSrcAddr = tempFrame.srcAddr;//Packet originally from
       tempFrame.destAddr = tempFrame.payload.isncHeader.secondDestAddr;//Next Dest
       tempFrame.srcAddr  = deviceMacAddr; //Sending with my MAC add
       tempFrame.payload.isncHeader.firstSeqNr = tempFrame.seqNr;
       tempFrame.payload.isncHeader.codedPacket = 0x00;

       tempFrame.payload.isncHeader.testCaseType = Sme.tc.cType;

       // If coding is disabled
       if(NO == Nclme.codingEnabled)
       {
           ret = sendPacket(tempFrame);
       }
       // If coding in enabled
       else if (YES == Nclme.codingEnabled)
       {

            if(!buffer_full(&keepBufPar))
            {
                write_buffer(&tempFrame, &keepBufPar, &readTime);

                ret = GLOB_SUCCESS;
            }
            else
            {
                if(YES == Sme.tc.cKbOP)
                {
                    ret = kbBufOptimizer(&tempFrame, readTime);
                }
                else
                {
                    if(0x01 == tempFrame.payload.isncHeader.originalSrcAddr) //CHANGE! 31-07-13
                    {
                        Log.keepBufDropA++;
                    }
                    else if(0x02 == tempFrame.payload.isncHeader.originalSrcAddr)
                    {
                        Log.keepBufDropB++;
                    }
                }
            }
       }
    }
    else
    {
        ret = GLOB_ERROR_OUT_OF_RANGE_PARAM;
    }
    return ret;
}
