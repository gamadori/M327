/** ###################################################################
**     Filename  : 402PositionMode.H
**     Project   : BattutaMobile
**     Processor : 56F8323
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 31/05/2011, 16.16
**     Contents  :
**         User source code
**
** ###################################################################*/

#ifndef __402PositionMode_H
#define __402PositionMode_H

/* MODULE 402PositionMode */


extern td_pp pp402[];

void PositionReset(td_DSP402 *pDev);

void PositionModeServer(td_DSP402 *pDev);

void PositionInit(td_DSP402 *pDev, word axe);

void SendTargetPosition(td_DSP402 *pDev);

void JogModeServerPos(td_DSP402 *pDev);

void JogModeServerNeg(td_DSP402 *pDev);

/* END 402PositionMode */

#endif

