/// \file I2C.c Rotinas de comunicação via protocolo I2C.
/// Autor: XploreLabz (www.xplorelabz.com)<br>
/// Modificado por Fábio Crestani<br>
/// Última modificação: 17/02/2015<br>

#include <REGx51.h>	
#include "I2C.h"

/////////////////////////////////////////////////////////////////////////////////
/// delay_us.
/// Propósito: Implementa um delay simples para as rotinas deste arquivo<br>
/// Entradas: Tempo de delay<br>
/// Saídas: Nenhuma
///
/// \param [in] us_count tempo de delay
/////////////////////////////////////////////////////////////////////////////////
void delay_us(unsigned int us_count){  
	while(us_count!=0) us_count--;
}
 
/////////////////////////////////////////////////////////////////////////////////
/// I2C_Clock.
/// Propósito: Gera o sinal de clock<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma
///
/////////////////////////////////////////////////////////////////////////////////
void I2C_Clock(void){
	delay_us(1);
    SCL = 1;
    delay_us(1);
    SCL = 0;
}
 
/////////////////////////////////////////////////////////////////////////////////
/// I2C_Start.
/// Propósito: Gera o start bit do I2C<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma
///
/////////////////////////////////////////////////////////////////////////////////
void I2C_Start(){
    SCL = 0;        // Pull SCL low 
    SDA = 1;        // Pull SDA High
    delay_us(1);
    SCL = 1;        // Pull SCL high
    delay_us(1);
    SDA = 0;        // Now Pull SDA LOW, to generate the Start Condition
    delay_us(1); 
    SCL = 0;        // Finally Clear the SCL to complete the cycle
}
  
/////////////////////////////////////////////////////////////////////////////////
/// I2C_Stop.
/// Propósito: Gera o stop bit do I2C<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma
///
///////////////////////////////////////////////////////////////////////////////// 
void I2C_Stop(void){
    SCL = 0;            // Pull SCL low
    delay_us(1);
    SDA = 0;            // Pull SDA  low
    delay_us(1);
    SCL = 1;            // Pull SCL High
    delay_us(1);
    SDA = 1;            // Now Pull SDA High, to generate the Stop Condition
}

/////////////////////////////////////////////////////////////////////////////////
/// I2C_Write.
/// Propósito: Envia 1 byte via I2C. MSB é enviado primeiro.<br>
/// Entradas: 1 byte (char) a ser enviado<br>
/// Saídas: Nenhuma
///
/// \param [in] dat char a ser enviado
///////////////////////////////////////////////////////////////////////////////// 
void I2C_Write(unsigned char dat){
    unsigned char i;
 
	for(i=0;i<8;i++){        // Loop 8 times to send 1-byte of data
    	SDA = dat & 0x80;    // Send Bit by Bit on SDA line
        I2C_Clock();         // Generate Clock at SCL
        dat = dat<<1;		 // Shift para enviar próximo bit
    }
    
	SDA = 1;                 // Set SDA at last
}

/////////////////////////////////////////////////////////////////////////////////
/// I2C_Read.
/// Propósito: Recebe 1 byte via I2C. MSB é recebido primeiro.<br>
/// Entradas: Nenhuma<br>
/// Saídas: Retorna char lido
///
///////////////////////////////////////////////////////////////////////////////// 
unsigned char I2C_Read(void){
	unsigned char i, dat=0x00;
 
    SDA=1;                  // Make SDA as I/P
    for(i=0;i<8;i++){       // Loop 8times to read 1-byte of data  
    	
		delay_us(1);
        SCL = 1;            // Pull SCL High
        delay_us(1);
 
        dat = dat<<1;       // dat is shifted each time and
        dat = dat | SDA;    // ORed with the received bit to pack into byte
        SCL = 0;            // Clear SCL to complete the Clock
       }

   return dat;              // Finally return the received Byte
}
 
/////////////////////////////////////////////////////////////////////////////////
/// I2C_Ack.
/// Propósito: Gera um ACK no barramento I2C.
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma
///
///////////////////////////////////////////////////////////////////////////////// 
void I2C_Ack(){
    SDA = 0;        // Pull SDA low to indicate Positive ACK
    I2C_Clock();    // Generate the Clock
    SDA = 1;        // Pull SDA back to High(IDLE state)
}
 
/////////////////////////////////////////////////////////////////////////////////
/// I2C_NoAck.
/// Propósito: Gera um NACK no barramento I2C.
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma
///
///////////////////////////////////////////////////////////////////////////////// 
void I2C_NoAck(){
    SDA = 1;        // Pull SDA high to indicate Negative/NO ACK
    I2C_Clock();    // Generate the Clock  
    SCL = 1;        // Set SCL */
}