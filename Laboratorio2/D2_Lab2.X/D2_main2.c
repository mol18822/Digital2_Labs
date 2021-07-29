// Dispositivo:		PIC16F887
// Autor;			Luis Pedro Molina Vel�squez 
// Carn�;			18822
// Compilador:		pic-as (v2.31) MPLABX V5.40
// Curso:           Electr�nica Digital II
// ----------------------------------------------------------------------------     
// ----------------------- Laboratorio No. 2  ---------------------------------      
// --------------- Interrupciones y uso de librerias --------------------------

// Creado:                  22 julio , 2021
// Ultima modificaci�n:     29 julio , 2021


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
#include <stdint.h>
#include <stdio.h> 
#include <stdlib.h>
#include <pic16f887.h> 
#include "UART.h" 
#include "ADC.h"
#include "LCD.h" 
#define _XTAL_FREQ 4000000

// ---------------------- Declaraci�n de puertos ------------------------------

#define RS RE0
#define EN RE2
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

// ----------------------------- Variables ------------------------------------

char voltage1, voltage2;                    // Voltaje de Pot 1 y Pot 2
char voltaje_b, voltaje_c;                  //
char value;                                 // Variable para almacenar valor
char centenas;                              // Variable para centenas
char decenas;                               // Variable para decenas
char unidades;                              // Variable para unidades
char residuo;                               // Variable para residuo de divisi�n
char dataV1;                                // Dato V1
char dataV2;                                // Dato V2
char counter = 0;                           // Variable para contador

// ----------------------------- Prototipos -----------------------------------

void setup(void);                           // Funci�n de par�metros
char division (char value);                 // Funci�n de separaci�n de nibbles
char voltajes (char voltajes_1);            // Funci�n de voltajes
void putch(char data);                      // Funci�n para comunicaci�n serial

// ------------- Interrupci�n de ADC para potenciometros ---------------------- 

void __interrupt() isr(void){

    if (ADIF){
        if(ADCON0bits.CHS == 0){
            ADCON0bits.CHS = 1;
            voltage1 = ADRESH;
        }
        else if(ADCON0bits.CHS == 1){
            ADCON0bits.CHS = 0;
            voltage2 = ADRESH; 
        }
    }
    ADIF = 0;
}

// -------------------------------- Main --------------------------------------

void main(void) {
    
    setup();
    LCD_Init();                             // LCD off
    LCD_Clear();                            // Limpieza de LCD
    char bufferV1[20];                      // Almacenando voltaje en string
    char bufferV2[20];                      // Almacenando voltaje en string
    LCD_Set_Cursor(1,1);                    // Primera linea, primera posici�n
    LCD_Write_String("S_1:  S_2:  S_3:");   // Print voltage indicators
    
// -------------------------------- Loop --------------------------------------    
    
    while(1) {
        
    dataV1 = voltage1*0.0196;               //
    dataV2 = voltage2*0.0196;               //

    sprintf(bufferV1, "%d V ", dataV1);     // Traducci�n de datos 
    sprintf(bufferV2, "%d V ", dataV2);     // Traducci�n de datos
    
    Lcd_Set_Cursor(2,2);                    // Segunda fila, segunda posici�n
    Lcd_Write_String(bufferV1);             // Desplegar valor almacenado
    Lcd_Write_String(bufferV2);             // Desplegar valor almacenado
    Lcd_Write_String('  ');                 // Espacio para counter 
    
    __delay_ms(1000);                       // 
         
    if (ADCON0bits.GO == 0){                // Encendiendo nuevamente
        __delay_us(100);                    //
        ADCON0bits.GO = 1;
    }
    
// ----------------- L�gica para desplegar valor de Pot 1 ---------------------
    
    division(voltage1);                     // Desplegando valor de Pot 1
    __delay_ms(250);                        //
    printf("Valor del Potenciometro 1:\r"); //
    __delay_ms(250);                        //
    
    TXREG = centenas;                       //
    __delay_ms(250);                        //
    TXREG = 46;                             //
    __delay_ms(250);                        //
    TXREG = decenas;                        //
    __delay_ms(250);                        //
    TXREG = unidades;                       //
    __delay_ms(250);                        //
    printf("\r");                           //
    
// ----------------- L�gica para desplegar valor de Pot 2 ---------------------
    
    division(voltage2);                     // Desplegando valor de Pot 2
    __delay_ms(250);                        //
    printf("Valor del Potenciometro 2:\r"); //
    __delay_ms(250);                        //

    TXREG = centenas;                       //
    __delay_ms(250);                        //
    TXREG = 46;                             //
    __delay_ms(250);                        //
    TXREG = decenas;                        //
    __delay_ms(250);                        //
    TXREG = unidades;                       //
    __delay_ms(250);                        //
    printf("\r");                           //
    
// --------- Separaci�n de "counter" para ser desplegado en la LCD ------------
    
    division(counter);                      // Divisi�n a variable counter
    LCD_Write_Char(centenas);               // Desplegando centenas
    LCD_Write_Char(decenas);                // Desplegando decenas
    LCD_Write_Char(unidades);               // Desplegando unidades
    
// ----------------------- Texto mostrado por la consola ----------------------
    
    __delay_ms(250);
    printf("\r Si desea aumentar contador, oprima +\r");
    __delay_ms(250);
    printf("\r Si desea disminuir contador, oprima -\r");
    __delay_ms(250);
    printf("\r Si no desea realizar nada oprima *\r");
    __delay_ms(250);

// -------------- L�gica de funcionamiento de comunicaci�n serial -------------
    
    while(RCIF == 0);
    char respuesta = RCREG;                 // 
    if (respuesta == '+'){                  // Si se recibe "+"
        counter = counter + 1;              // Aumentar variable counter
    }
    else if (respuesta == '-'){             // Si se recibe "-"
        counter = counter - 1;              // Decrementar variable counter
    }
    else if (respuesta == '*'){             // Si se recibe "*"
        NULL;                               // Nada
    }
    }
        
    return;
}

