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
        File name: usbCmd.c
        Version:   v0.0
        Date:      2012-08-08
    ============================================================================
*/

/*============================================================================*/
/*                           INCLUDE STATEMENTS
==============================================================================*/

#include "usbCmd.h"


unsigned char   deviceMacAddr = 0;
unsigned char   destMacAddr = 0;

unsigned char preDR = 0;


/*==============================================================================
** Function...: usbCMD
** Return.....: GLOB_RET
** Description: usb command interpreter
** Created....: 14.08.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/

GLOB_RET usbCMD(char *buffer)
{ 
    int i = 0;
    char s[96];
    char inbuf[6];  //Input buffer

    switch (atoi(buffer))
    {
        
        case SET_DEVICE_MAC_ADDR:
            for(i=0; i<4; i++)
            {
                inbuf[i] = buffer[i+2];
            }
            i = atoi(inbuf);
            config(i, 1);
            sprintf(s,"ID_%d\n",i);
            output_serial(s, sizeof(s));
            break;

        case SET_DEST_MAC_ADDR:
            for(i=0; i<4; i++)
            {
                inbuf[i] = buffer[i+2];
            }
            i = atoi(inbuf);
            config(i, 0);
            sprintf(s,"DEST_%d\n",i);
            output_serial(s, sizeof(s));
            break;
            
        case GET_DEVICE_MAC_ADDR:
            sprintf(s,"COM_RET_MAC-ID_%d\n",deviceMacAddr);
            output_serial(s, sizeof(s));
            break;

        case GET_DEST_MAC_ADDR:
            sprintf(s,"COM_RET_DEST-ID_%d\n",destMacAddr);
            output_serial(s, sizeof(s));
            break;

        case SET_DATA_RATE:
            for(i=0; i<4; i++)
            {
                inbuf[i] = buffer[i+2];
            }
            Sme.tc.cDataRate = atoi(inbuf);
            i = Sme.tc.cDataRate;
            if(i<=0 || i>100 )
            {
                i = preDR+1;
            }
            Sme.tc.cTicksTx = ONE_SEC_TICKS / i;
            preDR = i;
            sprintf(s,"DR_%d\n",i);
            output_serial(s, sizeof(s));
            break;

        case START_DATA_TX:
            Sme.tc.cDRSweepE = YES;
            sprintf(s,"START\n");
            output_serial(s, sizeof(s));
            break;

        case STOP_DATA_TX:
            Sme.tc.cDRSweepE = NO;
            sprintf(s,"STOP\n");
            output_serial(s, sizeof(s));
            inBufPar.data_size = 0;
            inBufPar.read_pointer = 0;
            inBufPar.write_pointer = 0;
            outBufPar.data_size = 0;
            outBufPar.read_pointer = 0;
            outBufPar.write_pointer = 0;
            keepBufPar.data_size = 0;
            keepBufPar.read_pointer = 0;
            keepBufPar.write_pointer = 0;
            break;

        case START_CODING:
            Nclme.codingEnabled = YES;
            sprintf(s,"START_NC\n");
            output_serial(s, sizeof(s));
            break;

        case STOP_CODING:
            Nclme.codingEnabled = NO;
            sprintf(s,"STOP_NC\n");
            output_serial(s, sizeof(s));
            break;

        case CW_ALGO:
            for(i=0; i<4; i++)
            {
                inbuf[i] = buffer[i+2];
            }
            cw_algo = atoi(inbuf);
            sprintf(s,"CW_ALGO_%d\n",cw_algo);
            output_serial(s, sizeof(s));

            break;

        case ASYM_RATE:
            for(i=0; i<4; i++)
            {
                inbuf[i] = buffer[i+2];
            }
            dropRate = atoi(inbuf);

            sprintf(s,"DROP_%d\n",dropRate);
            output_serial(s, sizeof(s));
            break;

         case SET_MIN_CW:
            for(i=0; i<5; i++)
            {
                inbuf[i] = buffer[i+3];
            }
            cw_min = atoi(inbuf);
            sprintf(s,"CW_MIN_%d\n",cw_min);
            output_serial(s, sizeof(s));
            break;

         case SET_MAX_CW:
            for(i=0; i<5; i++)
            {
                inbuf[i] = buffer[i+3];
            }
            cw_max = atoi(inbuf);
            sprintf(s,"CW_MAX_%d\n",cw_max);
            output_serial(s, sizeof(s));
            break;

         case SET_MAC_RE:
            for(i=0; i<5; i++)
            {
                inbuf[i] = buffer[i+3];
            }
            macDropRate = atoi(inbuf);
            sprintf(s,"MACRE_%d\n",macDropRate);
            output_serial(s, sizeof(s));
            break;

        case SET_TEST_CASE:
            for(i=0; i<5; i++)
            {
                inbuf[i] = buffer[i+3];
            }
            Sme.tc.cType = (unsigned char) atoi(inbuf);
            sprintf(s,"TC_%d\n",Sme.tc.cType);
            output_serial(s, sizeof(s));
            config(0, 3);
            break;

        case SET_HOLD_TIME:
            for(i=0; i<5; i++)
            {
                inbuf[i] = buffer[i+3];
            }
            Nclme.holdingTime =  atoi(inbuf);
            sprintf(s,"HT_%d\n",Nclme.holdingTime);
            output_serial(s, sizeof(s));
            break;

        case SET_KB_OP:
            for(i=0; i<5; i++)
            {
                inbuf[i] = buffer[i+3];
            }
            Sme.tc.cKbOP =  atoi(inbuf);
            sprintf(s,"KB-OP_%d\n",Sme.tc.cKbOP );
            output_serial(s, sizeof(s));
            break;

       case SET_CW_UPDAT_mCWmin:
            for(i=0; i<5; i++)
            {
                inbuf[i] = buffer[i+3];
            }
            Nclme.medianCWmin =  atoi(inbuf);
            sprintf(s,"CW_m_%d\n",Nclme.medianCWmin);
            output_serial(s, sizeof(s));
            break;

       case SET_CW_UPDAT_minCWmin:
            for(i=0; i<5; i++)
            {
                inbuf[i] = buffer[i+3];
            }
            Nclme.algoMinCWmin =  atoi(inbuf);
            sprintf(s,"CW_min_%d\n",Nclme.algoMinCWmin);
            output_serial(s, sizeof(s));
            break;

       case SET_CW_UPDAT_maxCWmin:
            for(i=0; i<5; i++)
            {
                inbuf[i] = buffer[i+3];
            }
            Nclme.algoMaxCWmin =  atoi(inbuf);
            sprintf(s,"CW_max_%d\n",Nclme.algoMaxCWmin);
            output_serial(s, sizeof(s));
            break;

       case DR_CAPA_TEST:
            for(i=0; i<5; i++)
            {
                inbuf[i] = buffer[i+3];
            }
            Alme.drCapaTest =  atoi(inbuf);
            sprintf(s,"DR_CAPA_%d\n",Alme.drCapaTest);
            output_serial(s, sizeof(s));
            break;

        default:
            #if defined (DEBUG)
                putsUSBUSART(">> wrong command\n\r");
            #endif
            break;
    }
}
