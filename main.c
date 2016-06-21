/*! \mainpage Documentação do programa para o microcontrolador Atmel AT89C5131A.
 * 
 * <i>Por Fábio Crestani</i>
 *
 * \section proposta Proposta
 *
 * Este projeto se propõe a implementar um sistema de controle de acesso utilizando um sistema
 * com um microcontrolador da família 8051 e um módulo de RFID.<br> O microcontrolador se comunica
 * com o computador via porta serial e valida ou não o acesso de um usuário.
 *
 * \image html diagrama_blocos.jpg
 *
 * \section fluxograma Fluxograma resumido
 * \image html fluxograma.jpg
 *
 * \section mais_info Mais informações
 *
 * Para mais informações acesse o relatório do projeto e/ou a documentação do programa do microcontrolador.
 *
 */

#include <REGx51.h>	
#include "DisplayLCD.h"
#include "LedsEBeeps.h"
#include "Serial.h"
#include "RFID.h"
#include "SPI.h"
#include "I2C.h"
#include "RTC.h"

// Pinos de I/O ------------------------------------------------------------------
	sfr LEDCON = 0xF1;
	#define SW1 P3_2
// -------------------------------------------------------------------------------

// Strings -----------------------------------------------------------------------
	char xdata STR_INICIALIZANDO[] = "Inicializando...";
	char xdata STR_SERIALINI[] = "Serial inicializada";
	char xdata STR_DISPLAYINI[] = "Display Inicializado";
	char xdata STR_RFIDINI[] = "RFID Inicializado";
	char xdata STR_RFIDSELFTEST[] = "RFID passou pelo self-test";
	char xdata STR_RFIDNAOSELFTEST[] = "RFID nao passou pelo self-test";
	char xdata STR_AGUARDANDOCARTAO[] = "Aguardando card";
	char xdata STR_TENTATIVAACESSO1[] = "Processando";
	char xdata STR_TENTATIVAACESSO2[] = "o acesso...";
	char xdata STR_AGUARDANDOPC[] = "Aguardando PC";
	char xdata STR_ACKSUCCESS[] = "Conectou com PC";
	char xdata STR_RFID_OK[] = "RFID OK";
	char xdata STR_RFID_NAO_OK[] = "RFID falhou";
	char xdata STR_RESET[] = "Erro. Reinicie";
	char xdata STR_PCOK[] = "Conectado ao PC";
// -------------------------------------------------------------------------------

// Outras variáveis globais
	unsigned int contador_timer0;
// -------------------------------------------------------------------------------

