/// \file RFID.c Rotinas de tratamento do módulo de RFID além de rotina específicas deste projeto que usam o módulo.
#include <REGx51.h>	
#include "RFID.h"
#include "SPI.h"
#include "Serial.h"
#include "DisplayLCD.h"
#include "LedsEBeeps.h"

/////////////////////////////////////////////////////////////////////////////////
/// MFRC522_Init.
/// Propósito: Envia comandos para inicializar o módulo de RFID<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma<br>
///
/////////////////////////////////////////////////////////////////////////////////
void MFRC522_Init(){
	MFRC522_CS = 1;
	MFRC522_Rst = 1;
	
	MFRC522_Reset();				  // Reinicia
	
	// Configura registradores:
	MFRC522_Wr(TMODEREG, 0x8D);      // Tauto=1; f(Timer) = 6.78MHz/TPreScaler
	MFRC522_Wr(TPRESCALERREG, 0x3E); // TModeReg[3..0] + TPrescalerReg
	MFRC522_Wr(TRELOADREGL, 30);           
	MFRC522_Wr(TRELOADREGH, 0);        
	MFRC522_Wr(TXAUTOREG, 0x40);     // 100%ASK
	MFRC522_Wr(MODEREG, 0x3D);       // CRC valor inicial de 0x6363
	
	MFRC522_AntennaOn();			  // Liga a antena do módulo
}


/////////////////////////////////////////////////////////////////////////////////
/// MFRC522_isCard.
/// Propósito: Chama a função que verifica se há algum cartão próximo do leitor 
/// e devolve o tipo do cartão, se houver.<br>
/// Tipos conhecidos:<br><ul>
/// <li>0x0044 = Mifare_UltraLight</li>
///	<li>0x0004 = Mifare_One (S50)</li>
/// <li>0x0002 = Mifare_One (S70)</li>
///	<li>0x0008 = Mifare_Pro (X)</li>
///	<li>0x0344 = Mifare_DESFire</li></ul><br>
/// Entradas: TagType é um ponteiro para string que guarda o tipo do cartão<br>
/// Saídas: Retorna 1 se há algum cartão e 0 caso contrário<br>
///
/// \param [out] *TagType ponteiro para string com o tipo de cartão
/////////////////////////////////////////////////////////////////////////////////
int MFRC522_isCard(char *TagType){				
    if(MFRC522_Request(PICC_REQIDL, TagType) == MI_OK)
        return 1;
    else												  
        return 0;
}


/////////////////////////////////////////////////////////////////////////////////
/// MFRC522_Request.
/// Propósito: Função que de fato verifica se há cartão próximo ao leitor<br>
/// Entradas: char reqMode, char *TagType<br>
/// Saídas: char com o status
///
/// \param [in] reqMode
/// \param [out] *TagType Tipo de cartão
/////////////////////////////////////////////////////////////////////////////////
char MFRC522_Request(char reqMode, char *TagType){
  char _status;
  unsigned int backBits;            // The received data bits

  MFRC522_Wr(BITFRAMINGREG, 0x07);  // TxLastBists = BitFramingReg[2..0]
  TagType[0] = reqMode;

  _status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

  if((_status != MI_OK) || (backBits != 0x10)){
    _status = MI_ERR;
  }

  return _status;
}


/////////////////////////////////////////////////////////////////////////////////
/// MFRC522_Wr.
/// Propósito: Envia dados para o módulo<br>
/// Entradas: char addr, char value<br>
/// Saídas: Nenhuma<br>
///
/// \param [in] addr Endereço para escrever
/// \param [in] value Dados para escrever
/////////////////////////////////////////////////////////////////////////////////
void MFRC522_Wr(char addr, char value){
	MFRC522_CS = 0;
	SPIWrite(( addr << 1 ) & 0x7E);
	SPIWrite(value);
	MFRC522_CS = 1;
}


