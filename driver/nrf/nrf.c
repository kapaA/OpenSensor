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
        File name: nrf.c
        Version:   v0.0
        Date:      2012-08-08
    ============================================================================
*/

/*============================================================================*/
/*                           INCLUDE STATEMENTS
==============================================================================*/

#include "nrf.h"

unsigned char nrfRxFrameWidthMem = 0;
unsigned char nrfTxFrameWidthMem = 0;

/*==============================================================================
** Function...: nrf
** Return.....: GLOB_RET
** Description: initializes the pins used by NRF
** Created....: 19.07.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
==============================================================================*/

GLOB_RET nrf(initModul *im)
{
    GLOB_RET ret = GLOB_ERROR_INIT;
    // Error if timer or spi is not initialized
    if(im->spi == 0 || im->timer == 0) return ret; 

    TRISAbits.TRISA4  = OUTPUT;  // Set A4 as TX_EN

    TRISBbits.TRISB2  = INPUT;   // Set B2 as CD
    TRISBbits.TRISB3  = INPUT;   // Set B3 as AM
    TRISBbits.TRISB4  = INPUT;   // Set B4 as DR

    TRISBbits.TRISB7  = OUTPUT; // Set B7 as CSN
    TRISBbits.TRISB13 = OUTPUT; // Set B13 as PWR_UP
    TRISBbits.TRISB14 = OUTPUT; // Set B14 as TRX_CE

    RPINR0bits.INT1R  = 4;      //set RP4 (DR) to external interrupt 1

    // nRF initial modes
    TX_EN  = 0; // Rx mode
    TRX_CE = 0; // Standby mode
    PWR_UP = 0; // Power down
    CSN    = 0; // Chip Select Not = enabled

    // Initializes the external interrupt (From DR)
    INTCON2 = 0x0000;     /* Setup interupt on falling edge */
    IFS1bits.INT1IF = 0;  /* Reset INT1 interrupt flag */
    IEC1bits.INT1IE = 1;  /* Enable INT1 Interrupt Service Routine */
    IPC5bits.INT1IP = 1;  /* Set low priority */

    ret     = initNRF();
    
    im->nrf = 1;

    return ret;
}

