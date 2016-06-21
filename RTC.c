/// \file RTC.c Rotinas de comunicação via protocolo I2C.
/// Autor: XploreLabz (www.xplorelabz.com)<br>
/// Modificado por Fábio Crestani<br>
/// Última modificação: 17/02/2015<br>

#include <REGx51.h>	
#include "I2C.h"
#include "RTC.h"
#include "DisplayLCD.h" 
#include "Serial.h"
#include "RFID.h"

/////////////////////////////////////////////////////////////////////////////////
/// DS1307_Init.
/// Propósito: Inicializa o DS1307: Envia o endereço no DS1307 no barramento I2C
/// e escreve 0x00 no registrador de controle do DS1307<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma
///
/////////////////////////////////////////////////////////////////////////////////
void DS1307_Init(){
    I2C_Start();                 // Start I2C communication
    DS1307_Write(DS1307_ID);   	 // Connect to DS1307 by sending its ID on I2c Bus
    DS1307_Write(DS1307_CONTROL);// Select the Ds1307 ControlRegister to configure Ds1307
    DS1307_Write(0x00);        	 // Write 0x00 to Control register to disable SQW-Out
    I2C_Stop();                  // Stop I2C communication after initilizing DS1307
}

/////////////////////////////////////////////////////////////////////////////////
/// DS1307_Write.
/// Propósito: Envia dados para o DS1307<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma
///
/////////////////////////////////////////////////////////////////////////////////
void DS1307_Write(unsigned char dat){
	I2C_Write(dat);
    I2C_Clock();
}
 
/////////////////////////////////////////////////////////////////////////////////
/// DS1307_Read.
/// Propósito: Lê dados do DS1307<br>
/// Entradas: Nenhuma<br>
/// Saídas: Retorna um char com o dado lido.
///
/////////////////////////////////////////////////////////////////////////////////
unsigned char DS1307_Read(){
    unsigned char dat;

    dat = I2C_Read();
    return(dat);
}
 
/////////////////////////////////////////////////////////////////////////////////
/// DS1307_SetTime.
/// Propósito: Escreve hora, minuto e segundo nos registradores do DS1307<br>
/// Entradas: unsigned char hh, unsigned char mm, unsigned char ss<br>
/// Saídas: Nenhuma.
///
/// \param [in] hh hora em formato BCD
/// \param [in] mm minuto em formato BCD
/// \param [in] ss segundo em formato BCD
/////////////////////////////////////////////////////////////////////////////////
void DS1307_SetTime(unsigned char hh, unsigned char mm, unsigned char ss){
    I2C_Start();              // Start I2C communication
 
    DS1307_Write(DS1307_ID);  // Connect to DS1307 by sending its ID on I2c Bus
    DS1307_Write(DS1307_SEC); // Select the SEC RAM address
 
    DS1307_Write(ss);         // Write sec  on RAM address 00H
    DS1307_Write(mm);         // Write min  on RAM address 01H
    DS1307_Write(hh);         // Write hour on RAM address 02H
 
    I2C_Stop();               // Stop I2C communication after Setting the Time
}

/////////////////////////////////////////////////////////////////////////////////
/// DS1307_SetDate.
/// Propósito: Escreve hora, minuto e segundo nos registradores do DS1307<br>
/// Entradas: unsigned char dd, unsigned char mm, unsigned char yy, unsigned char semana<br>
/// Saídas: Nenhuma.
///
/// \param [in] dd dia em formato BCD
/// \param [in] mm mês em formato BCD
/// \param [in] yy em formato BCD (apenas os últimos 2 dígitos do ano)
/// \param [in] semana dia da semana de 0 a 6. Convecionado que 1 corresponde à segunda-feira.
/////////////////////////////////////////////////////////////////////////////////
void DS1307_SetDate(unsigned char dd, unsigned char mm, unsigned char yy, unsigned char semana){
    I2C_Start();            	// Start I2C communication
 
    DS1307_Write(DS1307_ID);    // Connect to DS1307 by sending its ID on I2c Bus
    DS1307_Write(DS1307_DATE);  // Request DAY RAM address at 04H
 
 	DS1307_Write(semana);		// Escreve dia da semana
    DS1307_Write(dd);           // Write date on RAM address 04H
    DS1307_Write(mm);           // Write month on RAM address 05H
    DS1307_Write(yy);           // Write year on RAM address 06h
 
    I2C_Stop();             	// Stop I2C communication after Setting the Date
}
  
