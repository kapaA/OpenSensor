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
        Version:   v1.0
        Date:      2013-04-16
    ============================================================================
*/

#include "packet_simulation.h"

int pctCnt = 0;
void testCase_incomming(void);
void testCase_outgoing(void);


void simulation_testCases(char caseType)
{

    if(INCOMING == caseType)
    {
        testCase_incomming();
    }
    else if(OUTGOING == caseType)
    {
        testCase_outgoing();
    }
}



void constructFrame( char tc , frame *f)
{
    switch(tc)
    {
        case 0:
            f->destAddr = RELAY;
            f->srcAddr = 0x01;
            f->seqNr  = pctCnt;
            f->type = PAY;
            f->payload.isncHeader.codedPacket = NO;
            f->payload.isncHeader.secondDestAddr = 0x02;
            break;
        case 1:
            f->destAddr = RELAY;
            f->srcAddr = 0x02;
            f->seqNr  = pctCnt;
            f->type = PAY;
            f->payload.isncHeader.codedPacket = NO;
            f->payload.isncHeader.secondDestAddr = 0x01;
            break;
        case 2:

            break;

        case 3:

            break;

        case 4:

            break;

        case 5:

            break;
            
        default:
            break;
    }
}


void testCase_incomming(void)
{
    GLOB_RET  ret;
    frame f;
    int sendTo = 0;
    pctCnt++;

 /*
 * Testcase: 1
 * Test Description: Node receives a coded packet intended for it self.
 *                   This node is in promiscuous mode
 * expected return code: CODING_DECODING_SUCCESS
 */
    sendTo = ( rand() % 100 ) + 1;
    expType.expectPAY = YES;
    if(sendTo < 50) constructFrame(0, &f);
    else if(sendTo > 50) constructFrame(1, &f);
    ret = receiveFrame(  &f );

    if(ret==GLOB_SUCCESS)
    {
      //  printf("Testcase 1 Success\n");
    }
    else
    {
      //  printf("Testcase 1 Error [%d] \n",ret);
    }


}



void testCase_outgoing(void)
{
    GLOB_RET  ret;
    pctCnt++;
    frame f;
    int i;
   
    /*
 * Testcase: 1
 * Test Description: Node send a packet from the APP layer
 * expected return code:
 */
/*
   for(i=0;i<sizeof(f.payload.data);i++)
    {
        f.payload.data[i] = Alme.aID;
    }
    f.mHeader.seq = pctCnt;
   if (! buffer_full(&outBufMe))
   {
       outBufMe.timeSem = 0x01;
       write_buffer(&f, &outBufMe);
       outBufMe.timeSem = 0x00;
   }
    ret = dataRateSweep_app_interface( OUTGOING);
    if(ret==GLOB_SUCCESS)
    {
        printf("Testcase 3 Success\n");
    }
    else
    {
        printf("Testcase 3 Error [%d] \n",ret);
    }*/
/****** TESTCASE 3 END ******/
    
}
