#include "../commonVar.h"
#include "../commonDef.h"



void printStats(void)
{
    char s[96];
    
    GLOB_RET ret;
    
    if( RELAY == deviceMacAddr) 
    {
        
        sprintf(s,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
                Log.aRcvCnt_app,  Log.bRcvCnt_app, // 1 2
                Log.aRcvCnt_mac, Log.aRcvCnt_phy,     // 3 4
                Log.totalPhyTx, Log.ackRcv_cnt ,      // 5 6
                Log.codingCnt,   Log.fwdCnt,      // 7 8
                Log.codingCntA,  Log.codingCntB,  // 9 10
                Log.fwdCntA,     Log.fwdCntB,     // 11 12
                Log.fwdMacDropA, Log.fwdMacDropB, // 13 14
                Log.inKeepBufCntA,  Log.inKeepBufCntB, // 15 16
                Log.nodeCWminUpdate_A,  Log.nodeCWminUpdate_B,  // 17 18
                Log.inBufDropA,  Log.inBufDropB,  // 19 20
                Log.aHTdrop,     Log.bHTdrop, // 21 22
                Log.keepBufDropA, Log.keepBufDropB,    // 23 24
                Log.inBuffDrop                    // 25
                );
        output_serial(s, sizeof(s));

         #if defined (SIMULATION)
                        printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
                            Log.aRcvCnt_app,  Log.bRcvCnt_app, // 1 2
                            Log.nodeCWminUpdate_A, cw_min ,      // 3 4
                            Log.nodeCWminUpdate_A,      Log.nodeCWminUpdate_B ,      // 5 6
                            Log.codingCnt,   Log.fwdCnt,      // 7 8
                            Log.codingCntA,  Log.codingCntB,  // 9 10
                            Log.fwdCntA,     Log.fwdCntB,     // 11 12
                            Log.fwdMacDropA, Log.fwdMacDropB, // 13 14
                            Log.inKeepBufCntA,  Log.inKeepBufCntB, // 15 16
                            Log.nodeCWminUpdate_A,  Log.nodeCWminUpdate_B,  // 17 18
                            cw_min,  Log.inBufDropB,  // 19 20
                            Log.aHTdrop,     Log.bHTdrop, // 21 22
                            Log.keepBufDropA, Log.keepBufDropB,    // 23 24
                            Log.inBuffDrop                    // 25
                            );
                   #endif
    }
    else
    {
        if(Log.codingCntAtED != 0 || Log.fwdCntAtED != 0)
        {
            Log.avrCWmin = Log.avrCWmin / (Log.codingCntAtED + Log.fwdCntAtED);
        }
        sprintf(s,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
        Log.codingCntAtED + Log.fwdCntAtED,         // 1
        Log.codingCntAtED,Log.fwdCntAtED,           // 2 3
        Log.forMeRx,Log.overHearingRx,      // 4 5
        Log.totalApTx, Log.totalTx,         // 6 7
        Log.retransCnt, Log.totalRx,      // 8 9
        Log.ackRcv_cnt , Log.dummyStats,    // 10 11
        Log.dummyStats, Log.dummyStats, // 12 13
        Log.dummyStats, Log.dummyStats, // 14 15
        Log.dummyStats, Log.dummyStats, // 16 17
        Log.dummyStats, Log.dummyStats, // 18 19
        Log.dummyStats, Log.dummyStats,    // 20 21
        Log.totalPhyTx,Log.asymDrop,   // 22 23
        Log.avrCWmin, Log.inBuffDrop  // 24 25
        );

       

        output_serial(s, sizeof(s));
    }

    Log.totalPhyTx = 0;
    Log.codingCntAtED = 0;
    Log.fwdCntAtED = 0;
    Log.ackRcv_cnt = 0;
    Log.aRcvCnt_app = 0;
    Log.bRcvCnt_app = 0;
    Log.aRcvCnt_mac    = 0;
    Log.bRcvCnt_mac    = 0;
    Log.aRcvCnt_phy    = 0;
    Log.bRcvCnt_phy    = 0;
    Log.inKeepBufCntA = 0;
    Log.inKeepBufCntB = 0;
    Log.aHTdrop    = 0;
    Log.bHTdrop    = 0;

    Log.avrCWmin_R2A = 0;
    Log.avrCWmin_R2B = 0;
    Log.avrCWmin = 0;
    Log.codingCnt = 0;
    Log.fwdCnt = 0;
    Log.fwdCntA = 0;
    Log.fwdCntB = 0;
    Log.fwdMacDropA = 0;
    Log.fwdMacDropB = 0;
    Log.fwdMacDropC = 0;
    Log.fwdMacDropD = 0;
    Log.ncMacDropA = 0;
    Log.ncMacDropB = 0;
    Log.ncMacDropC = 0;
    Log.ncMacDropD = 0;

    Log.codingCntA = 0;
    Log.codingCntB = 0;
    Log.codingCntC = 0;
    Log.codingCntD = 0;
    Log.keepBufDropA = 0;
    Log.keepBufDropB = 0;
    Log.inBufDropA = 0;
    Log.inBufDropB = 0;
    Log.bufDropC = 0;
    Log.bufDropD = 0;
    Log.inBuffDrop = 0;
    Log.forMeRx = 0;
    Log.overHearingRx = 0;
    Log.cRcvCnt_app = 0;
    Log.dRcvCnt_app = 0;
    Log.totalRx = 0;
    Log.servRate = 0;
    Log.retransCnt = 0;
    Log.successSentPkt = 0;
    Log.eRcvCnt = 0;
    Log.totalTx = 0;
    Log.totalApTx = 0;
    Log.asymDrop = 0;

    Log.inBufferHandlerRuns = 0;
    Log.outBufferHandlerRuns = 0;
    Log.whileRuns = 0;

}
