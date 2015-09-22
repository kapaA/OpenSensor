/* 
 * File:   nrfbits.h
 * Author: ap
 *
 * Created on 17. juli 2012, 12:23
 */

#ifndef NRFBITS_H
#define	NRFBITS_H

// Port config for nRF905 ( pin function)
#define TX_EN       LATAbits.LATA4    //TX_EN=1 TX mode, TX_EN=0 RX mode
#define TRX_CE      LATBbits.LATB14    //Enables chip for receive and transmit
#define PWR_UP      LATBbits.LATB13    //Power up chip

#define CD          PORTBbits.RB2     //Carrier Detect
#define AM          PORTBbits.RB3     //Address Match
#define DR          PORTBbits.RB4     //Receive and transmit Data Ready

#define CSN         LATBbits.LATB7    //SPI enable, active low






#endif	/* NRFBITS_H */

