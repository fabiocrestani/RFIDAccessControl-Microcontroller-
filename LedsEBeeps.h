/// \file LedsEBeeps.h Arquivo de cabe�alho do arquivo LedsEBeeps.c
/// Cont�m rotinas que acendem e apagam leds da placa e acionam ou n�o
/// uma porta configurada para acionar um buzzer.<br>
/// Autor: F�bio Crestani.<br>
/// �ltima modifica��o: 26/01/2015.<br>

// =======================================================================
// Defines
	#define	BUZZER P1_3
	#define RELE   P1_2
// =======================================================================

// =======================================================================
// Prot�tipos		
	// Leds
	void AcendeLedVermelho();
	void ApagaLedVermelho();
	void AcendeLedVerde();
	void ApagaLedVerde();

	// Beeps
	void BeepRepetido(unsigned int repeticoes, unsigned int duracao);
	void LigaBeep();
	void DesligaBeep();

	// Rel�
	void LigaRele();
	void DesligaRele();
// =======================================================================