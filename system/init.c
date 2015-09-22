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
        File name: init.c
        Version:   v0.0
        Date:      2012-07-19
    ==========================================================================*/

/*
** =============================================================================
**                                  INCLUDE STATEMENTS
** =============================================================================
*/

#include "../commonVar.h"
#include "../commonDef.h"

void managementEntityInit(void);

stats Log;


unsigned int cw        = MIN_CW;
unsigned char cw_algo = NO;
int backoffTimer       = 0;
unsigned char dropRate = 0;
char gotPAY = 0;
char gotACK = 0;

bufPar keepBufPar;
bufPar outBufPar;
bufPar inBufPar;

unsigned int cw_m[CW_MEM_SIZE];
unsigned int cw_min;
unsigned int cw_max ;
unsigned char macDropRate;
unsigned char txLock;
unsigned long readBufTime = 0;

sme Sme;
alme Alme;
nclme Nclme;

/*==============================================================================
** Function...: init
** Return.....: GLOB_RET
** Description: Main init function
** Created....: 19.07.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/
GLOB_RET init()
{
    GLOB_RET ret = GLOB_FAILURE;
    AD1PCFGL     = 0xFFFF; // Set to all digital I/O
    initModul initMod;

    macDropRate = MAC_DROP;
    cw_max      = MAX_CW;
    cw_min      = MIN_CW;
    cw          = cw_min;
    dropRate    = 0;
    txLock      = NO;


    inBufPar.timeSem    = 0x00; 

    cw_m[0] = 1;
    cw_m[1] = 1;

    managementEntityInit();

    Log.totalPhyTx = 0;

    Log.codingCntAtED = 0;
    Log.fwdCntAtED = 0;

    Log.ncUnicastTargetA = 0;
    Log.ncUnicastTargetB = 0;

    Log.dummyStats = 0;

    Log.whileRuns = 0;
    Log.inBufferHandlerRuns = 0;
    Log.outBufferHandlerRuns = 0;

    Log.aRcvCnt_app    = 0;
    Log.bRcvCnt_app    = 0;
    Log.aRcvCnt_mac    = 0;
    Log.bRcvCnt_mac    = 0;
    Log.aRcvCnt_phy    = 0;
    Log.bRcvCnt_phy    = 0;

    Log.aHTdrop    = 0;
    Log.bHTdrop    = 0;

    Log.avrCWmin = 0;
    Log.avrCWmin_R2A = 0;
    Log.avrCWmin_R2B = 0;

    Log.keepBufDropA   = 0;
    Log.keepBufDropB   = 0;
    Log.inBufDropA = 0;
    Log.inBufDropB = 0;
    Log.inBuffDrop = 0;
    Log.codingCnt  = 0;
    Log.codingCntA = 0;
    Log.codingCntB = 0;
    Log.dRcvCnt_app    = 0;
    Log.forMeRx    = 0;
    Log.fwdCnt     = 0;
    Log.fwdCntA    = 0;
    Log.fwdCntB    = 0;
    Log.fwdMacDropA = 0;
    Log.fwdMacDropB = 0;
    Log.ncMacDropB = 0;
    Log.overHearingRx = 0;
    Log.totalTx    = 0;
    Log.totalRx    = 0;
    Log.servRate   = 0;
    Log.retransCnt = 0;
    Log.successSentPkt = 0;
    Log.eRcvCnt = 0;
    Log.totalApTx = 0;
    Log.asymDrop = 0;
    Log.ackRcv_cnt = 0;

    Log.nodeCWminUpdate_A = 8;
    Log.nodeCWminUpdate_B = 8;
    Log.nodeCWmaxUpdate_A = 64;
    Log.nodeCWmaxUpdate_B = 64;

    Log.inKeepBufCntA = 0;
    Log.inKeepBufCntB = 0;

    initMod.nrf   = 0;
    initMod.spi   = 0;
    initMod.timer = 0;
    initMod.usb   = 0;

    led();
    
    // MCU Driver initialization
    ret = spi(&initMod);
    if(GLOB_SUCCESS == ret) ret = timer(&initMod);
    if(GLOB_SUCCESS == ret) ret = nrf(&initMod);
    #if defined (SIMULATION)
        return GLOB_SUCCESS;
    #endif
    if(GLOB_SUCCESS == ret) ret = usb(&initMod);
    {
        expType.expectPAY = YES;
        expType.expectACK = NO;

        setNRFpw(RX, PACKET_SIZE);
    }

    return ret;
}

void managementEntityInit(void)
{
    Sme.tc.cType         = DEFAULT_TEST;
    Sme.tc.cAsymDropNode = NODE_A;
    Sme.tc.cLinkTarget   = 0x00;
    Sme.tc.cLinkOh       = 0x00;
    Sme.tc.cCWminUpdater    = 8;
    Sme.tc.cCWmaxUpdater    = 64;
    Sme.tc.cDataRate     = 0;
    Sme.tc.cTicksTx      = 0;
    Sme.tc.cDRSweepE     = NO;
    Sme.tc.cCpAlgo       = NO;
    Sme.tc.cKbOP         = YES;

    Sme.tc.cReport.cIncPck   = 0x00;
    Sme.tc.cReport.cTxReqest = 0x00;

    Alme.aDest = 0x00;
    Alme.aID = 0x00;
    Alme.aSeq = 0x00;
    Alme.drCapaTest = 0x00;

    Nclme.codingEnabled = NO;
    Nclme.holdingTime = HOLD_TIME;
    Nclme.dropTime    = DROP_TIME;

    Nclme.medianCWmin = 14;
    Nclme.algoMinCWmin = 4;
    Nclme.algoMaxCWmin = 24;


    
}