void main(){		
	unsigned char xdata TagType[64];
	unsigned int i;
	unsigned char A;
	char xdata numSerie[16];

// Inicializa Outros -------------------------------------------------------------
	LEDCON |= 0x10;						 // corrente de 2mA no led P3_6 (verde)
	DesligaBeep();
	DesligaRele();
	TMOD |= 1;						 	 // timer 0 no modo 1
	ET0 = 1;						     // habilita interrupção do timer 0
	EA = 1;								 // habilita todas as interrupções
// -------------------------------------------------------------------------------

// Inicializa Serial -------------------------------------------------------------
	InicializaSerial();
	StrToSerial(STR_SERIALINI);
// -------------------------------------------------------------------------------
	
// Inicializa Display LCD --------------------------------------------------------
	InicializaDisplay();
	EscStringDisplay(STR_INICIALIZANDO);
	StrToSerial(STR_DISPLAYINI);
// -------------------------------------------------------------------------------

// Inicializa Módulo RFID --------------------------------------------------------
	MFRC522_Init();
	StrToSerial(STR_RFIDINI);
	if(MFRC522_SelfTest()){						// Executa o selftest do RFID
		StrToSerial(STR_RFIDSELFTEST);
		EscStringDisplay2Linha(STR_RFID_OK);
		DelayRFID(3);
	}
	else{
		StrToSerial(STR_RFIDNAOSELFTEST);
		LimpaDisplay();
		EscStringDisplay(STR_RESET);
		PiscaString2Linha(STR_RFID_NAO_OK, 5, 1);
		EscStringDisplay2Linha(STR_RFID_NAO_OK);
		while(1); 								// Se RFID não-OK trava o programa
	}
// -------------------------------------------------------------------------------	

// Inicializa I2C e RTC ----------------------------------------------------------
	// temporário: para ajustar o relógio:
	//DS1307_Init();
	//DS1307_SetTime(0x16,0x22,0x02);     // 17:12:08
	//DS1307_SetDate(0x25,0x02,0x15, 5);  // 25/02/2015 QUA

	// temporário: para testar o RTC:
	//while(1) {
		//LimpaDisplay();
		//DS1307_GetDados_EnviaLCD();
		//DS1307_GetDados_EnviaSerial();
		//DelayRFID(5);
	//}
// -------------------------------------------------------------------------------	

MFRC522_Init();						 	 // Reinicia o módulo RFID

if(!SW1) while(1) ModoCadastro();		 // Se pressionou a tecla SW1

while(1){ // while principal -----------------------------------------------------

	TH0 = 0;  							 // valor inicial do timer 0
	TL0 = 0;							 // conta até 65535 (16 bits)
	contador_timer0 = 0;				 // contador auxiliar do timer 0

	// Envia enquiry e aguarda acknowledge -----------------------------------------
	do{	
		TR0 = 1;						 // liga timer 0
		EnviaACKPC();				 	 // Envia um pedido de acknowledge para o PC
		A = 0;							 // e aguarda confirmação.
		for(i=0;i<TIMEOUT_ACK;i++);		 // Delay de timeout	 
		A = RecebeCharSerial();		 	 // Recebe confirmação do ackowledge
		if(A == ASCII_ACK) break;	     // Se houve a confirmação, sai do loop
	}
    while(A != ASCII_ACK);				 // Enquanto não houve confirmação
	// -----------------------------------------------------------------------------

	// -----------------------------------------------------------------------------
	TR0 = 0;
	P3_6 = 0;
	DelayRFID(1);
	LimpaDisplay();
	EscStringDisplay2Linha(STR_PCOK);
	EscStringDisplay(STR_AGUARDANDOCARTAO);	
	// -------------------------------------------------------------------------------

	// Se tem um cartão no leitor ----------------------------------------------------
	if(MFRC522_isCard(&TagType)==1){ 	 			// Verifica se há algum cartão 
		AcendeLedVerde();
		LimpaDisplay();
		EscStringDisplay(STR_TENTATIVAACESSO1);
		EscStringDisplay2Linha(STR_TENTATIVAACESSO2);

		DelayRFID(1);
		EnviaTentativaPC();			 	 			// Envia tentativa de acesso via serial	

		if(MFRC522_ReadCardSerial(&numSerie)==1){	// Lê num. série cartão
			DelayRFID(1);							// Aguarda
			StrToSerial(":");					    // Envia início do núm. de série
			StrToSerial(numSerie);					// Envia núm. de série
			StrToSerial(";");						// Envia fim do núm. de série
		}else{
		 	StrToSerial("Erro: não foi possível ler num de série");
		}

		DS1307_GetDados_EnviaSerial(); 				// Envia informação de data e hora do RTC
		TrataCodResposta();							// Trata o código de resposta enviado pelo PC

		DelayRFID(3);
		LimpaDisplay();
		EscStringDisplay(STR_AGUARDANDOCARTAO);	
    }else{
		ApagaLedVerde();
	}	
	// -------------------------------------------------------------------------------


} // fim do while principal	

} // fim do main



/////////////////////////////////////////////////////////////////////////////////
/// timer0.
/// Propósito: Conta até (65535*0,5us*60 = 1,96 segundos) e então mostra no 
/// display a mensagem de que o computador não respondeu neste tempo e ainda está
/// aguardando a conexão.<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma<br>
///
/////////////////////////////////////////////////////////////////////////////////
void timer0(void) interrupt 1 {
	contador_timer0 = contador_timer0 + 1;
	if(contador_timer0 >= 60){
  		EscStringDisplay2Linha(STR_AGUARDANDOPC);
		TR0 = 0;
  	}
}