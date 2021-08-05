// Dispositivo:		PIC16F887
// Autor;			Luis Pedro Molina Velásquez 
// Carné;			18822
// Compilador:		pic-as (v2.31) MPLABX V5.40
// Curso:           Electrónica Digital II
// ----------------------------------------------------------------------------     
// ----------------------- Laboratorio No. 3  ---------------------------------      
// ----------------------------- SPI ------------------------------------------

// Creado:                  29 julio  , 2021
// Ultima modificación:     05 agosto , 2021

//  Bits de configuracion 

//CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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
#include "SPI.h"

// ----------------------------- Variables ------------------------------------

int varADC1, varADC2;
int var1, var2;                                    
int counter;                                    // Variable counter

// ----------------------------- Prototipos -----------------------------------

void setup(void);                               // PIC configurations
void putch(char data);                          // Send & receive data

// ------------------------- Interrupciones -----------------------------------

void __interrupt() isr(void) {

// ---------------------- Interrupción de ADC ---------------------------------
    
    if(PIR1bits.ADIF == 1) {                    // Flag ADIF == 1
        
    if(ADCON0bits.CHS == 0) {                   // Desplegar valor en PORTC
               varADC1 = ADRESH;                //
               ADCON0bits.CHS = 1;              //
           }
    else if(ADCON0bits.CHS == 1) {              //
            ADCON0bits.CHS = 0;                 //
               varADC2 = ADRESH;                //

            }
            PIR1bits.ADIF = 0;                  // Flag ADIF == 0
       }
       
// ---------------------- Interrupción de SPI ---------------------------------
       
    if(SSPIF == 1) {                            //
        var1 = Read_SPI();                      //
        Write_SPI(varADC2);                     //
        var2 = Read_SPI();                      //
        Write_SPI(varADC1);                     //
        SSPIF = 0;                              //
    }
}

// -------------------------------- Main --------------------------------------
void main(void) {
    
    setup();                                    // 
    
// -------------------------------- Loop -------------------------------------- 
    
    while(1) {
        
      if (ADCON0bits.GO == 0){                  // Encendiendo y apagando
            __delay_us(100);                    // constantemente
            ADCON0bits.GO = 1;
        }  
    }
}

// ----------------------- Configuración de I/O -------------------------------

void setup(void) {
    
    ANSEL  = 0b00000011;                        //
    ANSELH = 0;                                 //
    
    TRISAbits.TRISA0 = 1;                       // Potenciometro 1
    TRISAbits.TRISA1 = 1;                       // Potenciometro 2
        
    TRISB = 0x00;                               //
    TRISD = 0x00;                               //
    
// ------------------------- Limpieza de puertos ------------------------------    
    
    PORTA = 0x00;                               //
    PORTB = 0x00;                               //
    PORTD = 0x00;                               //
    
// ------------------ Configuración de oscillador interno --------------------- 
    
    OSCCONbits.IRCF2 = 1;                       // 1
    OSCCONbits.IRCF1 = 1;                       // 1
    OSCCONbits.IRCF0 = 0;                       // 0 --> 4 MHz
    OSCCONbits.SCS   = 1;                       // Oscillador interno == 1     
    
// ------------------- Configuración de interrupciones ------------------------
    
    INTCONbits.GIE   = 1;                       // Interrupciones == 1
    INTCONbits.PEIE  = 1;                       // 
    PIR1bits.SSPIF   = 0;                       // Flag SSPIF == 0
    PIE1bits.SSPIE   = 1;                       // 
    TRISAbits.TRISA5 = 1;                       // Seleccionando PIC slave
    
    Init_SPI(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW,
            SPI_IDLE_2_ACTIVE);
    
// ----------------------- Configuración de ADC -------------------------------
    
    ADC_config(2);
}