/*  =========================================================================
    Copyright (C) 2012 - 2014 Mobile Devices AAU.
    All rights reserved...sounds awsome
    =========================================================================
    This document contains proprietary information belonging to Achuthan.
    Passing on and copying of this document, use and
    communication of its contents is not permitted without prior written
    authorisation.
    =========================================================================
    Revision Information:
        File name: commonVar.h
        Version:   v0.0
        Date:      2012-08-09
    =========================================================================
*/
#define BUFFER_SIZE 40 //CHANGE! 26-07-13
#define CW_MEM_SIZE 2

/**
 * Enumeration containing different return types
*/
typedef enum
{
  KEEP_BUFFER_IGNORE            = 23,
  KEEP_BUFFER_FULL              = 22,
  IN_BUFFER_IGNORE              = 21,
  IN_BUFFER_EMPTY               = 20,
  IN_BUFFER_FULL                = 19,
  OUT_BUFFER_EMPTY              = 18,
  OUT_BUFFER_FULL               = 17,

  APP_CONF_NOT_SET              = 16,
  APP_CONF_SUCCESS              = 15,
  APP_CONF_FAILURE              = 14,

  CODING_FAILURE                = 13,
  CODING_BELOW_HOLDING          = 12,
  CODING_FWD                    = 11,
  CODING_SUCCESS                = 10,
  CODING_BUFFER_FULL            = 9,
  CODING_BUFFER_EMPTY           = 8,
  DATA_LINK_ACK_TIMEOUT         = 7, /* Payload sent but did not receive ack */
  DATA_LINK_GOT_ACK             = 6, /* Payload sent and received ack */
  DATA_LINK_CTS_TIMEOUT         = 5, /* RTS sent but did not receive cts*/
  DATA_LINK_GOT_CTS             = 4, /* RTS sent and received cts*/
  DATA_LINK_DIFS_WAIT_OK        = 3, /* Waited DIFS without any CD*/
  DATA_LINK_BACKOFF_OK          = 2, /* Did backoff without CD*/
  DATA_LINK_CARRIER_DETECTED    = 1, /* Carrier detected */

  GLOB_SUCCESS                  = 0,  /* The prosses was prossed with success */
  GLOB_FAILURE                  = -1, /* The prosses was prossed with some or more error */
  GLOB_ERROR_OUT_OF_RANGE_PARAM = -2, /* The parameter passed to the function is outside the valid range */
  GLOB_ERROR_INVALID_MESSAGE    = -3, /* The given message does not identify a valid object */
  GLOB_ERROR_INVALID_PARAM      = -4, /* The parameter passed to the function is invalid*/
  GLOB_ERROR_OUT_OF_HANDLES     = -5, /* There is no free handle available*/
  GLOB_ERROR_INIT               = -6, /* Initialisation when wrong */
  GLOB_WARNING_PARAM_NOT_SET    = -20, /* There requiret parameter is not set */
}GLOB_RET;

typedef struct{
unsigned char secondDestAddr; // 1 byte  /* Over hearing node addres */
unsigned char originalSrcAddr;// 1 byte  /* Original src addres */
unsigned char firstSeqNr;     // 1 byte
unsigned char secondSeqNr;    // 1 byte
unsigned char codedPacket;    // 1 byte
unsigned char txReq;          // 1 byte
unsigned char rxTotal;        // 1 byte
char optimalCWmin_A;    // 1 byte
char optimalCWmin_B;    // 1 byte
unsigned char testCaseType;   // 1 byte
}isnc; //Network coding header

typedef struct{
    char chA;
    char chB;
    char chC;
    char chD;
} data;

/**
 * Typedef struct containing the payload structure
 * Size: 28 bytes
*/
typedef struct{
  isnc isncHeader;   // 10 bytes
  data d;            // 4 bytes
  char reserved[14]; // 12 bytes
} pload; //Payload split-up