/////////////////////////////////////////////////////////////////////////////////
/// MFRC522_Rd.
/// Propósito: Recebe dados do módulo<br>
/// Entradas: char addr<br>
/// Saídas: Retorna o dado lido (char)<br>
///
/// \param [in] addr Endereço para ler
/////////////////////////////////////////////////////////////////////////////////
char MFRC522_Rd(char addr){
	char value;
  	MFRC522_CS = 0;
	SPIWrite((( addr << 1 ) & 0x7E) | 0x80);
	value = SPIRead();
	MFRC522_CS = 1;
	return value;
}


/////////////////////////////////////////////////////////////////////////////////
/// MFRC522_Clear_Bit.
/// Propósito: Zera um bit do módulo<br>
/// Entradas: char addr, char mask<br>
/// Saídas: Nenhuma<br>
///
/// \param [in] addr Endereço
/// \param [in] mask Máscara
/////////////////////////////////////////////////////////////////////////////////
void MFRC522_Clear_Bit(char addr, char mask){
     MFRC522_Wr(addr, MFRC522_Rd( addr ) & (~mask));
}


/////////////////////////////////////////////////////////////////////////////////
/// MFRC522_Set_Bit.
/// Propósito: Seta um bit do módulo<br>
/// Entradas: char addr, char mask<br>
/// Saídas: Nenhuma<br>
///
/// \param [in] addr Endereço
/// \param [in] mask Máscara
/////////////////////////////////////////////////////////////////////////////////
void MFRC522_Set_Bit(char addr, char mask){
     MFRC522_Wr(addr, MFRC522_Rd( addr ) | mask);
}


/////////////////////////////////////////////////////////////////////////////////
/// MFRC522_Reset.
/// Propósito: Reinicia o módulo<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma<br>
///
/////////////////////////////////////////////////////////////////////////////////
void MFRC522_Reset(){
	MFRC522_Wr(COMMANDREG, PCD_RESETPHASE);
}


/////////////////////////////////////////////////////////////////////////////////
/// MFRC522_AntennaOn.
/// Propósito: Liga a antena do módulo<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma<br>
///
/////////////////////////////////////////////////////////////////////////////////
void MFRC522_AntennaOn(){
 MFRC522_Set_Bit(TXCONTROLREG, 0x03);
}



