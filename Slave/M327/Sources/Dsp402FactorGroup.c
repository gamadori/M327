/** ###################################################################
**     Filename  : dsp402FactorGroup.C
**     Project   : MP_M307_SLAVE
**     Processor : 56F8323
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 06/06/2011, 22.57
**     Contents  :
**         User source code
**
** ###################################################################*/

/* MODULE dsp402FactorGroup */



#include "cpu.h"
#include "Dsp402StateMachine.h"
#include "Dsp402FactorGroup.h"

td_factorGroup fg402[NUM_MOTORS];

/*====================================
 * 
 * Inizializza le variabili del modulo
 *
 *====================================*/

void FactorGroupInit(td_DSP402 *pDev, word axe)
{
	pDev->fg = &fg402[axe];	
		
	pDev->fg->speedFactor.count = 2;
	pDev->fg->speedFactor.encoderRev = 5;
	pDev->fg->speedFactor.unitMinute = 600;
	
}



/* END dsp402FactorGroup */
