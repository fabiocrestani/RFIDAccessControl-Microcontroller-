/// \file I2C.h Arquivo de cabeçalho do arquivo I2C.c.
/// Rotinas de comunicação via protocolo I2C.

// =======================================================================
// ROTINAS DE I2C
// =======================================================================
// Adaptado do site:
// http://www.8051projects.net/i2c-twi-tutorial/8051-i2c-implementation.php
// 
// Última modificação: 05/01/2014
// =======================================================================

// =======================================================================
// Defines
	sbit SCL=0xC0;      //SCL Connected to P4.0
	sbit SDA=0xC1;      //SDA Connected to P4.1
// =======================================================================

// =======================================================================
// Protótipos
	void delay_us(unsigned int us_count);
	void I2C_Clock(void);
	void I2C_Start();
	void I2C_Stop(void);
	void I2C_Write(unsigned char dat);
	unsigned char I2C_Read(void);
	void I2C_Ack();
	void I2C_NoAck();
// =======================================================================