/////////////////////////////////////////////////////////////////////////////////
/// MFRC522_ToCard.
/// Propósito: Envia comandos e dados para o cartão e recebe dados de resposta<br>
/// Entradas: char command, char *sendData, char sendLen, char *backData, unsigned *backLen<br>
/// Saídas: Retorna um char com o código de erro ou sucesso<br>
///
/// \param [in] command 
/// \param [in] *sendData
/// \param [in] sendLen
/// \param [out] *backData
/// \param [out] *backLen
/////////////////////////////////////////////////////////////////////////////////
char MFRC522_ToCard(char command, char *sendData, char sendLen, char *backData, unsigned *backLen){
	char _status = MI_ERR;
	char irqEn = 0x00;
	char waitIRq = 0x00;
	char lastBits;
	char n;
	unsigned int i;
	
	switch(command){
		case PCD_AUTHENT:       //Certification cards close
		{
		  irqEn = 0x12;
		  waitIRq = 0x10;
		  break;
		}
		case PCD_TRANSCEIVE:    //Transmit FIFO data
		{
		  irqEn = 0x77;
		  waitIRq = 0x30;
		  break;
		}
		default: break;
	}
	
	MFRC522_Wr(COMMIENREG, irqEn | 0x80);  //Interrupt request
	MFRC522_Clear_Bit(COMMIRQREG, 0x80);   //Clear all interrupt request bit
	MFRC522_Set_Bit(FIFOLEVELREG, 0x80);   //FlushBuffer=1, FIFO Initialization
	MFRC522_Wr(COMMANDREG, PCD_IDLE);      //NO action; Cancel the current command???
	
	//Writing data to the FIFO
	for(i=0; i<sendLen; i++){
		MFRC522_Wr(FIFODATAREG, sendData[i]);
	}
	
	//Execute the command
	MFRC522_Wr(COMMANDREG, command);
	if(command == PCD_TRANSCEIVE){
		MFRC522_Set_Bit(BITFRAMINGREG, 0x80); //StartSend=1,transmission of data starts  
	}
	
	//Waiting to receive data to complete
	//i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms???
	//i = 0xFFFF;   
	i = 0xFF;
	do{
		//CommIrqReg[7..0]
		//Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
		n = MFRC522_Rd(COMMIRQREG);
		i--;
	}
	while (i && !(n & 0x01) && !( n & waitIRq ));

  	MFRC522_Clear_Bit(BITFRAMINGREG, 0x80);    			//StartSend=0
  	if(i!=0){
    	if(!( MFRC522_Rd( ERRORREG ) & 0x1B )){ 		//BufferOvfl Collerr CRCErr ProtecolErr

      		_status = MI_OK;
      		if ( n & irqEn & 0x01 ){
        		_status = MI_NOTAGERR;       //??
      		}

      		if(command == PCD_TRANSCEIVE){
        		n = MFRC522_Rd( FIFOLEVELREG );
        		lastBits = MFRC522_Rd( CONTROLREG ) & 0x07;
        		if(lastBits){
          			*backLen = (n-1) * 8 + lastBits;
        		}
        		else{
          			*backLen = n * 8;
        		}

        		if(n==0){ n = 1;  }
        		if(n>16){ n = 16; }

        		
		        for(i=0; i < n; i++){							//Reading the received data in FIFO
    	      		backData[i] = MFRC522_Rd( FIFODATAREG );
        		}
  
  				backData[i] = 0;
    		}
    	}
    	else{
      	_status = MI_ERR;
    	}
	}

  //MFRC522_Set_Bit( CONTROLREG, 0x80 );
  //MFRC522_Wr( COMMANDREG, PCD_IDLE );
  return _status;
}


// *****************************************************************************************
// As funções a seguir não foram utilizadas neste projeto e foram comentadas para evitar 
// sua compilação e assim poupar espaço da memória ROM do microcontrolador.
// *****************************************************************************************

/////////////////////////////////////////////////////////////////////////////////
// MFRC522_AntennaOff.
// Propósito: Desliga a antena do módulo<br>
// Entradas: Nenhuma<br>
// Saídas: Nenhuma<br>
//
/////////////////////////////////////////////////////////////////////////////////
//void MFRC522_AntennaOff(){
//	MFRC522_Clear_Bit(TXCONTROLREG, 0x03);
//}

////////////////////////////////////////////////////////////////////////////////
// MFRC522_CRC.
// Propósito: Chama a função de cálculo do CRC (Cyclic redundancy check) do chip.
// (Algoritmo para detecção de erros).<br>
// Entradas: char *dataIn, char length, char *dataOut<br>
// Saídas: Nenhuma<br>
//
// \param [in] *dataIn
// \param [in] length
// \param [out] *dataOut
/////////////////////////////////////////////////////////////////////////////////
//void MFRC522_CRC(char *dataIn, char length, char *dataOut){
//	char i, n;
//	MFRC522_Clear_Bit(DIVIRQREG, 0x04);
//	MFRC522_Set_Bit(FIFOLEVELREG, 0x80);    
//	   
//	for(i=0; i<length; i++){             // Escreve dados no FIFO 
//	    MFRC522_Wr(FIFODATAREG, *dataIn++);
//	}
//	
//	MFRC522_Wr(COMMANDREG, PCD_CALCCRC);
//	i = 0xFF;
//	
//	do{									 // Espera a finalização do Calculo do CRC
//		n = MFRC522_Rd(DIVIRQREG);
//	    i--;
//	}
//	while( i && !(n & 0x04) );       	 // CRCIrq = 1
//	    
//	dataOut[0] = MFRC522_Rd(CRCRESULTREGL);
//	dataOut[1] = MFRC522_Rd(CRCRESULTREGM);
//}

