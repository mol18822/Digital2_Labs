#include "I2C.h"

// ------------------------ Iniciando MASTER PIC ------------------------------ 

void I2C_Master_Init(const unsigned long c) {
    
    SSPCON = 0b00101000;
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ/(4*c))-1;
    SSPSTAT = 0;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}

// ------------------------ Send/Receive Data ---------------------------------

void I2C_Master_Wait() {
    
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

// --------------------- Initialize de comunicación I2C -----------------------

void I2C_Master_Start() {
    
    I2C_Master_Wait();      
    SSPCON2bits.SEN = 1;    
}

// --------------------- Reset de comunicación I2C ----------------------------

void I2C_Master_RepeatedStart() {
    
    I2C_Master_Wait();     
    SSPCON2bits.RSEN = 1;   
}

// ------------------------ Stop I2C comunicación -----------------------------

void I2C_Master_Stop() {
    
    I2C_Master_Wait();      
    SSPCON2bits.PEN = 1;    
}

// ---------------- Transmisión de datos de Master a Slave --------------------

void I2C_Master_Write(unsigned d) {
    
    I2C_Master_Wait();      
    SSPBUF = d;             
}

// ---------------- Transmisión de datos de Slave a Master --------------------

unsigned short I2C_Master_Read(unsigned short a) {
    
    unsigned short temp;
    I2C_Master_Wait();     
    SSPCON2bits.RCEN = 1;
    I2C_Master_Wait();     
    temp = SSPBUF;
    I2C_Master_Wait();      
    if(a == 1){
        SSPCON2bits.ACKDT = 0;
    }else{
        SSPCON2bits.ACKDT = 1;
    }
    SSPCON2bits.ACKEN = 1;         
    return temp;                   
}

// ----------------------- Iniciando I2C Slave --------------------------------

void I2C_Slave_Init(uint8_t address) {
    
    SSPADD = address;
    SSPCON = 0x36;     
    SSPSTAT = 0x80;     
    SSPCON2 = 0x01;    
    TRISC3 = 1;
    TRISC4 = 1;
    GIE = 1;
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
}