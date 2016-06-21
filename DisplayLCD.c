#include <REGx51.h>	
#include "DisplayLCD.h"

/// \file DisplayLCD.c Rotinas de tratamento do display de cristal l�quido de 16 colunas e 2 linhas.
/// Autor: F�bio Crestani<br>
/// �ltima modifica��o: 24/12/2014<br>
/// Baseado no site http://www.8051projects.net/lcd-interfacing/
/// e no arquivo LCD_RW_P51_2014-2.a51
///

/////////////////////////////////////////////////////////////////////////////////
/// InicializaDisplay.
/// Prop�sito: Limpa o display, seta o modo de cursor e posiciona no in�cio<br>
/// Entradas: Nenhuma<br>
/// Sa�das: Nenhuma
///
/////////////////////////////////////////////////////////////////////////////////
void InicializaDisplay(){
		EscComandoDisplay(0x38);				// 8-bit, 5x7, 1� linha
		EscComandoDisplay(0x38);				// 8-bit, 5x7, 2� linha
		EscComandoDisplay(0x06);  				// Entry mode
		EscComandoDisplay(0x0C);  				// N�o mostra cursor
		EscComandoDisplay(0x01);  				// Limpa o display
}	

/////////////////////////////////////////////////////////////////////////////////
/// LCD_Busy.
/// Prop�sito: Aguarda a busy flag do display LCD ser zerada<br>
/// Entradas: Nenhuma<br>
/// Sa�das: Nenhuma
///
/////////////////////////////////////////////////////////////////////////////////
void LCD_busy(){						 
     LCD_D7   = 1;           							// Seta busy flag
     LCD_en   = 1;          						  	// LCD Enable
     LCD_rs   = 0;           							// RS = 0
     LCD_rw   = 1;           							// RW = 1 (leitura)
														//
     while(LCD_D7){          							// Enquanto busy flag estiver habilitada
           LCD_en   = 0;     							// Desabilita LCD
           LCD_en   = 1;     							// Habilita LCD
     }										   			//
}

/////////////////////////////////////////////////////////////////////////////////
/// EscComandoDisplay.
/// Prop�sito: Envia um comando ao display<br>
/// Entradas: char comando<br>
/// Sa�das: Nenhuma
///
/// \param [in] comando comando a ser enviado
/////////////////////////////////////////////////////////////////////////////////
void EscComandoDisplay(unsigned char comando){
     LCD_data = comando;  							  // Escreve o comando na porta
     LCD_rs   = 0;        							  // Registrador de instru��es
     LCD_rw   = 0;        							  // Modo de escrita
     LCD_en   = 1;        							  // Habilita
	 LCD_en   = 0;        						  	  // Desabilita
	 LCD_busy();          						  	  // Aguarda busy flag
}													  //

/////////////////////////////////////////////////////////////////////////////////
/// EscDadoDisplay.
/// Prop�sito: Escreve um dado ASCII no display<br>
/// Entradas: char dado<br>
/// Sa�das: Nenhuma
///
/// \param [in] dado dado  a ser enviado
/////////////////////////////////////////////////////////////////////////////////
void EscDadoDisplay(unsigned char dado){
     LCD_data = dado;     						    // Dado a ser enviado
     LCD_rs   = 1;        						    // RS = 1
     LCD_rw   = 0;        						    // RW = 0 (escrita)
     LCD_en   = 1;        						    // Habilita	
     LCD_en   = 0;									// Desabilita
     LCD_busy();          						    // Aguarda busy flag
}													//

/////////////////////////////////////////////////////////////////////////////////
/// EscStringDisplay.
/// Prop�sito: Escreve uma string no display<br>
/// Entradas: char* string<br>
/// Sa�das: Nenhuma
///
/// \param [in] *string pointeiro para vetor (string) a ser enviado
/////////////////////////////////////////////////////////////////////////////////
void EscStringDisplay(unsigned char *string){
     while(*string)              				// Percorre a string
       EscDadoDisplay(*string++);  				// Envia caractere por caractere
}

/////////////////////////////////////////////////////////////////////////////////
/// EscStringDisplay2Linha.
/// Prop�sito: Escreve uma string na segunda linha do display
/// Entradas: char* string
/// Sa�das: Nenhuma
///
/// \param [in] *string pointeiro para vetor (string) a ser enviado
/////////////////////////////////////////////////////////////////////////////////
void EscStringDisplay2Linha(unsigned char *string){
	XYDisplay(1,0);							 // Posiciona cursor 2a linha
	EscStringDisplay("                ");	 // Limpa 2a linha
	XYDisplay(1,0);						     // Reposiciona cursor 2a linha
	EscStringDisplay(string);				 // Escreve dado
	XYDisplay(0,0);							 // Posiciona cursor 1a linha
}

/////////////////////////////////////////////////////////////////////////////////
/// LimpaDisplay.
/// Prop�sito: Limpa todo o conte�do do display
/// Entradas: Nenhuma
/// Sa�das: Nenhuma
///
/////////////////////////////////////////////////////////////////////////////////
void LimpaDisplay(){
	EscComandoDisplay(1);
}									 

/////////////////////////////////////////////////////////////////////////////////
/// XYDisplay.
/// Prop�sito: Move o cursor para a posi��o XY do display
/// Entradas: x: linha, y: coluna
/// Sa�das: Nenhuma
///
/// \param [in] x linha do cursor (0 ou 1)
/// \param [in] y coluna do cursor (0 a 15)
/////////////////////////////////////////////////////////////////////////////////
void XYDisplay(int x, int y){ 
	unsigned int A;							//
	if(x==0) A = 0x80; else A = 0xC0;		// Primeira ou segunda linha
	A |= y;									// A = A OR y
	EscComandoDisplay(A);					// Envia comando
}

/////////////////////////////////////////////////////////////////////////////////
/// PiscaString2Linha.
/// Prop�sito: Escreve uma string na segunda linha do display e pisca n vezes
/// Entradas: char* string, unsigned int n
/// Sa�das: Nenhuma
///
/// \param [in] *string ponteiro para o vetor (string) a ser enviado
/// \param [in] n n�mero de vezes para piscar a string no display
/// \param [in] beep: se for 1, soa um beep enquanto pisca
/////////////////////////////////////////////////////////////////////////////////
void PiscaString2Linha(unsigned char *string, unsigned int n, bit beep){
	unsigned int i, j, k;

	for(i=0;i<n;i++){
		EscStringDisplay2Linha(string);		  			// Escreve a string
		if(beep) P1_3 = 1;
		for(j=0;j<255;j++) for(k=0;k<255;k++);			// Delay
		EscStringDisplay2Linha("                ");	    // Limpa a linha
		if(beep) P1_3 = 0;
		for(j=0;j<255;j++) for(k=0;k<255;k++);		    // Delay
	}
}