/////////////////////////////////////////////////////////////////////////////////
/// DS1307_GetTime.
/// Propósito: Recebe hora, minuto e segundo do RTC.<br>
/// Entradas: unsigned char *h_ptr,unsigned char *m_ptr,unsigned char *s_ptr<br>
/// Saídas: Nenhuma.
///
/// \param [out] *h_ptr ponteiro para char de hora
/// \param [out] *m_ptr ponteiro para char de minuto
/// \param [out] *s_ptr ponteiro para char segundo
/////////////////////////////////////////////////////////////////////////////////
void DS1307_GetTime(unsigned char *h_ptr,unsigned char *m_ptr,unsigned char *s_ptr){
    I2C_Start();           		// Start I2C communication
    DS1307_Write(DS1307_ID);    // Connect to DS1307 by sending its ID on I2c Bus
    DS1307_Write(DS1307_SEC);   // Request Sec RAM address at 00H
    I2C_Stop();            		// Stop I2C communication after selecting Sec Register
 
    I2C_Start();                // Start I2C communication
    DS1307_Write(0xD1);         // Connect to DS1307( under Read mode)by sending its ID on I2c Bus
	 
	*s_ptr = DS1307_Read();     // Read second 
	I2C_Ack();     				// Return Positive ACK
 	*m_ptr = DS1307_Read();  	// Read minute
	I2C_Ack();      			// Return Positive ACK
 	*h_ptr = DS1307_Read();     // Read hour
	I2C_NoAck();   				// Return Negative/No ACK
 
  	I2C_Stop();                 // Stop I2C communication after reading the Time
}

/////////////////////////////////////////////////////////////////////////////////
/// DS1307_GetDate.
/// Propósito: Recebe dia, mês, ano e dia da semana do RTC.<br>
/// Entradas: unsigned char *d_ptr,unsigned char *m_ptr,unsigned char *y_ptr, unsigned char *semana_ptr<br>
/// Saídas: Nenhuma.
///
/// \param [out] *d_ptr ponteiro para char de dia
/// \param [out] *m_ptr ponteiro para char de mês
/// \param [out] *y_ptr ponteiro para char de anos
/// \param [out] *semana_ptr ponteiro para char de dia da semana. De 0 a 6. 1 corresponde à segunda-feira.
/////////////////////////////////////////////////////////////////////////////////
void DS1307_GetDate(unsigned char *d_ptr,unsigned char *m_ptr,unsigned char *y_ptr, unsigned char *semana_ptr){
	I2C_Start();            	// Start I2C communication
    DS1307_Write(DS1307_ID);    // connect to DS1307 by sending its ID on I2c Bus
    DS1307_Write(DS1307_DATE);  // Request DAY RAM address at 04H
    I2C_Stop();             	// Stop I2C communication after selecting DAY Register
 
    I2C_Start();             	// Start I2C communication
    DS1307_Write(0xD1);         // connect to DS1307( under Read mode)
	*semana_ptr = DS1307_Read();// Lê dia da semana
	I2C_Ack();     				// Return Positive ACK
    *d_ptr = DS1307_Read(); 	// Read day
	I2C_Ack();     				// Return Positive ACK
    *m_ptr = DS1307_Read(); 	// Read month
	I2C_Ack();     				// Return Positive ACK
    *y_ptr = DS1307_Read(); 	// Read year
	I2C_NoAck();   				// Return Negative/No ACK
 
    I2C_Stop();         		// Stop I2C communication after reading the Time
}

