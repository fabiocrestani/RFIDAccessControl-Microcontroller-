/// \file RTC.h Arquivo de cabeçalho do arquivo RTC.c.
/// Rotinas de controle do chip de relógio de tempo real DS1307.

// =======================================================================
// ROTINAS DE CONTROLE DO RTC (DS1307)
// =======================================================================
// Adaptado do site:
// http://exploreembedded.com/wiki/A6.8051_Interfacing:_RTC_DS1307
// 
// Última modificação: 17/02/2014
// =======================================================================

// =======================================================================
// Defines
	#define DS1307_ID      0xD0 // Endereço do DS1307
	#define DS1307_SEC     0x00 // Endereço do registrador de segundos
	#define DS1307_DATE    0x03 // Endereço do registrador de dia da semana
	#define DS1307_CONTROL 0x07 // Endereço do registrador de controle
// =======================================================================

// =======================================================================
// Protótipos
	void DS1307_Init();
	void DS1307_Write(unsigned char dat);
	unsigned char DS1307_Read();
	void DS1307_SetTime(unsigned char hh, unsigned char mm, unsigned char ss);
	void DS1307_SetDate(unsigned char dd, unsigned char mm, unsigned char yy, unsigned char semana);
	void DS1307_GetTime(unsigned char *h_ptr,unsigned char *m_ptr,unsigned char *s_ptr);
	void DS1307_GetDate(unsigned char *d_ptr,unsigned char *m_ptr,unsigned char *y_ptr, unsigned char *semana_ptr);
	void DS1307_GetDados_EnviaLCD();
	void DS1307_GetDados_EnviaSerial();
	char SWAP(char entrada);
// =======================================================================