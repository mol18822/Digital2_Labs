#ifndef LCD_H
#define	LCD_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#ifndef RS
#define RS RE0
#endif

#ifndef EN
#define EN RE1
#endif

#ifndef D4
#define D4 RC2
#endif

#ifndef D5
#define D5 RC1
#endif

#ifndef D6
#define D6 RC0
#endif

#ifndef D7
#define D7 RC5
#endif

#include <xc.h> 

void LCD_Port(char a);

void LCD_Cmd(char a);

void LCD_Clear(void);

void LCD_Set_Cursor(char a, char b);

void LCD_Init(void);

void LCD_Write_Char(char a);

void LCD_Write_String(char *a);

void LCD_Shift_Right(void);

void LCD_Shift_Left(void);

#endif	