/////////////////////////////////////////////////////////////////////////////////
/// DS1307_GetDados_EnviaLCD.
/// Propósito: Obtém dados atualizados de data e hora e exibe no display de lcd
/// no formato dd/mm/aaaa hh:mm:ss  codificados em ASCII
/// Entradas: Nenhuma.<br>
/// Saídas: Nenhuma.
///
/////////////////////////////////////////////////////////////////////////////////
void DS1307_GetDados_EnviaLCD(){
	char hour, min, sec, day, month, year, semana;

	DS1307_GetDate(&day,&month,&year,&semana);

	XYDisplay(0,0);
	EscDadoDisplay((SWAP(day) & 0x0F) + 0x30);
	EscDadoDisplay((day & 0x0F) + 0x30);
	EscDadoDisplay('/');
	EscDadoDisplay((SWAP(month) & 0x0F) + 0x30);
	EscDadoDisplay((month & 0x0F) + 0x30);
	EscDadoDisplay('/');
	EscDadoDisplay('2');
	EscDadoDisplay('0');
	EscDadoDisplay((SWAP(year) & 0x0F) + 0x30);
	EscDadoDisplay((year & 0x0F) + 0x30);

	DS1307_GetTime(&hour,&min,&sec);
	
	XYDisplay(1,0);	
    EscDadoDisplay((SWAP(hour) & 0x0F) + 0x30);
    EscDadoDisplay((hour & 0x0F) + 0x30);
	EscDadoDisplay(':');
	EscDadoDisplay((SWAP(min) & 0x0F) + 0x30);
	EscDadoDisplay((min & 0x0F) + 0x30);
	EscDadoDisplay(':');		 
	EscDadoDisplay((SWAP(sec) & 0x0F) + 0x30);
	EscDadoDisplay((sec & 0x0F) + 0x30);


}

/////////////////////////////////////////////////////////////////////////////////
/// DS1307_GetDados_EnviaSerial.
/// Propósito: Obtém dados atualizados de data e hora e os envia através da porta 
/// serial no formato dd/mm/aaaa hh:mm:ss codificados em ASCII
/// Entradas: Nenhuma.<br>
/// Saídas: Nenhuma.
///
/////////////////////////////////////////////////////////////////////////////////
void DS1307_GetDados_EnviaSerial(){
 	char hour, min, sec, day, month, year, semana;

	DS1307_GetDate(&day,&month,&year,&semana);
	DelayRFID(1);
	DS1307_GetTime(&hour,&min,&sec);

	EnviaCharSerial((SWAP(day) & 0x0F) + 0x30);
	EnviaCharSerial((day & 0x0F) + 0x30);
	EnviaCharSerial('/');
	EnviaCharSerial((SWAP(month) & 0x0F) + 0x30);
	EnviaCharSerial((month & 0x0F) + 0x30);
	EnviaCharSerial('/');
	EnviaCharSerial('2');
	EnviaCharSerial('0');
	EnviaCharSerial((SWAP(year) & 0x0F) + 0x30);
	EnviaCharSerial((year & 0x0F) + 0x30);	

	EnviaCharSerial(' ');

    EnviaCharSerial((SWAP(hour) & 0x0F) + 0x30);
    EnviaCharSerial((hour & 0x0F) + 0x30);
	EnviaCharSerial(':');
	EnviaCharSerial((SWAP(min) & 0x0F) + 0x30);
	EnviaCharSerial((min & 0x0F) + 0x30);
	EnviaCharSerial(':');		 
	EnviaCharSerial((SWAP(sec) & 0x0F) + 0x30);
	EnviaCharSerial((sec & 0x0F) + 0x30);
}

/////////////////////////////////////////////////////////////////////////////////
/// SWAP.
/// Propósito: Implementa a instrução SWAP através de operadores do C.
/// Entradas: Char.<br>
/// Saídas: Char com o nibbles permutados
///
/// \param [in] entrada char a ser permutado os nibbles
/////////////////////////////////////////////////////////////////////////////////
char SWAP(char entrada){
	return ((entrada & 0x0F) << 4) | ((entrada & 0xF0) >> 4);
}