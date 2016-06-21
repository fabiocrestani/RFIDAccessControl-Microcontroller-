#include <REGx51.h>	
#include "Timers.h"
#include "DisplayLCD.h"

/// \file Timers.c Rotinas de interrup��es de timers.
/// Autor: F�bio Crestani<br>
/// �ltima modifica��o: 29/01/2015<br>

/////////////////////////////////////////////////////////////////////////////////
/// timer0.
/// Prop�sito: 
/// Entradas: Nenhuma<br>
/// Sa�das: Nenhuma
///
/////////////////////////////////////////////////////////////////////////////////
void timer0 (void) interrupt 1{// using 2  {
	contador_timer0 = contador_timer0 + 1;
	if(contador_timer0 >= 60){
  		EscStringDisplay2Linha(STR_AGUARDANDOPC);
		TR0 = 0;
  	}
}