// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef x_USART //definir nuevo nombre que el otro header para que no lo llame
#define	x_USART //dos veces

#include <xc.h> // include processor files - each processor file is guarded.  

void UART_config(void);

#endif	/* XC_HEADER_TEMPLATE_H */