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
#include <string.h> 
#include <stdlib.h>
#include "SPI.h"

// ----------------------------- Variables ------------------------------------

char pot1, pot2;                            //                                                                
char Centenas;                              // Variable para centenas        
char Decenas;                               // Variable para decenas
char Unidades;                              // Variable para unidades
char Residuo;                               // Variable para residuo de división
char cen;                                   // Variable para centenas en UART
char dec;                                   // Variable para decenas en UART
char uni;                                   // Variable para unidades en UART
char var;                                   //                
char con;                                   //
int complete;                               //
int counter;                                //

// ----------------------------- Prototipos -----------------------------------

void setup(void);                           // PIC configuration
void ReadSlave(void);                       // Read from slave
char division (char value);                 // Separación de data
void information(void);                     // Console

// ------------------------- Interrupciones -----------------------------------

void __interrupt() isr(void)
{
     
}

// -------------------------------- Main --------------------------------------

void main(void) {
    
    setup();    

// -------------------------------- Loop --------------------------------------      
    
    while(1) {
        
    PORTCbits.RC2 = 0;                          // Comunicación con slave == 1
       __delay_ms(1);       
       
       Write_SPI(0x0A);
       pot1 = Read_SPI();
       __delay_ms(1);
       
       Write_SPI(0x0A);
       pot2 = Read_SPI();
       __delay_ms(1);
       
       __delay_ms(1);
       PORTCbits.RC2 = 1;                       // Comunicación con slave == 0
       information();
       
       PORTD = complete;
        
    }
}

// ----------------------------- División -------------------------------------

char division (char value) {
    Centenas = value/50;                        // X/100, guarda entero
    Residuo  = value%100;                       // Residuo
    Decenas  = Residuo/10;                      // X/10, guarda entero
    Unidades = Residuo%10;                      // Residuo 
    
    Centenas = Centenas + 48;
    Decenas  = Decenas + 48;
    Unidades = Unidades + 48;
} 

// ----------- Transformación de información de Potenciometros ----------------

void information (void) {
    
// ------------------------ Potenciometro No. 1 -------------------------------  
    
    division(pot1);                             //
    __delay_ms(250);                            //
    printf("\r El valor del POT1 es:\r");       //
    __delay_ms(250);                            //
    printf("  ");                               //
    __delay_ms(250);                            //
            
    TXREG = Centenas;                           //
    __delay_ms(250);                            //
    TXREG = 46;                                 //
    __delay_ms(250);                            //
    
    TXREG = Decenas;                            //
    __delay_ms(250);                            //
    
    TXREG = Unidades;                           //
    __delay_ms(250);                            //
    printf(" V ");                              //
    __delay_ms(250);                            //
    printf("\r");                               //
    
// ------------------------ Potenciometro No. 2 ------------------------------- 
    
    division(pot2);                             //
    __delay_ms(250);                            //
    printf("\r El valor del POT2 es: \r");      //
    __delay_ms(250);                            //
    printf("  ");                               //
    __delay_ms(250);                            //
    
    TXREG = Centenas;                           //
    __delay_ms(250);                            //
    TXREG = 46;                                 //
    __delay_ms(250);                            //
    
    TXREG = Decenas;                            //
    __delay_ms(250);                            //
    
    TXREG = Unidades;                           //
    __delay_ms(250);                            //
    printf(" V ");                              //
    __delay_ms(250);                            //
    printf("\r");                               //
    
    
// ---------------------- Lógica para ingresar valores ------------------------
    
    printf("Ingresar Centena: \r");             //
      defensaC:                                 //
       while(RCIF == 0);                        //
        cen = RCREG -48;                        //

       while(RCREG > '4'){                      //
           goto defensaC;                       //
       }
    
    printf("Ingresar Decenas: \r");             //
      defensaD:                                 //
        while(RCIF == 0);                       //
         dec = RCREG -48;                       //

        if(cen == 4){                           //
           while(RCREG > '5'){                  //
               goto defensaD;                   //
           }
       }

    printf("Ingresar Unidades: \r");            //
      defensaU:                                 //
       while(RCIF == 0);                        //
        uni = RCREG - 48;                       //

       if(cen == 4 && dec == 5){                //
           while(RCREG > '5'){                  //
               goto defensaU;                   //
           }
       }
      con = concat(cen, dec);                   //
      complete = concat(con, uni);              //
      __delay_ms(250);                          //
    printf("El numero elegido es: %d", complete);
}

// ------------------------------ Putch ---------------------------------------

void putch(char data){                      // 
    while(TXIF == 0);
    TXREG = data;                           // 
    return;
}   

// -------------------------- Concatenación------------------------------------

int concat(int a, int b)
{
 
    char s1[20];                            //
    char s2[20];                            //

    sprintf(s1, "%d", a);                   // Convirtiendo a string
    sprintf(s2, "%d", b);                   // Convirtiendo a string

    strcat(s1, s2);                         // Concatenando ambos strings
 
    int c = atoi(s1);                       // Regresando a integrador
    
    return c;                               //
}


// ----------------------- Configuración de I/O -------------------------------

void setup(void){
    
    ANSEL  = 0x00;                          //
    ANSELH = 0x00;                          //
    
    TRISC2 = 0;                             //
    TRISD  = 0x00;                          //
    
// ------------------------- Limpieza de puertos ------------------------------
    
    PORTC = 0x00;                           //
    PORTD = 0x00;                           //
        
    PORTCbits.RC2 = 1;
    Init_SPI(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE,
            SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    
// ------------------ Configuración de oscillador interno --------------------- 
    
    OSCCONbits.IRCF2 = 1;                   // 1
    OSCCONbits.IRCF1 = 1;                   // 1
    OSCCONbits.IRCF0 = 0;                   // 0 --> 4 MHz
    OSCCONbits.SCS   = 1;                   // Oscillador interno == 1   
    
// ------------------ Configuración de UART - RX y TX -------------------------
    
    TXSTAbits.SYNC = 0;                     //
    TXSTAbits.BRGH = 1;                     //
    BAUDCTLbits.BRG16 = 0;                  //
    
    SPBRG = 25;                             //
    SPBRGH = 1;                             //
    
    RCSTAbits.SPEN = 1;                     //
    RCSTAbits.RX9  = 0;                     //
    RCSTAbits.CREN = 1;                     //
    TXSTAbits.TX9  = 0;                     // 
    TXSTAbits.TXEN = 1;                     //    
    PIR1bits.RCIF = 0;                      // RX flag
    PIR1bits.TXIF = 0;                      // TX flag
    
}