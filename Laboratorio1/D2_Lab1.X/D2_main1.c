// Dispositivo:		PIC16F887
// Autor;			Luis Pedro Molina Velásquez 
// Carné;			18822
// Compilador:		pic-as (v2.31) MPLABX V5.40
// Curso:           Electrónica Digital II
// ----------------------------------------------------------------------------     
// ----------------------- Laboratorio No. 1  ---------------------------------      
// --------------- Interrupciones y uso de librerias --------------------------

// Creado:                  10 julio , 2021
// Ultima modificación:     14 julio , 2021

//  Bits de configuracion 

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

#include <xc.h>
#include "ADC.h"
#define _XTAL_FREQ 8000000

// ----------------------------- Variables ------------------------------------

char var4Disp1, var4Disp2;                  // Vars para transis de D1 y D2 
char counter = 0;                           // Var para alarma de overflow
char increase_ADC;                          //
char decrease_ADC;                          //
char value_ADC;                             //
char flag_ADC = 1;                          //
char table4Disp [16] = {                    //
    0b00111111,                             // 0
    0b00000110,                             // 1
    0b01011011,                             // 2    
    0b01001111,                             // 3
    0b01100110,                             // 4
    0b01101101,                             // 5 
    0b01111101,                             // 6
    0b00000111,                             // 7
    0b01111111,                             // 8
    0b01101111,                             // 9
    0b01110111,                             // A
    0b01111100,                             // B
    0b00111001,                             // C
    0b01011110,                             // D
    0b01111001,                             // E
    0b01110001 };                           // F

// --------------------------- Interrupciones ---------------------------------

void __interrupt() isr(void) {
    
    if (INTCONbits.T0IF == 1){              // Si la flag del Timer0 == 1
        TMR0 = 100;                         // TMR0 == 100
        
    if(PORTEbits.RE1 == 1) {                // Si el transis de Disp1 == 1
        PORTD = 0;                          // 0 a PORTD
        PORTEbits.RE1 = 0;                  // Transis de Disp1 == 0
        PORTEbits.RE0 = 1;                  // Transis de Disp2 == 1
        PORTD = var4Disp1;                  // Disp2 --> bits más significativos
    }
                
    else {
        PORTD = 0;                          // 
        PORTEbits.RE1 = 1;                  // Transis de Disp2 == 1
        PORTEbits.RE0 = 0;                  // Transis de Disp1 == 0 
        PORTD = var4Disp2;                  // Disp1 --> bits menos significativos  
            }
    INTCONbits.T0IF	 = 0;                   // Flag del Timer0 = 0
  }
    
       
    PIR1bits.ADIF = 0;                      //limpio la bandera
                            }
    
    if (INTCONbits.RBIF == 1) {             // Si la flag RBIF == 1
        
    if (PORTBbits.RB0 == 1) {               // Si PB1 == 1   
        counter--;                          // Decrease var counter
            }
    if (PORTBbits.RB1 == 1){                // Si PB2 == 1
        counter++;                          // Increase var counter
      }
    }
    INTCONbits.RBIF = 0;                    // Flag RBIF == 0
} 

// ----------------------------- Prototipos -----------------------------------
void setup (void);

// -------------------------------- Main --------------------------------------

void main (void) {
    setup();
    
// -------------------------------- Loop --------------------------------------
    
while (1) { 
    ADC_config(flag_ADC);                    // Configuración del ADC por líbreria
    var4Disp1 = table4Disp[increase_ADC];   // 
    var4Disp2 = table4Disp[decrease_ADC];   //    
    PORTC = counter;                        //
    if (value_ADC > counter){               // Si el valor ADC es mayor a counter
        PORTEbits.RE2 = 1;                  // Led de alarma == 1
       }
    else {                                  // Si no es mayor
        PORTEbits.RE2 = 0;                  // Led de alarma == 0
       }   
    }
}


// ----------------------- Configuración de I/O -------------------------------

void setup (void) {
   
    ANSEL = 0b01;                           // AN0 analógico --> Potenciometro
    ANSELH = 0;                             //
    ADCON0 = 0b01010101;                    // Habilitando ADC
    
    TRISA = 0b00000001;                     // 
    TRISB = 0b00000011;                     // 
    TRISC = 0x00;                           //
    TRISD = 0x00;                           //
    TRISE = 0x00;                           //
    
// ------------------------- Limpieza de puertos ------------------------------
    
    PORTA = 0;
    PORTC = 0;
    PORTD = 0; 
    PORTE = 0;
    
// ------------------ Configuración de oscillador interno --------------------- 
    
    OSCCONbits.IRCF2 = 1;                   // 1
    OSCCONbits.IRCF1 = 1;                   // 1
    OSCCONbits.IRCF0 = 1;                   // 1 --> 8 MHz
    OSCCONbits.SCS   = 1;                   // Oscillador interno == 1      
    
// ----------------------------- IOCB ----------------------------------------- 
    
    INTCONbits.GIE  = 1;                    //
    INTCONbits.PEIE = 1;                    //
    INTCONbits.T0IE = 1;                    //
    INTCONbits.RBIE = 1;                    //
    IOCBbits.IOCB0  = 1;                    // IOCB en RB0 = 1 --> PB1 
    IOCBbits.IOCB1  = 1;                    // IOCB en RB1 = 1 --> PB2
    
// ------------------------------ Timer0 --------------------------------------
    
    OPTION_REG = 0x84;                      //
    TMR0 = 100;                             //
    INTCON = 0b11101000;                    // 

 // ------------------------------ ADC ----------------------------------------
    
    ADCON1 = 0b00000000;                    //
    ADCON0 = 0b10000001;                    //
    PIR1bits.ADIF = 0;                      //
    PIE1bits.ADIE = 1;                      // Interrupción del ADC == 1 
    ADCON1bits.ADFM = 0;                    // Justificación a la izquierda
   
}


