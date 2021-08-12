// Dispositivo:		PIC16F887
// Autor;			Luis Pedro Molina Velásquez 
// Carné;			18822
// Compilador:		pic-as (v2.31) MPLABX V5.40
// Curso:           Electrónica Digital II
// ----------------------------------------------------------------------------     
// ----------------------- Laboratorio No. 4  ---------------------------------      
// ----------------------------- I2C ------------------------------------------
// ------------------------- SLAVE No. 1 PIC ----------------------------------

// Creado:                  05 agosto , 2021
// Ultima modificación:     12 agosto , 2021

//  Bits de configuracion 

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// ----------------------------- Librerias ------------------------------------

#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>  
#include "ADC.h"
#include "I2C.h"

// ----------------------------- Variables ------------------------------------

uint8_t var;
uint8_t voltage;

// ----------------------------- Prototipos -----------------------------------

void setup(void);

// ------------------------- Interrupciones -----------------------------------

void __interrupt() isr(void){
   
// ----------------------- Interrupción de ADC --------------------------------
    
       if(PIR1bits.ADIF == 1)       
       {
           if(ADCON0bits.CHS == 0) { 
               PORTB = ADRESH;
               voltage = ADRESH;
           }
           PIR1bits.ADIF = 0;
       }
    
    if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
    if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
        var = SSPBUF;                 
        SSPCONbits.SSPOV = 0;       
        SSPCONbits.WCOL = 0;        
        SSPCONbits.CKP = 1;         
    }

    if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
        var = SSPBUF;                 
        PIR1bits.SSPIF = 0;         
        SSPCONbits.CKP = 1;         
        while(!SSPSTATbits.BF);     
        voltage = SSPBUF;             
        __delay_us(250);
            }
        
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
        var = SSPBUF;
        BF = 0;
        SSPBUF = voltage;
        SSPCONbits.CKP = 1;
        __delay_us(250);
        
        while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}

// -------------------------------- Main --------------------------------------

void main(void) {
    setup();

// -------------------------------- Loop --------------------------------------  
    
    while(1){
        
        if(ADCON0bits.GO == 0){
            __delay_ms(100);
            ADCON0bits.GO = 1;
            
             }
    }
    return;
}

// ----------------------- Configuración de I/O -------------------------------

void setup(void){
    ANSEL = 0b00000001;
    ANSELH = 0;
    
    TRISAbits.TRISA0 = 1;
    TRISB = 0;
    
// ------------------------- Limpieza de puertos ------------------------------
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
// ------------------ Configuración de oscillador interno --------------------- 
    
    OSCCONbits.IRCF2 = 1;                   // 1
    OSCCONbits.IRCF1 = 1;                   // 1
    OSCCONbits.IRCF0 = 1;                   // 1 --> 8 MHz
    OSCCONbits.SCS   = 1;                   // Oscillador interno == 1   
    
// -------------------- Configuración de interrupciones -----------------------
    
    PIE1bits.ADIE = 1;                      // ADC interrupt == 1
    PIR1bits.ADIF = 0;                      // 
    INTCONbits.GIE = 1;                     // Global interrupt == 1
    INTCONbits.PEIE = 1;                    // Periferical interrupt == 1
    
// ------------------------ Configuración de ADC ------------------------------
    
    ADC_config(2);                          //
    
// ----------------------- Configuración de Slave -----------------------------
        
    I2C_Slave_Init(0x50);                   //
    
}