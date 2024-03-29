#include "LCD.h"

void LCD_Port(char a) {
    
    if (a & 1)
        D4 = 1;
    else
        D4 = 0;

    if (a & 2)
        D5 = 1;
    else
        D5 = 0;

    if (a & 4)
        D6 = 1;
    else
        D6 = 0;

    if (a & 8)
        D7 = 1;
    else
        D7 = 0;
}

void LCD_Cmd(char a) {
    
    RS = 0; 
    LCD_Port(a);
    EN = 1; 
    __delay_ms(4);
    EN = 0; 
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
        y = temp & 0x0F;
        LCD_Cmd(z);
        LCD_Cmd(y);
    }
}

void LCD_Init(void) {
    
    LCD_Port(0x00);
    __delay_ms(20);
    LCD_Cmd(0x03);
    __delay_ms(5);
    LCD_Cmd(0x03);
    __delay_ms(11);
    LCD_Cmd(0x03);
    LCD_Cmd(0x02);
    LCD_Cmd(0x02);
    LCD_Cmd(0x08);
    LCD_Cmd(0x00);
    LCD_Cmd(0x0C);
    LCD_Cmd(0x00);
    LCD_Cmd(0x06);
}

void LCD_Write_Char(char a) {
    
    char temp, y;
    temp = a & 0x0F;
    y = a & 0xF0;
    RS = 1; 
    LCD_Port(y >> 4); 
    EN = 1;
    __delay_us(40);
    EN = 0;
    LCD_Port(temp);
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