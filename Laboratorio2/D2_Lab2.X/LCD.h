// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_librery
#define	LCD_librery

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

// ------------------ Definición de puertos y sus funciones -------------------

#ifndef RS
#define RS PORTEbits.RE0 //se quita el PORTDbits para que no los tome todos en ese puerto
#endif

#ifndef EN
#define EN PORTEbits.RE2
#endif

#ifndef D0
#define D0 PORTDbits.RD0
#endif

#ifndef D1
#define D1 PORTDbits.RD1
#endif

#ifndef D2
#define D2 PORTDbits.RD2
#endif

#ifndef D3
#define D3 PORTDbits.RD3
#endif

#ifndef D4
#define D4 PORTDbits.RD4
#endif

#ifndef D5
#define D5 PORTDbits.RD5
#endif

#ifndef D6
#define D6 PORTDbits.RD6
#endif

#ifndef D7
#define D7 PORTDbits.RD7
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