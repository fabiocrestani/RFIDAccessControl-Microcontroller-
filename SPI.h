/// \file SPI.h Arquivo de cabeçalho do arquivo SPI.c.

// =======================================================================
// ROTINAS DE SPI VIA BIT BANGING
// =======================================================================
// Autor: Fábio Crestani
// A versão atual não usa o periférico SPI do microcontrolador, mas sim
//  a técnica de big banging. MISO, MOSI e SCK podem ser configurados para
//  quaisquer pinos de portas disponíveis do microcontrolador.
// Última modificação: 28/12/2014
// =======================================================================

// =======================================================================
// Defines
	 #define MISO        P1_5	   				///< Master In, Slave Out
	 #define MOSI 		 P1_7	   				///< Master Out, Slave In
	 #define SCK		 P1_6      				///< Slave Clock
// =======================================================================

// =======================================================================
// Protótipos
	char SPIRead();
	void SPIWrite(char dado);
	
	void DelaySPI(unsigned int i);
	void SetMOSI();
	void ClearMOSI();
	void SetSCK();
	void ClearSCK();
	char GetMISO();
// =======================================================================