//char MFRC522_SelectTag(char *serNum){
//  char i;
//  char _status;
//  char size;
//  unsigned recvBits;
//  char buffer[9];
//  
//  //MFRC522_Clear_Bit( STATUS2REG, 0x08 );   //MFCrypto1On=0
//  
//  buffer[0] = PICC_SElECTTAG;
//  buffer[1] = 0x70;
//  
//  for(i=2; i<7; i++){
//    buffer[i] = *serNum++;
//  }
//  
//  MFRC522_CRC(buffer, 7, &buffer[7]);
//  
//  _status = MFRC522_ToCard(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);
//
//  if( (_status == MI_OK) && (recvBits == 0x18) ){
//    size = buffer[0];
//  }
//  else{
//    size = 0;
//  }
//
//  return size;
//}
//		
//void MFRC522_Halt(){ //hibernation
//  //unsigned unLen;
//  unsigned int unLen;
//  //char buff[4];
//  char xdata buff[4];
//  
//  buff[0] = PICC_HALT;
//  buff[1] = 0;
//  MFRC522_CRC( buff, 2, &buff[2] );
//  MFRC522_Clear_Bit( STATUS2REG, 0x80 );
//  MFRC522_ToCard( PCD_TRANSCEIVE, buff, 4, buff, &unLen );
//  MFRC522_Clear_Bit( STATUS2REG, 0x08 );
//}
//
//char MFRC522_Auth(char authMode, char BlockAddr, char *Sectorkey, char *serNum){
//  char _status;
//  //unsigned recvBits;
//  unsigned int recvBits;
//  char i;
//  char xdata buff[12];
//  //char buff[12];
//  
//  //Verify the command block address + sector + password + card serial number
//  buff[0] = authMode;
//  buff[1] = BlockAddr;
//  
//  for(i=2; i<8; i++){
//    buff[i] = Sectorkey[i-2];
//  }
//  
//  for(i=8; i<12; i++){
//    buff[i] = serNum[i-8];
//  }
//  
//  _status = MFRC522_ToCard( PCD_AUTHENT, buff, 12, buff, &recvBits );
//  
//  if(( _status != MI_OK ) || !( MFRC522_Rd( STATUS2REG ) & 0x08 )){
//    _status = MI_ERR;
//  }
//  
//  return _status;
//} 
//
//char MFRC522_Write(char blockAddr, char *writeData){
//  char _status;
//  unsigned int recvBits;
//  char i;
//  char buff[18];
//  buff[0] = PICC_WRITE;
//  buff[1] = blockAddr;
//  
//  MFRC522_CRC(buff, 2, &buff[2]);
//  _status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);
//
//  if((_status != MI_OK) || (recvBits != 4) || ( (buff[0] & 0x0F) != 0x0A)){
//    _status = MI_ERR;
//  }
//  if(_status == MI_OK){
//    for(i=0; i<16; i++){                //Data to the FIFO write 16Byte
//    	buff[i] = writeData[i];
//    }
//    
//    MFRC522_CRC(buff, 16, &buff[16]);
//    _status = MFRC522_ToCard( PCD_TRANSCEIVE, buff, 18, buff, &recvBits );
//
//    if((_status != MI_OK) || (recvBits != 4) || ( (buff[0] & 0x0F) != 0x0A)){
//      _status = MI_ERR;
//    }
//  }
//  return _status;
//}
//
//char MFRC522_Read(char blockAddr, char *recvData){
//  char _status;
//  unsigned int unLen;
//  recvData[0] = PICC_READ;
//  recvData[1] = blockAddr;
//  
//  MFRC522_CRC(recvData, 2, &recvData[2]);
//  _status = MFRC522_ToCard(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);
//
//  if((_status != MI_OK) || (unLen != 0x90)){
//    _status = MI_ERR;
//  }
//  return _status;
//}
 
