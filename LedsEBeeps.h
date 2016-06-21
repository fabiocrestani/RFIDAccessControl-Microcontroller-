/// \file LedsEBeeps.h Arquivo de cabeçalho do arquivo LedsEBeeps.c
/// Contém rotinas que acendem e apagam leds da placa e acionam ou não
/// uma porta configurada para acionar um buzzer.<br>
/// Autor: Fábio Crestani.<br>
/// Última modificação: 26/01/2015.<br>

// =======================================================================
// Defines
	#define	BUZZER P1_3
	#define RELE   P1_2
// =======================================================================

// =======================================================================
// Protótipos		
	// Leds
	void AcendeLedVermelho();
	void ApagaLedVermelho();
	void AcendeLedVerde();
	void ApagaLedVerde();

	// Beeps
	void BeepRepetido(unsigned int repeticoes, unsigned int duracao);
	void LigaBeep();
	void DesligaBeep();

	// Relé
	void LigaRele();
	void DesligaRele();
// =======================================================================