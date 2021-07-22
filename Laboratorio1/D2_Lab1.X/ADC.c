#include <stdint.h>
#include <pic16f887.h>
#include "ADC.h"
#define _XTAL_FREQ 8000000

void ADC_config (uint8_t flag_ADC){
    if(flag_ADC == 1) {
        __delay_us(100);
        flag_ADC = 0;
        ADCON0bits.GO = 1;
    }
}