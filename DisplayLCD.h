/// \file DisplayLCD.h Arquivo de cabe�alho do arquivo DisplayLCD.c.
/// Este arquivo re�ne rotinas de tratamento do display de cristal l�quido
/// de 16 colunas e 2 linhas. E al�m disso cont�m fun��es para acionar
/// ou n�o os Leds da placa P51.

// =======================================================================
// Rotinas de tratamento do display de cristal l�quido 16x2
// =======================================================================
// Autor: F�bio Crestani
// �ltima modifica��o: 24/12/2014
// Baseadas no site http://www.8051projects.net/lcd-interfacing/
// e no arquivo LCD_RW_P51_2014-2.a51
// =======================================================================

// =======================================================================
// Defines
	 #define LCD_data P0					///< Dados do LCD (Porta P0)
	 #define LCD_D7   P0_7 					///< Busy flag do LCD
	 #define LCD_rs   P2_5					///< R/S do LCD
	 #define LCD_rw   P2_6					///< Read/Write do LCD
	 #define LCD_en   P2_7					///< LCD Enable
// =======================================================================

// =======================================================================
// Prot�tipos
	// Display LCD
	void InicializaDisplay();
	void LCD_busy();
	void EscComandoDisplay(unsigned char var);
	void EscDadoDisplay(unsigned char dado);
	void EscStringDisplay(unsigned char *string);
	void EscStringDisplay2Linha(unsigned char *string);
	void LimpaDisplay();
	void XYDisplay(int x, int y);
	void PiscaString2Linha(unsigned char *string, unsigned int n, bit beep);
// =======================================================================