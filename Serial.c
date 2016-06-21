#include <REGx51.h>	
#include "Serial.h"

/// \file Serial.c Rotinas de comunicação UART através da porta serial.
/// Autor: Fábio Crestani<br>
/// Última modificação: 24/12/2014<br>
///

/////////////////////////////////////////////////////////////////////////////////
/// InicializaSerial.
/// Propósito: Inicializa registradores para conexão serial<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma
///
/////////////////////////////////////////////////////////////////////////////////
void InicializaSerial(){			  
	TMOD = 0x21;				// 00100001b
	TH1  = 243;					// valor inicial do timer 1 para 9600bps
	PCON = 0x80;				// 10000000b 
	SCON = 0x50;				// 01010000b
	TR1 = 1;					// liga timer 1
	EA  = 1;				    // Enable All Interrupts
}

/////////////////////////////////////////////////////////////////////////////////
/// RecebeCharSerial.
/// Propósito: Recebe um único char da porta serial configurada<br>
/// Entradas: Nenhuma<br>
/// Saídas: usigned char com o char recebido
///
/////////////////////////////////////////////////////////////////////////////////
unsigned char RecebeCharSerial(){
	if(RI){									// flag de recebido
		RI = 0;							  	// limpa flag de recebido
		return SBUF;						// retorna valor da serial
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
/// EnviaCharSerial.
/// Propósito: Envia um único char através da porta serial configurada<br>
/// Entradas: unsigned char c: char a ser escrito na serial<br>
/// Saídas: Nenhuma
///
/// \param [in] c caractere (char) a ser enviado para a porta serial
/////////////////////////////////////////////////////////////////////////////////
void EnviaCharSerial(unsigned char c){
	SBUF = c;								// escreve no buffer da serial
	while(TI == 0){}						// enquanto não enviou, aguarda
	TI = 0;									// limpa o flag de enviado serial
}

/////////////////////////////////////////////////////////////////////////////////
/// StrToSerial.
/// Propósito: Envia uma string através da porta serial usando EnviaCharSerial<br>
/// Entradas: unsigned char* string: string a ser enviada<br>
/// Saídas: Nenhuma
///
/// \param [in] *string vetor de caracteres a ser enviado para a porta serial
/////////////////////////////////////////////////////////////////////////////////
void StrToSerial(unsigned char *string){
	while(*string)              			// Percorre a string
       EnviaCharSerial(*string++);  		// Envia caractere por caractere
	EnviaCharSerial(ASCII_CR); 				// Envia quebra de linha
}

/////////////////////////////////////////////////////////////////////////////////
/// EnviaACKPC.
/// Propósito: Envia um pedido de acknowledge para o PC<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma
///
/////////////////////////////////////////////////////////////////////////////////
void EnviaACKPC(){
	unsigned int i, j, k;
	EnviaCharSerial(ASCII_ENQ);
	for(k=0; k<TIMEOUT_ENQ; k++)
		for(i=0;i<60;i++)					// Ajuste do tempo de delay
			for(j=0;j<255;j++);
}

/////////////////////////////////////////////////////////////////////////////////
/// EnviaTentativaPC.
/// Propósito: Envia informação de tentativa de acesso para o PC<br>
/// Entradas:<br>
/// Saídas: 
///
/////////////////////////////////////////////////////////////////////////////////
void EnviaTentativaPC(){	
	EnviaCharSerial('?');
}