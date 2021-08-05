#ifndef __SPI_H
#define	__SPI_H
#include <xc.h> 
#include <pic16f887.h>

typedef enum {
    SPI_MASTER_OSC_DIV4  = 0b00100000,          //
    SPI_MASTER_OSC_DIV16 = 0b00100001,          //
    SPI_MASTER_OSC_DIV64 = 0b00100010,          //
    SPI_MASTER_TMR2      = 0b00100011,          //
    SPI_SLAVE_SS_EN      = 0b00100100,          //
    SPI_SLAVE_SS_DIS     = 0b00100101           //
}   SPI_Type;                                   //

typedef enum {
    SPI_DATA_SAMPLE_MIDDLE   = 0b00000000,      //
    SPI_DATA_SAMPLE_END      = 0b10000000       //
}   SPI_Data_Sample;                            //

typedef enum {      
    SPI_CLOCK_IDLE_HIGH  = 0b00010000,          //
    SPI_CLOCK_IDLE_LOW   = 0b00000000           //
}   SPI_Clock_Idle;                             //

typedef enum {
    SPI_IDLE_2_ACTIVE    = 0b00000000,          //
    SPI_ACTIVE_2_IDLE    = 0b01000000           //
}   SPI_Transmit_Edge;                          //

void Init_SPI(SPI_Type, SPI_Data_Sample, SPI_Clock_Idle, SPI_Transmit_Edge);
void Write_SPI(char);
unsigned DataReady4_SPI();
char Read_SPI();

#endif	/* SPI_H */
      
        