// ------------------------------ Putch ---------------------------------------

void putch(char data){                      // Funcion de libreria stdio.h
    while(TXIF == 0);                       //
    TXREG = data;                           // 
    return;
}

// ----------------------------- Divisi�n -------------------------------------

char division (char value){
    centenas = value/100;               // Valor/100 y guarda n�mero entero
    residuo  = value%100;               // Residuo de la divisi�n anterior
    decenas  = residuo/10;              // Valor/10 y guarda n�mero entero 
    unidades = residuo%10;              // Residuo de la divisi�n anterior

    centenas = centenas + 48;
    decenas  = decenas + 48;
    unidades = unidades + 48;
} 

// ------------------------------ Voltajes ------------------------------------

char voltajes(char voltaje_1){
    char voltaje_a;
    return voltaje_a = division(voltaje_1);
}

// ----------------------- Configuraci�n de I/O -------------------------------

void setup(void){

    ANSEL = 0b00000011;                 // Activando canales an�logicos
    TRISAbits.TRISA0 = 1;               // RA0 como input an�logico --> Pot 1
    TRISAbits.TRISA1 = 1;               // RA1 como input an�logico --> Pot 2
    
    TRISD = 0x00;                       // Outputs para LCD
    TRISE = 0x00;                       // Outputs para LCD
    
// ------------------------- Limpieza de puertos ------------------------------
    
    PORTA = 0x00;
    PORTD = 0x00; 
    PORTE = 0x00;    
    
// ------------------ Configuraci�n de oscillador interno --------------------- 
    
    OSCCONbits.IRCF2 = 1;                   // 1
    OSCCONbits.IRCF1 = 1;                   // 1
    OSCCONbits.IRCF0 = 0;                   // 0 --> 4 MHz
    OSCCONbits.SCS   = 1;                   // Oscillador interno == 1      
    
// ------------------- Configuraci�n de interrupciones ------------------------
    
    PIE1bits.ADIE   = 1;                    // Interrupci�n de ADC == 1
    PIR1bits.ADIF   = 0;                    // Limpieza de ADIE
    INTCONbits.GIE  = 1;                    // Interrupciones globales == 1
    INTCONbits.PEIE = 1;                    // Interrupciones perifericas == 1
    PIE1bits.RCIE   = 0;                    // Interrupcion RX
    PIE1bits.TXIE   = 0;                    // Interrupcion TX
    
// -------------------------- Configuraci�n de ADC ----------------------------

    ADC_config(1);                          // M�dulo de libreria
    
// ------------------ Configuraci�n de UART - RX y TX -------------------------
    
    TXSTAbits.SYNC = 0;                     //
    TXSTAbits.BRGH = 1;                     //
    BAUDCTLbits.BRG16 = 0;                  //
    
    SPBRG = 25;                             //
    SPBRGH = 1;                             //
        
    RCSTAbits.SPEN = 1;                     //
    RCSTAbits.RX9 = 0;                      //
    RCSTAbits.CREN = 1;                     //
    TXSTAbits.TX9 = 0;                      //
    TXSTAbits.TXEN = 1;
    PIR1bits.RCIF = 0;                      // RX flag
    PIR1bits.TXIF = 0;                      // TX flag
}