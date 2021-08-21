// Dispositivo:		PIC16F887
// Autor;			Luis Pedro Molina Velásquez 
// Carné;			18822
// Compilador:		pic-as (v2.31) MPLABX V5.40
// Curso:           Electrónica Digital II
// ----------------------------------------------------------------------------     
// ----------------------- Laboratorio No. 5  ---------------------------------      
// ------------------------ PIC y Adafruit IO ---------------------------------

// Creado:                  13 agosto , 2021
// Ultima modificación:     19 agosto , 2021

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
#include <string.h> 
#include <stdlib.h>

// ----------------------------- Variables ------------------------------------

int counter;                                // Variable para contador
int choosen_value;                          // Variable para valor ingresado 
char valor;                                 //
char Centenas;                              // Variable para centenas        
char Decenas;                               // Variable para decenas
char Unidades;                              // Variable para unidades
char Residuo;                               // Variable para residuo de división
char cen;                                   // Variable para centenas en UART
char dec;                                   // Variable para decenas en UART
char uni;                                   // Variable para unidades en UART
char conca;
int complete;
                                 

// ----------------------------- Prototipos -----------------------------------

void setup(void);                           // Función de configuración del PIC
void Text(void);                            // Función de UART
char division (char valor);                 // Función de separación de nibbles

// ------------------------- Interrupciones -----------------------------------

void __interrupt() isr(void) {

// --------------------- Interrupción de PORTB --------------------------------
    
    if (RBIF == 1) {
        
    if (PORTBbits.RB0 == 0) {               // Si PB1 se oprime
        counter = counter + 1;              // Increase counter --> Leds en PORTD 
    }
    
    if  (PORTBbits.RB1 == 0) {              // Si PB2 se oprime
        counter = counter - 1;              // Decrease counter --> Leds en PORTD
    }
    INTCONbits.RBIF = 0;                    // Limpiando interrupt flag
    }
}

// -------------------------------- Main --------------------------------------

void main(void) {
    
    setup();                                // PIC configuration
    
// -------------------------------- Loop --------------------------------------  
    
    while(1) {
        
    PORTD = counter;                        // Valor de counter a PORTD
    PORTA = choosen_value;                  // Valor escogido a PORTA
    Text();                                 // Comunicación serial
    }
    return;
}

// ------------------------------ Putch ---------------------------------------

void putch(char data){                      // 
    while(TXIF == 0);                       //
    TXREG = data;                           // 
    return;
}  

// ------------------------ Comunicación serial -------------------------------

void Text(void) {

// ------------------- Lógica para mostrar valor del contador -----------------
    
    __delay_ms(250);                        // Delay para despliegue de caracteres
    division(counter);                      // Dividiendo valor de counter
    printf("El valor del contador es:\r");  // 
    __delay_ms(250);                        //
    TXREG = Centenas;                       // 
    __delay_ms(250);                        //
    TXREG = Decenas;                        //
    __delay_ms(250);                        //
    TXREG = Unidades;                       //
    __delay_ms(250);                        //
    printf("\r");                           //
    
// ---------------------- Lógica para ingresar valores ------------------------
    
    printf("Ingresar Centena: \r");         // 
    defensaC:                               // Progra defensiva para ingreso 
    while(RCIF == 0);                       // de centenas 
    cen = RCREG -48;                        //

    while(RCREG > '2'){                     // Si el valor ingresado es mayor a 
    goto defensaC;                          // 2, regresar a defensaC
    }
    
    printf("Ingresar Decenas: \r");         //
    defensaD:                               // Progra defensiva para ingreso
    while(RCIF == 0);                       // de decenas
    dec = RCREG -48;                        //

    if(cen == 2){                           //
    while(RCREG > '5'){                     //
    goto defensaD;                          //
        }
    }

    printf("Ingresar Unidades: \r");        //
    defensaU:                               // Progra defensiva para ingreso 
    while(RCIF == 0);                       // de unidades
    uni = RCREG - 48;                       //

    if(cen == 2 && dec == 5){               //
    while(RCREG > '5'){                     //
    goto defensaU;                          //
        }
    }
      
// ------------------ Concatenación de valor ingresado ------------------------
    
    conca = concat(cen, dec);
    complete = concat(conca, uni);
    __delay_ms(250);
    printf("El número ingresado es: %d \r", complete);
    choosen_value = complete;
}

// ----------------------------- División -------------------------------------

char division (char value) {
    Centenas = value/50;                        // X/100, guarda entero
    Residuo  = value%100;                       // Residuo
    Decenas  = Residuo/10;                      // X/10, guarda entero
    Unidades = Residuo%10;                      // Residuo 
    
    Centenas = Centenas + 48;                   //
    Decenas  = Decenas + 48;                    //
    Unidades = Unidades + 48;                   //
} 
// -------------------------- Concatenación------------------------------------

int concat(int a, int b) {
 
    char s1[20];                                // 
    char s2[20];                                //

    sprintf(s1, "%d", a);                       // Haciendolos una string
    sprintf(s2, "%d", b);                       // Haciendolos una string
    strcat(s1, s2);                             // Concatenando strings
    int c = atoi(s1);                           //
    return c;                                   //
}

// ----------------------- Configuración de I/O -------------------------------

void setup(void){
    
    ANSEL  = 0x00;                              //
    ANSELH = 0x00;                              //
    
    TRISA = 0x00;
    TRISBbits.TRISB0 = 1;                       // PB1
    TRISBbits.TRISB1 = 1;                       // PB2
    TRISD = 0x00;                               // Counter
       
// ------------------------- Limpieza de puertos ------------------------------
    
    PORTA = 0x00;                               //
    PORTB = 0x00;                               //
    PORTD = 0x00;                               //
    
// ------------------ Configuración de oscillador interno --------------------- 
    
    OSCCONbits.IRCF2 = 1;                       // 1
    OSCCONbits.IRCF1 = 1;                       // 1
    OSCCONbits.IRCF0 = 1;                       // 1 --> 8MHz
    OSCCONbits.SCS   = 1;                       // 
    
// ----------------------- Configuración de Pull-up ---------------------------
    
    OPTION_REGbits.nRBPU = 0;                   //
    WPUB = 0b00000011;                          //
    IOCBbits.IOCB0 = 1;                         // PB1 
    IOCBbits.IOCB1 = 1;                         // PB2
    
// ---------------------- Configuración de RX y TX ----------------------------
    
    TXSTAbits.SYNC = 0;                         //
    TXSTAbits.BRGH = 1;                         //
    BAUDCTLbits.BRG16 = 0;                      //
    SPBRG = 25;                                 //
    SPBRGH = 1;                                 //
    RCSTAbits.SPEN = 1;                         //
    RCSTAbits.RX9 = 0;                          //
    RCSTAbits.CREN = 1;                         //
    TXSTAbits.TX9 = 0;                          // 
    TXSTAbits.TXEN = 1;                         //
    PIR1bits.RCIF = 0;                          // RX flag
    PIR1bits.TXIF = 0;                          // TX flag
    
// -------------------- Configuración de interrupciones -----------------------
    
    INTCONbits.GIE = 1;                         // Global interrupt == 1
    INTCONbits.PEIE = 1;                        // Periferical interrupt == 1
    PIE1bits.RCIE = 0;                          // RX interrupt == 0 
    PIE1bits.TXIE = 0;                          // TX interrupt == 0
    INTCONbits.RBIF = 1;                        // 
    INTCONbits.RBIE = 1;                        // PORTB interrupt == 1

}