/**
 * Typedef struct containing the frame structure
 * Size: One frame is 32 bytes
*/
typedef struct
{
  unsigned int  type     :8; // 1 byte
  unsigned char srcAddr;     // 1 byte
  unsigned char destAddr;    // 1 byte
  unsigned char seqNr;       // 1 byte
  pload payload;             // 28 bytes
} frame;

/**
 * Typedef struct containing the init confirm parameters
*/
typedef struct
{
   int  spi    :1;
   int  nrf    :1;
   int  timer  :1;
   int  usb    :1;
} initModul;

/**
 * Typedef struct containing the expected frame used in data link layer
*/
typedef struct{
   int expectPAY :1;
   int expectACK :1;
} expectedTypes;

typedef struct{
    unsigned char cIncPck;
    unsigned char cTxReqest;
}report;

typedef struct{
   char cType;
   char cAsymDropNode;
   char cLinkTarget;
   char cLinkOh;
   char cDataRate;
   char cDRSweepE;
   char cKbOP;
   int  cTicksTx;
   int cCpAlgo;
   unsigned char cCWminUpdater;
   unsigned char cCWmaxUpdater;
   report cReport;

}testCase;





/**
 * System level management entity
*/
typedef struct{
   testCase tc;
}sme;

/**
 * APP layer management entity
*/
typedef struct{
    char aID;
    char aDest;
    unsigned char aSeq;
    unsigned char drCapaTest;
}alme;

/**
 * coding layer management entity
*/
typedef struct{
    unsigned char codingEnabled;
    unsigned int holdingTime;
    unsigned int dropTime;
    unsigned char medianCWmin;
    unsigned char algoMinCWmin;
    unsigned char algoMaxCWmin;
}nclme;

// For statistic
typedef struct{

    unsigned char codingCntAtED;
    unsigned char fwdCntAtED;

    unsigned char ncUnicastTargetA;
    unsigned char ncUnicastTargetB;
    
    unsigned char inKeepBufCntA;
    unsigned char inKeepBufCntB;

    unsigned char codingCnt;  // incomming/outgoing coded packets
    unsigned char codingCntA; // outgoing coded packet to A from R
    unsigned char codingCntB; // outgoing coded packet to B from R
    unsigned char codingCntC; // outgoing coded packet to C from R
    unsigned char codingCntD; // outgoing coded packet to D from R
    unsigned char fwdCnt;     // outgoing non-coded packet from R
    unsigned char fwdCntA;    // outgoing non-coded packet to A from R
    unsigned char fwdCntB;    // outgoing non-coded packet to B from R

    unsigned char aRcvCnt_app;    // incomming packet at APP Relay from A
    unsigned char bRcvCnt_app;    // incomming packet at APP Relay from B
    unsigned char cRcvCnt_app;    // incomming packet at APP Relay from C
    unsigned char dRcvCnt_app;    // incomming packet at APP Relay from D

    unsigned char aTxReq;     // Num. packet A would like to send to B
    unsigned char bTxReq;     // Num. packet B would like to send to A
    unsigned char aRxTot;     // Total num. packet rx by A
    unsigned char bRxTot;     // Total num. packet rx by B

    unsigned char aRcvCnt_mac;
    unsigned char bRcvCnt_mac;
    unsigned char aRcvCnt_phy;
    unsigned char bRcvCnt_phy;

    unsigned char ackRcv_cnt;

    unsigned char aHTdrop;
    unsigned char bHTdrop;

    unsigned char eRcvCnt;
    unsigned char totalTx;    // Total Transmission incl. retrans
    unsigned char totalRx;    // Total incomming packets
    unsigned char fwdMacDropA; // fwd packet drop due to ack timeout at R to A
    unsigned char fwdMacDropB; // fwd packet drop due to ack timeout at R to B
    unsigned char fwdMacDropC; // fwd packet drop due to ack timeout at R to C
    unsigned char fwdMacDropD; // fwd packet drop due to ack timeout at R to D
    unsigned char ncMacDropA; // nc packet drop due to ack timeout at R to A
    unsigned char ncMacDropB; // nc packet drop due to ack timeout at R to B
    unsigned char ncMacDropC; // nc packet drop due to ack timeout at R to C
    unsigned char ncMacDropD; // nc packet drop due to ack timeout at R to D
    unsigned char keepBufDropA;
    unsigned char keepBufDropB;
    unsigned char inBufDropA;
    unsigned char inBufDropB;
    unsigned char bufDropC;
    unsigned char bufDropD;
    unsigned char inBuffDrop;
    unsigned char overHearingRx;
    unsigned char forMeRx;
    unsigned char servRate;
    unsigned char retransCnt;
    unsigned char successSentPkt;
    unsigned char totalApTx;
    unsigned char totalPhyTx;
    unsigned char asymDrop;
    char nodeCWminUpdate_A;
    char nodeCWminUpdate_B;
    char nodeCWmaxUpdate_A;
    char nodeCWmaxUpdate_B;
    unsigned int whileRuns;
    unsigned int inBufferHandlerRuns;
    unsigned int outBufferHandlerRuns;

    unsigned int avrCWmin;

    unsigned int avrCWmin_R2A;
    unsigned int avrCWmin_R2B;

    unsigned char dummyStats;
}stats;



