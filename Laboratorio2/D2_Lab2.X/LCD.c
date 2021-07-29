#include <xc.h> 
#include <stdint.h>  
#include <pic16f887.h> 
#include "LCD.h"

void LCD_Port(char PORTD_Value) {
    PORTD = PORTD_Value;                // Toma el valor del PORTD

}

void LCD_Cmd(char a) {
    RS = 0; // => RS = 0 
    Lcd_Port(a);
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0
}

void LCD_Clear(void) {
    LCD_Cmd(0);
    LCD_Cmd(1);
}

void LCD_Set_Cursor(char a, char b) {
    char temp, z, y;
    if (a == 1) {
        temp = 0x80 + b - 1;
        z = temp >> 4;
        y = temp & 0x0F;
        LCD_Cmd(z);
        
        LCD_Cmd(y);
    } else if (a == 2) {
        temp = 0xC0 + b - 1;
        z = temp >> 4;
        y = temp & 0xF0; 
        LCD_Cmd(z);
        LCD_Cmd(y);
    }
}

void LCD_Init(void) { 
    LCD_Port(0x00);
    __delay_ms(20);
    LCD_Cmd(0x30); 
    __delay_ms(5);
    LCD_Cmd(0x30); 
    __delay_us(200);
    LCD_Cmd(0x30); 

    LCD_Cmd(0x3C); 

    LCD_Cmd(0x08); 

    LCD_Cmd(0x01);

    LCD_Cmd(0x06); 
}

void LCD_Write_Char(char a) {
    char temp, y;
    temp = a;
    y = temp;
    RS = 1;
    LCD_Port(y);
    EN = 1;
    __delay_us(40);
    EN = 0;
}

void LCD_Write_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++)
        LCD_Write_Char(a[i]);
}

void LCD_Shift_Right(void) {
    LCD_Cmd(0x01);
    LCD_Cmd(0x0C);
}

void LCD_Shift_Left(void) {
    LCD_Cmd(0x01);
    LCD_Cmd(0x08);
}
