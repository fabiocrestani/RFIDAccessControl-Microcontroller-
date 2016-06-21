/// \file I2C.h Arquivo de cabe�alho do arquivo I2C.c.
/// Rotinas de comunica��o via protocolo I2C.

// =======================================================================
// ROTINAS DE I2C
// =======================================================================
// Adaptado do site:
// http://www.8051projects.net/i2c-twi-tutorial/8051-i2c-implementation.php
// 
// �ltima modifica��o: 05/01/2014
// =======================================================================

// =======================================================================
// Defines
	sbit SCL=0xC0;      //SCL Connected to P4.0
	sbit SDA=0xC1;      //SDA Connected to P4.1
// =======================================================================

// =======================================================================
// Prot�tipos
	void delay_us(unsigned int us_count);
	void I2C_Clock(void);
	void I2C_Start();
	void I2C_Stop(void);
	void I2C_Write(unsigned char dat);
	unsigned char I2C_Read(void);
	void I2C_Ack();
	void I2C_NoAck();
// =======================================================================