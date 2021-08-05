#include "SPI.h"

void Init_SPI(SPI_Type sType, SPI_Data_Sample sDataSample, 
     SPI_Clock_Idle sClockIdle, SPI_Transmit_Edge sTransmitEdge)
{
    TRISC5 = 0;
    if(sType & 0b00000100)                  //
    {
        SSPSTAT = sTransmitEdge;            //
        TRISC3 = 1;                         //
    }
    else                                    //
    {
        SSPSTAT = sDataSample | sTransmitEdge;
        TRISC3 = 0;                         //
    }
    
    SSPCON = sType | sClockIdle;            //
}

static void spiReceiveWait()                //
{
    while ( !SSPSTATbits.BF );              // 
}

void Write_SPI(char dat)                     //
{
    SSPBUF = dat;                           //
}

unsigned DataReady4_SPI()                     //
{
    if(SSPSTATbits.BF)                      //
        return 1;                           //
    else                                    //
        return 0;                           //
}

char Read_SPI()                              //
{
    spiReceiveWait();                       // 
    return(SSPBUF);                         // 
}