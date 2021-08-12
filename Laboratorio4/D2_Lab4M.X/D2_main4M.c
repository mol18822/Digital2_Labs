// Dispositivo:		PIC16F887
// Autor;			Luis Pedro Molina Velásquez 
// Carné;			18822
// Compilador:		pic-as (v2.31) MPLABX V5.40
// Curso:           Electrónica Digital II
// ----------------------------------------------------------------------------     
// ----------------------- Laboratorio No. 4  ---------------------------------      
// ----------------------------- I2C ------------------------------------------
// -------------------------- MASTER PIC --------------------------------------

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
#include <string.h> 
#include <stdlib.h>
#include "I2C.h"
#include "LCD.h"

// ---------------------- Declaración de puertos ------------------------------

#define RS RE0                              //
#define EN RE1                              //
#define D4 RC2                              //
#define D5 RC1                              //
#define D6 RC0                              //
#define D7 RC5                              //

// ----------------------------- Variables ------------------------------------

volatile char temperature = 0;              //
volatile uint8_t POT = 0;                   //
char ADC0[10];                              //
char ADC1[10];                              //
char contador[10];                          //
float counter = 0;                          //
float Pot_conversor = 0;                    //
char Temp_conversor;                        //       
char Cont_conversor;                        //
char Centenas;                              //       
char Decenas;                               // 
char Unidades;                              // 
char Residuo;                               // 
char value;                                 //

// ----------------------------- Prototipos -----------------------------------

void setup(void);
void ADC_convert(char *data,float a, int place); 
char division (char value);

// ------------------------- Interrupciones -----------------------------------

void __interrupt() isr(void)
{
     
}

// -------------------------------- Main --------------------------------------

void main(void) {
    
    setup();                                //                                    
    LCD_Init();                             //
    LCD_Clear();                            //
    
// -------------------------------- Loop --------------------------------------      
    
    while(1) {
        
        I2C_Master_Start();             
        I2C_Master_Write(0x51); 
        PORTB = I2C_Master_Read(0); 
        I2C_Master_Stop();
        __delay_ms(200);

        I2C_Master_Start();
        I2C_Master_Write(0x80); 
        I2C_Master_Write(0xF3); 
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0x81); 
        PORTD = I2C_Master_Read(0); 
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0x91); 
        PORTA = I2C_Master_Read(0); 
        I2C_Master_Stop();
        __delay_ms(200);
        
        counter = PORTA;
        POT = PORTB;
        temperature = PORTD;
        
        LCD_Clear();
        LCD_Set_Cursor(1,2);
        LCD_Write_String("S1");
        LCD_Set_Cursor(1,8);
        LCD_Write_String("S2");
        LCD_Set_Cursor(1,14);
        LCD_Write_String("S3");
        __delay_ms(1000);
        
        LCD_Set_Cursor(2,1);
        LCD_Write_String(ADC0);
        LCD_Set_Cursor(2,4);
        LCD_Write_String("V");
        LCD_Set_Cursor(2,7);
        LCD_Write_String(ADC1);
        LCD_Set_Cursor(2,11);
        LCD_Write_String("'");
        LCD_Set_Cursor(2,14);
        LCD_Write_String(contador);
        __delay_ms(2000);
    

    Temp_conversor = temperature - 77 ;
  
    Pot_conversor = 0;
      
    Pot_conversor = (POT / (float) 255)*5; 
                                         
    ADC_convert(ADC0, Pot_conversor, 2);

    ADC_convert(ADC1, Temp_conversor, 2);
    
    Cont_conversor = division(counter);   
    
    ADC_convert(contador, Cont_conversor, 2); 
    
    }
    return;
}

// ----------------------------- División -------------------------------------

char division (char value) {
    Centenas = value/50;                        // X/100, guarda entero
    Residuo  = value%100;                       // Residuo
    Decenas  = Residuo/10;                      // X/10, guarda entero
    Unidades = Residuo%10;                      // Residuo 
}    

// -------------------------- Conversión de ADC -------------------------------

void ADC_convert(char *data,float a, int place) {
    
     int temp=a;
     float x=0.0;
     int character=0;
     int i=0,mu=1;
     int j=0;
     if(a<0) {
        a=a*-1;
        data[i]='-';
        i++;
    }
     
     while(temp!=0) {
        temp=temp/10;
        character++;          
     }
     
     while(character!=0) {
        if(character==1)mu=1;
        else  for(j=2;j<=character;j++)mu=mu*10;
         
        x=a/mu;
        a=a-((int)x*mu);
        data[i]=0x30+((int)x);
        i++;
        character--;
        mu=1;
     }

    data[i]='.';
    i++;
    character=0;
    for(j=1;j<=place;j++)mu=mu*10;
    x=(a-(int)a)*mu; 
    a=x;
    temp=a;
    x=0.0;
    mu=1;
    character=place;
    
    while(character!=0) {
        if(character==1)mu=1;
        else  for(j=2;j<=character;j++)mu=mu*10;
         
        x=a/mu;
        a=a-((int)x*mu);
        data[i]=0x30+((int)x);
        i++;
        character--;
        mu=1;
    }   
     
    data[i]='\n';
}

// ----------------------- Configuración de I/O -------------------------------

void setup(void){
    
    ANSEL = 0;
    ANSELH = 0;
    
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
    
    TRISB = 0;
    TRISD = 0;
    TRISC = 0;
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
   
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

 
// --------------------- Configuración de I2C Master --------------------------
    
    I2C_Master_Init(100000);        // Inicializar Comuncaci?n I2C
}



