#include <xc.h> 
#include <stdint.h>  
#include <pic16f887.h> 
#include "ADC.h"
#define _XTAL_FREQ 4000000

void ADC_config(char frecuencia) { 
    switch(frecuencia) {
        
        case 0:                             
            ADCON0bits.CHS = 0;             // Canal 0
            __delay_us(100);                //
    
            ADCON0bits.ADCS0 = 0;           //
            ADCON0bits.ADCS1 = 0;           // FOSC/2
            ADCON0bits.ADON = 1;            // ADC enable bit
            ADCON1bits.ADFM = 0;            // Justificación a la izquierda
            ADCON1bits.VCFG1 = 0;           // 5 V
            ADCON1bits.VCFG0 = 0;           // Ground
            break;
            
        case 1:                             //
            ADCON0bits.CHS = 0;             // Canal 0
            __delay_us(100);                //

            ADCON0bits.ADCS0 = 1;           // FOSC/8
            ADCON0bits.ADCS1 = 0;           //
            ADCON0bits.ADON = 1;            // ADC enable bit
            ADCON1bits.ADFM = 0;            // Justificación a la izquierda
            ADCON1bits.VCFG1 = 0;           // 5 V
            ADCON1bits.VCFG0 = 0;           // Ground
            break;
            
        case 2: 
            ADCON0bits.CHS = 0;             // Canal 0
            __delay_us(100);                //

            ADCON0bits.ADCS0 = 0;           // FOSC/32
            ADCON0bits.ADCS1 = 1;           //
            ADCON0bits.ADON = 1;            // ADC enable bit
            ADCON1bits.ADFM = 0;            // Justificación a la izquierda
            ADCON1bits.VCFG1 = 0;           // 5 V
            ADCON1bits.VCFG0 = 0;           // Ground
            break;
            
        case 3: 
            ADCON0bits.CHS = 0;             //canal 0
            __delay_us(100);                //

            ADCON0bits.ADCS0 = 1;           // FRC
            ADCON0bits.ADCS1 = 1;           //
            ADCON0bits.ADON = 1;            // ADC enable bit
            ADCON1bits.ADFM = 0;            // Justificacion a la izquierda
            ADCON1bits.VCFG1 = 0;           // 5 V
            ADCON1bits.VCFG0 = 0;           // Ground
            break;
    }
    return;
}
