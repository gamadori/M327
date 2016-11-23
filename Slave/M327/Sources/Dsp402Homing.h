/** ###################################################################
**     Filename  : 402Homing.H
**     Project   : BattutaMobile
**     Processor : 56F8323
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 31/05/2011, 16.20
**     Contents  :
**         User source code
**
** ###################################################################*/

#ifndef __402Homing_H
#define __402Homing_H

/* MODULE 402Homing */



extern td_hm hm402[];


void HomingInit(td_DSP402 *pDev, word axe);

void HomingServer(td_DSP402 *pDev);

void HomingReset(td_DSP402 *pDev);

bool IsInHoming();

/* END 402Homing */

#endif

