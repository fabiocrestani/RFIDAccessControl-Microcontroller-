// =======================================================================
// ROTINAS DE SPI VIA BIT BANGING
// =======================================================================

/// \file SPI.c Rotinas de SPI via Bit Baning.
/// A versão atual não usa o periférico SPI do microcontrolador, mas sim
/// a técnica de big banging. MISO, MOSI e SCK podem ser configurados para
/// quaisquer pinos de portas disponíveis do microcontrolador.
/// Autor: Fábio Crestani
/// Última modificação: 28/12/2014 

#include <REGx51.h>	
#include "SPI.h"
#include "Serial.h"

/////////////////////////////////////////////////////////////////////////////////
/// DelaySPI.
/// Propósito: Implementa delay de até 255 * 0,5 [micro segundos]<br>
/// Entradas: i define o tempo do delay<br>
/// Saídas: Nenhuma
///
/// \param [in] i inteiro sem sinal com o tempo de delay (i * 0,5us)
/////////////////////////////////////////////////////////////////////////////////
void DelaySPI(unsigned int i){
	unsigned int j;
	for(j=0;j<i;j++);
}

/////////////////////////////////////////////////////////////////////////////////
/// SetMOSI.
/// Propósito: MOSI = 1<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma
///
/////////////////////////////////////////////////////////////////////////////////
void SetMOSI(){
	MOSI = 1;
}

/////////////////////////////////////////////////////////////////////////////////
/// ClearMOSI.
/// Propósito: MOSI = 0<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma
///
/////////////////////////////////////////////////////////////////////////////////
void ClearMOSI(){
	MOSI = 0;
}

/////////////////////////////////////////////////////////////////////////////////
/// SetSCK.
/// Propósito: SCK = 1<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma
///
/////////////////////////////////////////////////////////////////////////////////
void SetSCK(){
	SCK = 1;
}

/////////////////////////////////////////////////////////////////////////////////
/// ClearSCK.
/// Propósito: SCK = 0<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma
///
/////////////////////////////////////////////////////////////////////////////////
void ClearSCK(){
	SCK = 0;
}

/////////////////////////////////////////////////////////////////////////////////
/// GetMISO.
/// Propósito: Recebe estado do pino MISO<br>
/// Entradas: Nenhuma<br>
/// Saídas: char com o estado do pino MISO
///
/////////////////////////////////////////////////////////////////////////////////
char GetMISO(){
	char A;
	A = MISO;
	return A;
}

/////////////////////////////////////////////////////////////////////////////////
/// SPIWrite.
/// Propósito: Envia um caractere através do pino MOSI<br>
/// Entradas: char dado, dado que será enviado<br>
/// Saídas: Nenhuma
///
/// \param [in] dado char que será enviado através do pino MOSI
/////////////////////////////////////////////////////////////////////////////////
void SPIWrite(char dado){	  
	int i;
	for(i=7; i>=0; i--){				// para os 8 bits do caractere
		if(dado & (1 << i ))            // faz um AND com uma máscara 
										// que se move até percorrer os 8 bits
			SetMOSI();					// seta o pino
		else 						    // ou
			ClearMOSI();				// zera o pino

		DelaySPI(10);					// aguarda e gera pulsos de clock
		SetSCK();                       // para o dispositivo slave SPI
		DelaySPI(10);
		ClearSCK();
		DelaySPI(10);
	}
}

/////////////////////////////////////////////////////////////////////////////////
/// SPIRead.
/// Propósito: Lê um caractere serializado através do pino MISO<br>
/// Entradas: Nenhuma<br>
/// Saídas: char com o caractere lido do pino MISO
///
/////////////////////////////////////////////////////////////////////////////////
char SPIRead(){
	int i, b=0;
	
	for(i=7; i>=0; i--){				  // para os 8 bits do caractere
		SetSCK();						  // gera pulso de clock
		DelaySPI(10);                     // aguarda
		if(GetMISO()) b |= 1 << i ;		  // se o bit for alto, faz um OR
		DelaySPI(10);                     // lógico e seta o bit do resultado
		ClearSCK();						  // mais um pulso de clock
		DelaySPI(10);					  // aguarda
	}	
return b;
}