#include <xc.h> 
#include <stdint.h>  
#include <pic16f887.h> 
#include "UART.h"

void UART_config(void){

    SPBRG = 103;        
    SPBRGH = 0;
    BAUDCTLbits.BRG16 = 1; 
    TXSTAbits.BRGH    = 1; 
    TXSTAbits.SYNC    = 0; 
    RCSTAbits.SPEN    = 1; 
    RCSTAbits.CREN    = 1; 
    TXSTAbits.TX9     = 0; 
    TXSTAbits.TXEN    = 1; 
    RCSTAbits.RX9     = 0; 
    INTCONbits.GIE    = 1; 
    INTCONbits.PEIE   = 1;
    PIR1bits.TXIF     = 0;  
    PIR1bits.RCIF     = 0;
    
    return;
}