/////////////////////////////////////////////////////////////////////////////////
/// MFRC522_AntiColl.
/// Propósito: Algoritmo anti-colisão: Se dois ou mais cartões estiverem próximos
/// da antena, apenas um deles é selecionado para a leitura.<br>
/// Entradas: char *serNum<br>
/// Saídas: Retorna um char com o código de erro ou sucesso<br>
///
/// \param [in] *serNum número serial do cartão atual.
/////////////////////////////////////////////////////////////////////////////////
char MFRC522_AntiColl(char *serNum){
  char _status;
  char i;
  char serNumCheck = 0;
  unsigned int unLen;

  MFRC522_Wr(BITFRAMINGREG, 0x00);                //TxLastBists = BitFramingReg[2..0]
  serNum[0] = PICC_ANTICOLL;
  serNum[1] = 0x20;
  MFRC522_Clear_Bit(STATUS2REG, 0x08);
  _status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

  if(_status == MI_OK){
    for(i=0; i<4; i++){
      serNumCheck ^= serNum[i];
    }
    
	if(serNumCheck != serNum[4]){
    	_status = MI_ERR;
    }
  }
  return _status;
}
 
/////////////////////////////////////////////////////////////////////////////////
/// MFRC522_ReadCardSerial.
/// Propósito: Lê o número de série do cartão.<br>
/// Entradas: str é um ponteiro para a string que receberá o número de série<br>
/// Saídas: Retorna 1 se leu corretamente, e 0 caso contrário<br>
///
/// \param [out] *str ponteiro para o número de série
/////////////////////////////////////////////////////////////////////////////////
char MFRC522_ReadCardSerial(char *str){
	char _status; 
	_status = MFRC522_AntiColl(str);
	str[5] = 0;

	if(_status == MI_OK)
  		return 1;
 	else
  		return 0;
}
  
/////////////////////////////////////////////////////////////////////////////////
/// MFRC522_SelfTest.
/// Propósito: Faz um auto-teste digital. Baseado na página 82 do manual do<br>
///  MFRC522 da NXP: The MFRC522 has the capability to perform a digital 
///  self test. The self test is started by using the following procedure: <br>
///  <ul>
/// 	<li>1. Perform a soft reset.</li>
/// 	<li>2. Clear the internal buffer by writing 25 bytes of 00h and implement 
///     the Config command.</li>
/// 	<li>3. Enable the self test by writing 09h to the AutoTestReg register.</li>
/// 	<li>4. Write 00h to the FIFO buffer.</li>
/// 	<li>5. Start the self test with the CalcCRC command.</li>
/// 	<li>6. The self test is initiated.</li>
/// 	<li>7. When the self test has completed, the FIFO buffer contains 64 bytes.</li>
///  </ul><br>
/// 	Obs.: A versão de firmware deste chip é a 2.<br>
/// Entradas: Nenhuma<br>
/// Saídas: int retorna 1 se passou pelo teste, ou 0 caso contrário<br>
///
/////////////////////////////////////////////////////////////////////////////////
int MFRC522_SelfTest(){
	char A, n;
	unsigned int i;

	// resultado esperado (é armazenado na memória externa (xdata))
	char xdata selfTestResultV2[] = {					
		 0x00, 0xEB, 0x66, 0xBA, 0x57, 0xBF, 0x23, 0x95,
         0xD0, 0xE3, 0x0D, 0x3D, 0x27, 0x89, 0x5C, 0xDE,
         0x9D, 0x3B, 0xA7, 0x00, 0x21, 0x5B, 0x89, 0x82,
         0x51, 0x3A, 0xEB, 0x02, 0x0C, 0xA5, 0x00, 0x49,
         0x7C, 0x84, 0x4D, 0xB3, 0xCC, 0xD2, 0x1B, 0x81,
         0x5D, 0x48, 0x76, 0xD5, 0x71, 0x61, 0x21, 0xA9,
         0x86, 0x96, 0x83, 0x38, 0xCF, 0x9D, 0x5B, 0x6D,
         0xDC, 0x15, 0xBA, 0x3E, 0x7D, 0x95, 0x3B, 0x2F}; 
	
	//A = MFRC522_Rd(VERSIONREG);					// 0. Versão do firmware:
	//EnviaCharSerial(A);
	MFRC522_Wr(COMMANDREG, PCD_SOFTRESET);			// 1. Soft Reset
													// 2. Limpar o buffer interno 00h para FIFO
	MFRC522_Wr(FIFODATAREG, 0x00); 					// 00h para o FIFO e então copia da FIFO para o buffer
	MFRC522_Wr(COMMANDREG, PCD_MEM); 				// Comando Mem 0001
	MFRC522_Wr(AUTOTESTREG, 0x09); 					// 3. 09h para o AutoTestReg
	MFRC522_Wr(FIFODATAREG, 0x00);					// 4. 00h para o FIFO
													// 5. Comando CalcCRC 0011
	MFRC522_Wr(COMMANDREG, PCD_CALCCRC); 			// CalcCRC
	MFRC522_Wr(COMMANDREG, PCD_IDLE); 				// Idle
  	i = 0xFF; 										// 6. Aguarda o teste completar
  	do{												//
    	n = MFRC522_Rd(DIVIRQREG);				    // 
    	i--;									    //
  	}while((i!=0)&&!(n&0x04));					    // agurda IRQ ou timeout
	
	for(i=0;i<64;i++){								// 7. Lê os dados (64 bytes)
		A = MFRC522_Rd(FIFODATAREG);				//
		//EnviaCharSerial(A);						//
		if(A!= selfTestResultV2[i])					// Se algum byte estiver errado
		return 0;								    // retorna 0 (falhou)
	}
	  
	return 1;
} // fim do self test


