/*  ============================================================================
    Copyright (C) 2012 - 2014 Mobile Devices AAU.
    All rights reserved...sounds awsome
    ============================================================================
    This document contains proprietary information belonging to Achuthan.
    Passing on and copying of this document, use and
    communication of its contents is not permitted without prior written
    authorisation.
    ============================================================================
    Revision Information:
        File name: commonDef.h
        Version:   v0.0
        Date:      2012-08-09
    ============================================================================
*/

/*
** =============================================================================
**                                   INCLUDE STATEMENTS
** =============================================================================
*/
#include <stdint.h>          /* For uint32_t definition */
#include <stdio.h>
#include <stdlib.h>
#include <p24Fxxxx.h>
#include <math.h>
/*
** =============================================================================
**                                   DEFINES
** =============================================================================
*/

#define NETWORK_SIZE 10

/**
* Protocol control specific Defines
*/
#define RELAY_CP 0
#define END_NODE_CP 1
#define BOTH_CP 2


/**
* Test specific Defines
*/
#define NODE_A 0x01
#define NODE_B 0x02

#define DEFAULT_TEST 0
#define BEST_LINK 1
#define WORST_LINK 2
#define RELAY_PRIO 3
#define HOLDING_TIME 4
#define PC_RELAY_BI_CONTROL_1 5

/**
* Genral Defines
*/

#define CODING 1
#define NO_CODING 0

#define MIN_MAC_ID 1
#define MAX_MAC_ID 127

#define SLOT_TIME 8 
#define MIN_CW 8
#define MAX_CW 64
#define MAC_DROP 3
#define CW_ALPHA 9

#define ALICE 0x01
#define RELAY 0x63
#define SNIFFER 0x64

#define HOLD_TIME 100 // 100 milliseconds ... NO TICKS!!!!! BUT Mil.SEC!!!!

#define DROP_TIME 100

#define INCOMING 0
#define OUTGOING 1

/**
* Comment in while simulating and debugging
*/
//#define SIMULATION
//#define DEBUG

/**
* Logial
*/
#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ACTIVE  1

#define YES     1
#define NO      0


/**
* Led Debug
*/


#define LED_YELLOW_1 LATBbits.LATB0
#define LED_GREEN_1  LATAbits.LATA1

#define LED_YELLOW_1_TOGGLE LED_YELLOW_1 = ~LED_YELLOW_1
#define LED_GREEN_1_TOGGLE LED_GREEN_1   = ~LED_GREEN_1


/**
* clock ticks needed for x-Time
*/
#define ONE_SEC_TICKS 10000

/**
* Start the timer
*/
#define START_CTS_TIMER      ctsTimer           = 0;
#define START_PAY_TIMER      payTimer           = 0;
#define START_ACK_TIMER      ackTimer           = 0;

#define START_ONESEC_TIMER   oneSecTimer        = 0;
#define START_SIFS_TIMER     sifsTimer          = 0;
#define START_DIFS_TIMER     difsTimer          = 0;
#define START_DATARATE_TIMER dataRateTimer      = 0;
#define START_USB_TX_TIMER   cdcTxServiceTimer  = 0;
#define START_CW_ALGO_TIMER  cwAlgoTimer        = 0;

/**
* Timeout defines. Unit is "hundreds of microseconds"
*/
#define SIFS            8  // 800 muSec = 12 symbols
#define DIFS            24 // 2400 muSec = 40 symbols
#define CTS_TIMEOUT     45
#define PAYLOAD_TIMEOUT 84
#define ACK_TIMEOUT     45

/**
 * nRF payload buffer types
*/
#define RX 0
#define TX 1

/**
 * frame size
*/
#define PACKET_SIZE      32
#define PAYLOAD_SIZE     28
#define HEADER_SIZE       4
#define NRF_BUFFER_SIZE  10
#define REDUCED_PAYLOAD_SIZE 20

/**
 * frame type
*/
#define RTS    1
#define CTS    2
#define PAY    3
#define ACK    4
#define NC_PAY 5




/*
** =============================================================================
**                   GLOBAL EXPORTED FUNCTION DECLARATIONS
** =============================================================================
*/

void printStats(void);
int buffer_full( bufPar *bPar); // make it const pointer
int buffer_empty( bufPar *bPar); // make it const pointer
void write_buffer( frame *f, bufPar *bpar, unsigned long *readTime); // make frame as const pointer
void read_buffer(frame *f, bufPar *bPar, unsigned char opt, unsigned long *readTime);
GLOB_RET remove_frame(int rmPointer, bufPar *bPar);
GLOB_RET get_frame(frame *f, unsigned long *time ,int rPointer, bufPar *bPar);
GLOB_RET read_time(unsigned long *time ,int rPointer, bufPar *bPar);
GLOB_RET copyPacketIBtoKB( void );
/**
 * Description:
 * @param:
 * @return:
*/
int backOff(void);

/**
 * Description:
 * @param:
 * @return:
*/
void selectBackoff(void);

/**
 * Description:
 * @param:
 * @return:
*/
void selectCW(void);

/**
 * Description:
 * @param:
 * @return:
*/
void dataRateSweep(void);

void nrfReset(void);
int constructData(frame *f, unsigned long readTime, char target);
GLOB_RET  protocolController( void );
void relayCP(void);

GLOB_RET network_interface(char iKey, frame *f);




