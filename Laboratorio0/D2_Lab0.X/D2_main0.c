// Dispositivo:		PIC16F887
// Autor;			Luis Pedro Molina Velásquez 
// Carné;			18822
// Compilador:		pic-as (v2.31) MPLABX V5.40
// ----------------------------------------------------------------------------     
// ----------------------- Laboratorio No. 0  ---------------------------------      
// ------------------------ Jueco Carrera -------------------------------------

// Creado:                  12 julio , 2021
// Ultima modificación:     14 julio , 2021

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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 8000000

// ------------------------------ Variables -----------------------------------

// ------------------------------ Funciones -----------------------------------

void setup(void);
void semaforo(void);
int juego_carrera(unsigned int var);

// -------------------------------- Main --------------------------------------

void main(void) {
    setup();

//--------------------------------- Loop --------------------------------------

    while (1) {
        
        if (PORTAbits.RA0 == 0) {
            __delay_ms(50);
            
        if (PORTAbits.RA0 == 1) {
            semaforo();
            juego_carrera(1);
            }
        }
    } 
}

// ------------------------------ Macros -------------------------------------- 

void semaforo(void) { 
    
    __delay_ms(500);
    PORTAbits.RA3 = 1;                          // Ciclo de luz roja

    __delay_ms(500);
    PORTAbits.RA3 = 0;
    
    PORTAbits.RA4 = 1;                          // Ciclo de luz amarilla
    __delay_ms(500);
    PORTAbits.RA4 = 0;
    
    PORTAbits.RA5 = 1;                          // Ciclo de luz verde
    __delay_ms(500);
    PORTAbits.RA5 = 0;    
    
}

int juego_carrera (unsigned int var) {
    
    if (var == 1) {
        while (1) {
            if (PORTAbits.RA1 == 0) {           // Si PB1 == 0 
                __delay_ms(100);                // Nada
                
            if (PORTAbits.RA1 == 1) {           // Si PB1 == 1
                PORTC ++;                       // Increase PORTC -> Player 1
                
            if (PORTC == 0b11111111) {          // Si PORTC == 11111111
                PORTB = 0b00000110;             // Load to PORTB 00000110
                PORTBbits.RB7 = 1;              // Transis de disp == 1
                PORTAbits.RA6 = 1;              // Led de Player1 win == 1
                break;                          // End game
                }     
            }
        }
            
            if (PORTAbits.RA2 == 0) {           // Si PB2 == 0
                __delay_ms(100);                // Nada
                    
            if (PORTAbits.RA2 == 1) {           // Si PB2 == 1
                PORTD ++;                       // Increase PORTD -> Player 2 
                
            if (PORTD == 0b11111111) {          // Si PORTD == 11111111
                PORTB = 0b01011011;             // Load to PORTB = 01011011
                PORTBbits.RB7 = 1;              // Transis de disp = 1
                PORTAbits.RA7 = 1;              // Led de Player2 win == 1
                break;                          // End game
                }
            }
        }
            
        }
    } 
}

// ----------------------- Configuración de I/O -------------------------------

void setup(void) {
    
    ANSEL  = 0;
    ANSELH = 0;
    
    TRISA = 0b00000111;                     // Leds y PB's
    TRISB = 0x00;                           // 7seg Display
    TRISC = 0x00;                           // Cont Player 1
    TRISD = 0x00;                           // Cont Player 2
    
// ------------------------- Limpieza de puertos ------------------------------

    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    
// ------------------ Configuración de oscillador interno --------------------- 
    
    OSCCONbits.IRCF2 = 1;                // 1
    OSCCONbits.IRCF1 = 1;                // 1
    OSCCONbits.IRCF0 = 1;                // 1 --> 8 MHz
    OSCCONbits.SCS   = 1;                // Oscillador interno == 1    
}    