/*------------------------------------------------------------------------------
** Function...: nrfConf
** Return.....: GLOB_RET
** Description: configure nrf chip
** Created....: 25.06.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/
GLOB_RET initNRF(void){
    int i;
    char *nRFAddress = "MOTE";
    unsigned char nRFConfig[10];

    nRFConfig[0] = CH_NO_BYTE; //Sets center frequency together with HFREQ_PLL
    //Output power, Band 433 or 868/915 MHz
    nRFConfig[1] = PA_PWR_2dBm|HFREQ_PLL_433MHz|CH_NO_BIT8;
    nRFConfig[2] = TX_AFW_4BYTE|RX_AFW_4BYTE; //Tx and Rx Address width
    nRFConfig[3] = RX_PW_4BYTE; //RX payload width
    nRFConfig[4] = TX_PW_4BYTE; //Tx payload width
    nRFConfig[5] = nRFAddress[0];
    nRFConfig[6] = nRFAddress[1];
    nRFConfig[7] = nRFAddress[2];
    nRFConfig[8] = nRFAddress[3];
    nRFConfig[9] = CRC8_EN|XOF_16MHz; //CRC check. Crystal oscillator frequency.

    PWR_UP = 1;
    CSN    = 1;

    CSN    = 0;
    SPI_wr(WC); //Write nRF configuration
    for(i=0;i<10;i++)
    {
        SPI_wr(nRFConfig[i]);
    }
    CSN = 1;

    CSN = 0;
    SPI_wr(WTA); //Write TX Address
    for(i=0;i<4;i++)
    {
        SPI_wr(nRFAddress[i]);
    }
    CSN = 1;

    TRX_CE = 1;

    return GLOB_SUCCESS;
}

/*------------------------------------------------------------------------------
** Function...: setNRFpw
** Return.....: void
** Description: set payload width
** Created....: 25.06.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/

void setNRFpw(int type, char pw)
{
  //configure RX payload width
  if(type == RX)
  {
    CSN = 0;
    SPI_wr(WC3); //Write to nRF configuration bit 3
    SPI_wr(pw);
    CSN = 1;
    rxClockOutSize = pw;
  }
  //configure TX payload width
  else if(type == TX)
  {
    CSN = 0;
    SPI_wr(WC4); //Write to nRF configuration bit 4
    SPI_wr(pw);
    CSN = 1;
  } // end else if
}

/*------------------------------------------------------------------------------
** Function...: TXPacket
** Return.....: void
** Description: transmit frame
** Created....: 25.06.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/
GLOB_RET TXPacket(void)
{
  
  TRX_CE = 1; // Start transmitting
  while(!DR);
  TX_EN = 0;
  Log.totalTx++;
 
  return GLOB_SUCCESS;
}

/*------------------------------------------------------------------------------
** Function...: clockIn
** Return.....: int
** Description: clock payload in to nrf register
** Created....: 25.06.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/
int clockIn(frame packet, int size)
{
  char *p = (char *)&packet;
  int i;
  
  TRX_CE = 0;
  TX_EN = 1;
  CSN = 0;

  SPI_wr(WTP); //Write TX Payload

  for(i=0;i<size;i++)
    SPI_wr(p[i]);
  CSN = 1;

  return 0;
}

/*------------------------------------------------------------------------------
** Function...: clockOut
** Return.....: frame
** Description: clock payload out from nrf register
** Created....: 25.06.2012 by Achuthan
** Modified...: dd.mm.yyyy by nn
------------------------------------------------------------------------------*/
frame clockOut(int size)
{
  //The same as RXpacket used before
  int i;
  char SpiRxBuf[PACKET_SIZE];
  frame *f = (frame *)&SpiRxBuf;

  TRX_CE = 0;
  CSN = 0;
  SPI_wr(RRP); //Read RX Payload
  for (i=0;i<size;i++)
    SpiRxBuf[i] = SPI_wr(0);
  CSN = 1;
  TRX_CE = 1;
  
  return f[0];
}
 



void nrfReset(void)
 {
    // nRF initial modes
    TX_EN  = 0; // Rx mode
    TRX_CE = 0; // Standby mode
    PWR_UP = 0; // Power down
    CSN    = 0; // Chip Select Not = enabled

    // Initializes the external interrupt (From DR)
    INTCON2 = 0x0000;     /* Setup interupt on falling edge */
    IFS1bits.INT1IF = 0;  /* Reset INT1 interrupt flag */
    IEC1bits.INT1IE = 1;  /* Enable INT1 Interrupt Service Routine */
    IPC5bits.INT1IP = 1;  /* Set low priority */

    int i;
    char *nRFAddress = "MOTE";
    unsigned char nRFConfig[10];

    nRFConfig[0] = CH_NO_BYTE; //Sets center frequency together with HFREQ_PLL
    //Output power, Band 433 or 868/915 MHz
    nRFConfig[1] = PA_PWR_2dBm|HFREQ_PLL_433MHz|CH_NO_BIT8; 
    nRFConfig[2] = TX_AFW_4BYTE|RX_AFW_4BYTE; //Tx and Rx Address width
    nRFConfig[3] = RX_PW_4BYTE; //RX payload width
    nRFConfig[4] = TX_PW_4BYTE; //Tx payload width
    nRFConfig[5] = nRFAddress[0];
    nRFConfig[6] = nRFAddress[1];
    nRFConfig[7] = nRFAddress[2];
    nRFConfig[8] = nRFAddress[3];
    nRFConfig[9] = CRC8_EN|XOF_16MHz; //CRC check. Crystal oscillator frequency.

    PWR_UP = 1;
    CSN    = 1;

    CSN    = 0;
    SPI_wr(WC); //Write nRF configuration
    for(i=0;i<10;i++)
    {
        SPI_wr(nRFConfig[i]);
    }
    CSN = 1;

    CSN = 0;
    SPI_wr(WTA); //Write TX Address
    for(i=0;i<4;i++)
    {
        SPI_wr(nRFAddress[i]);
    }
    CSN = 1;

    TRX_CE = 1;
 }
