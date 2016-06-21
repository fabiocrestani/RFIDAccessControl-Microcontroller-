/// \file Serial.h Arquivo de cabe�alho do arquivo Serial.c.
// =======================================================================
// ROTINAS DE COMUNICA��O COM A PORTA SERIAL
// =======================================================================
// Autor: F�bio Crestani
// �ltima modifica��o: 24/12/2014
// 
// =======================================================================

// =======================================================================
// Defines
	 #define ASCII_CR  		    13				///< ASCII para Carriage Return
	 #define ASCII_ENQ 		    5				///< ASCII para Enquiry
	 #define ASCII_ACK 		    6				///< ASCII para Acknowledge
	 #define COD_ACESSO_OK      7               ///< COD 5: Acesso OK
	 #define COD_CREDITO_INSUF  4				///< COD 4: Cr�dito insuficiente
	 #define COD_NAO_CADASTRADO 3				///< COD 3: Cart�o n�o cadastrado
	 #define COD_ERRO_PC        2				///< COD 2: Erro com o PC
	 #define COD_ERRO_RFID      1				///< COD 8: Erro com o RFID

	 #define TIMEOUT_ENQ     1				///< m�x. de 255
	 #define TIMEOUT_ACK	 10				///< m�x. de 255
// =======================================================================

// =======================================================================
// Prot�tipos
	  void InicializaSerial();
		unsigned char RecebeCharSerial();
		void EnviaCharSerial(unsigned char c);
		void StrToSerial(unsigned char *string);

		void EnviaACKPC();
		void EnviaTentativaPC();
// =======================================================================