typedef struct{
   unsigned int  tx_en    :1;
   unsigned int  trx_ce   :1;
   unsigned int  pwr_up :1;
   unsigned int csn :1;

   unsigned int cd :1;
   unsigned int am :1;
   unsigned int dr :1;
}nrfbits;

typedef struct
{
    unsigned volatile int read_pointer;
    unsigned volatile int write_pointer;
    unsigned volatile int data_size;
    frame buffer[BUFFER_SIZE];
    unsigned long timerBuffer[BUFFER_SIZE];
    unsigned char timeSem;
}bufPar;

/*
** ==========================================================================
**                       Extern Global variables
** ==========================================================================
*/

extern stats Log;

/**
 * Global management entity
 */
extern sme Sme;
extern alme Alme;
extern nclme Nclme;

/**
 * Buffer
 */
extern bufPar keepBufPar;
extern bufPar outBufPar;
extern bufPar inBufPar;

/**
 * Network
 */
extern unsigned char destMacAddr;
extern unsigned char deviceMacAddr;
extern unsigned char cw_algo;

/**
 * Backof alogo
 */
extern int backoffTimer;
extern unsigned int cw;
extern unsigned int cw_min;
extern unsigned int cw_max;

/**
 * External Timer variables
*/
extern unsigned int oneSecTimer;
//USB timer
extern unsigned int USBtxTimer;
// Inter frame space timers
extern unsigned int dataRateTimer;
extern unsigned int sifsTimer;
extern unsigned int difsTimer;
extern unsigned long longCounter;

// Frame timeout timers
extern unsigned int ackTimer;
extern unsigned int cwAlgoTimer;
extern unsigned int time_tik;
extern unsigned long timer_ms;

/**
 * External nrf config
*/
extern  char rxClockOutSize;

/**
 * External usb variables
 */
extern unsigned int cdcTxServiceTimer;
extern char INbuffer[96];
extern char OUTbuffer[96];
extern volatile unsigned char semph;

extern expectedTypes expType;

extern int in_data_size;
extern int in_read_pointer ;
extern int in_write_pointer;

extern int out_data_size;
extern int out_read_pointer ;
extern int out_write_pointer;

extern int a_data_size;
extern int a_read_pointer;
extern int a_write_pointer;

extern int b_data_size;
extern int b_read_pointer;
extern int b_write_pointer;

extern unsigned int cw_m[CW_MEM_SIZE];

extern unsigned char macDropRate;
extern unsigned char dropRate;

extern unsigned char txLock;

extern char gotPAY;
extern char gotACK;