/////////////////////////////////////////////////////////////////////////////////
/// DelayRFID.
/// Propósito: Implementa um delay <br>
/// Entradas: v * 253 * 253 * 0,5[micro segundos] ~= tempo do delay<br>
/// Saídas: Nenhuma<br>
///
/// \param [in] v Relativo ao tempo de delay
/////////////////////////////////////////////////////////////////////////////////
void DelayRFID(int v){
	unsigned int j, i, k;
	for(j=0; j<v; j++){
		for(i=0;i<254;i++){
			for(k=0;k<254;k++);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////
// GravaTagRFID.
// - não implementado - <br><br>
// Propósito: Grava uma TAG no cartão<br>
// Entradas: <br>
// Saídas: nenhuma<br>
/////////////////////////////////////////////////////////////////////////////////
//void GravaTagRFID(char *Tag){
//}

/////////////////////////////////////////////////////////////////////////////////
/// ModoCadastro.
/// Propósito: Rotina para cadastro de cartão<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nenhuma<br>
///
/////////////////////////////////////////////////////////////////////////////////
void ModoCadastro(){
	char modo_cadastro = 1;
	char cadastro_sucesso = 0;
	char A;
	unsigned int i;
	unsigned char xdata TagType[64];
	char xdata numSerie[16];

	char xdata STR_NUMSERIALLIDO[] = "Num. serial lido";
	char xdata STR_NUMSERIALNAOLIDO[] = "Num. serial não lido";
	char xdata STR_TENTECADASTRO[] = "Erro no cadastro";
	char xdata STR_MODOCADASTRO[] = "Modo cadastro";
	char xdata STR_APROXIMECADASTRO[] = "Aproxime o card";
	char xdata STR_CADASTROOK1[] = "Conclua seu ";
	char xdata STR_CADASTROOK2[] = "cadastrado no PC";
	char xdata STR_PRESSERIALPC[] = "Confirme no PC";
	char xdata STR_AGUARDANDOPC[] = "Aguardando PC";

	LimpaDisplay();
	EscStringDisplay(STR_MODOCADASTRO);
	EscStringDisplay2Linha(STR_AGUARDANDOPC);
	DelayRFID(8);

	i = 0;	
	while(modo_cadastro){

		// Envia enquiry e aguarda acknowledge -----------------------------------------
		do{	
			EnviaACKPC();				 	 // Envia um pedido de acknowledge para o PC
			A = 0;							 // e aguarda confirmação.
			for(i=0;i<TIMEOUT_ACK;i++);		 // Delay de timeout	 
			A = RecebeCharSerial();		 	 // Recebe confirmação do ackowledge
			if(A == ASCII_ACK) break;	     // Se houve a confirmação, sai do loop
		}
		while(A != ASCII_ACK);				 // Enquanto não houve confirmação

		EscStringDisplay2Linha(STR_APROXIMECADASTRO);

    	if(MFRC522_isCard(&TagType)==1){ 	 // Verifica se há algum cartão 
			P3_6 = 0;					 	 // Acende led verde da placa

 			// Tenta ler o número de série do cartão e envia via porta serial
			if(MFRC522_ReadCardSerial(&numSerie)==1){
			
//				// Envia enquiry e aguarda acknowledge -----------------------------------------
//				do{	
//					EnviaACKPC();				 	 // Envia um pedido de acknowledge para o PC
//					A = 0;							 // e aguarda confirmação.
//					for(i=0;i<TIMEOUT_ACK;i++);		 // Delay de timeout	 
//					A = RecebeCharSerial();		 	 // Recebe confirmação do ackowledge
//					if(A == ASCII_ACK) break;	     // Se houve a confirmação, sai do loop
//				}
//    			while(A != ASCII_ACK);				 // Enquanto não houve confirmação
			 	DelayRFID(1);
				StrToSerial(":");
				StrToSerial(numSerie);
				StrToSerial(";");
				cadastro_sucesso = 1;
				modo_cadastro = 0;
				EscStringDisplay2Linha(STR_NUMSERIALLIDO);
				DelayRFID(5);
			}else{	
				cadastro_sucesso = 0;
				EscStringDisplay2Linha(STR_NUMSERIALNAOLIDO);
				DelayRFID(5);
			}
        }else{
			P3_6 = 1;						 // apaga led verde da placa
		}
			i++;
			if(i==255) modo_cadastro = 0;
			DelayRFID(3);
	} // fim while modo cadastro
			
	if(cadastro_sucesso){
		EscStringDisplay2Linha(STR_PRESSERIALPC);
		DelayRFID(12);
		EscStringDisplay(STR_CADASTROOK1);
		EscStringDisplay2Linha(STR_CADASTROOK2);
		DelayRFID(5);
	}else{
		EscStringDisplay2Linha(STR_TENTECADASTRO);
		DelayRFID(5);
	}
}


/////////////////////////////////////////////////////////////////////////////////
/// TrataCodResposta.
/// Propósito: Tratamento do código de resposta enviado pelo PC numa tentativa de
/// acesso.<br>
/// Entradas: Nenhuma<br>
/// Saídas: Nnenhuma<br>
///
/// <ul>
/// <li>ERRO_LEITURA_SERIAL       (0): Não foi possível ler o número de série do cartão</li>
/// <li>ERRO_PC                   (1): Erro genérico no programa do PC</li>
/// <li>ERRO_NAO_CADASTRADO       (2): Número de série não cadastrado</li>
/// <li>ERRO_CREDITO_INSUFICIENTE (3): Número de créditos insuficiente (igual a 0)</li>
/// <li>OK_ACESSO                 (4): Acesso ok</li>
/// </ul>
/// <br>
/////////////////////////////////////////////////////////////////////////////////
void TrataCodResposta(){
	unsigned int A, NUM_CREDITOS, i, overflow;

// Strings -----------------------------------------------------------------------
	char xdata STR_ACESSO_7[] = "Acesso OK";
	char xdata STR_ACESSO_4[] = "Creditos";
	char xdata STR_ACESSO_4_2L[] = "insuficientes";
	char xdata STR_ACESSO_3[] = "Nao cadastrado";
	char xdata STR_ACESSO_2[] = "Erro no PC";
	char xdata STR_ACESSO_1[] = "Erro no RFID";
	char xdata STR_ACESSO_1_2L[] = "Tente novamente";
	char xdata STR_CREDITOS[] = "Creditos: ";
// -------------------------------------------------------------------------------

	do{	
		A = 0;							 	  // Aguarda ACK
		for(i=0;i<TIMEOUT_ACK;i++);			  // Delay de timeout	 
		A = RecebeCharSerial();		 	 	  // Recebe confirmação do ackowledge
		if(A == ASCII_ACK) break;	     	  // Se houve a confirmação, sai do loop
	}
    while(A != ASCII_ACK);				 	  // Enquanto não houve ACK

	A = 0;
	while(1){							 	  // Aguarda código de erro
		for(i=0;i<TIMEOUT_ACK;i++);		 	  // Delay de timeout	 
		A = RecebeCharSerial();		 	 	  // Recebe confirmação do ackowledge
		if(A == COD_ACESSO_OK      || 
		   A == COD_CREDITO_INSUF  || 
		   A == COD_NAO_CADASTRADO || 
		   A == COD_ERRO_PC        ||
		   A == COD_ERRO_RFID) break;
	}
	EnviaCharSerial(A);					 	  // Devolve o mesmo código para comparação
	LimpaDisplay();

	// Acesso OK -----------------------------------------------------------------------
	if(A == COD_ACESSO_OK){					
		EscStringDisplay(STR_ACESSO_7);
		//AcendeLedVermelho();						  // Aciona porta P1_4
		LigaRele();									  // Aciona porta P1_2
		LigaBeep();									  // Beep na porta P1_3
		DelayRFID(1);

		NUM_CREDITOS = 0;	  						  // Começa leitura núm. créditos
		while(NUM_CREDITOS==0)
			NUM_CREDITOS = RecebeCharSerial();	      // Descarta este valor

		EscStringDisplay2Linha(STR_CREDITOS);		  // Escreve "Creditos: "
		XYDisplay(1,10);						  	  // Posiciona cursor no display
		overflow = 1;
		for(i=0;i<3;i++){
			NUM_CREDITOS = 0;
			while(NUM_CREDITOS==0)
				NUM_CREDITOS = RecebeCharSerial();	  // Recebe o número de créditos
			EscDadoDisplay(NUM_CREDITOS);	  		  // Exibe créditos no display
			if(NUM_CREDITOS != 0x39) overflow = 0;
		}
		if(overflow == 1) EscDadoDisplay('+'); 		  // Escreve '+' se valor é 999
		
		DelayRFID(9);
	}
	// ---------------------------------------------------------------------------------

	// Crédito insuficiente ------------------------------------------------------------
	if(A ==	COD_CREDITO_INSUF){
	  EscStringDisplay(STR_ACESSO_4);
	  EscStringDisplay2Linha(STR_ACESSO_4_2L);
	  BeepRepetido(3, 80);
	}
	// ---------------------------------------------------------------------------------

	// Cartão não cadastrado -----------------------------------------------------------
	if(A ==	COD_NAO_CADASTRADO){
		EscStringDisplay(STR_ACESSO_3);
		BeepRepetido(4, 150);
	}
	// ---------------------------------------------------------------------------------

	// Outros erros --------------------------------------------------------------------
	if(A ==	COD_ERRO_PC){
		EscStringDisplay(STR_ACESSO_2);
		BeepRepetido(3, 100);
	}

	if(A == COD_ERRO_RFID){
		EscStringDisplay(STR_ACESSO_1);
		EscStringDisplay2Linha(STR_ACESSO_1_2L);
		BeepRepetido(2, 150);
	}
	// ---------------------------------------------------------------------------------

	
	//ApagaLedVermelho();
	DesligaRele();									  
	DesligaBeep();
	DelayRFID(10);
}


