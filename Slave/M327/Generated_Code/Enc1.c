/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : Enc1.c
**     Project     : M327
**     Processor   : MC56F84789VLL
**     Component   : PulseAccumulator
**     Version     : Component 01.193, Driver 02.06, CPU db: 3.50.001
**     Compiler    : CodeWarrior DSP C Compiler
**     Date/Time   : 2016-11-17, 22:35, # CodeGen: 172
**     Abstract    :
**          Pulse accumulator decodes the primary and secondary external inputs as
**          quadrature encoded signals. Quadrature signals are usually generated by
**          rotary or linear sensors used to monitor movement of motor shafts or
**          mechanical equipment.
**     Settings    :
**             Pulse accumulator       : TMRB0_PACNT
**
**
**         Mode                        : Quadrature
**
**         Initialization:
**              Pulse accumulator      : Enabled
**              Events                 : Enabled
**
**         Timer registers
**              Counter                : TMRB_0_CNTR [E185]
**              Hold                   : TMRB_0_HOLD [E184]
**              Run                    : TMRB_0_CTRL [E186]
**              Prescaler              : TMRB_0_CTRL [E186]
**
**         User handling procedure     : not specified
**
**     Contents    :
**         Enable           - byte Enc1_Enable(void);
**         Disable          - byte Enc1_Disable(void);
**         ResetCounter     - void Enc1_ResetCounter(void);
**         GetCounterValue  - void Enc1_GetCounterValue(Enc1_TCounterValue *Value);
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file Enc1.c
** @version 02.06
** @brief
**          Pulse accumulator decodes the primary and secondary external inputs as
**          quadrature encoded signals. Quadrature signals are usually generated by
**          rotary or linear sensors used to monitor movement of motor shafts or
**          mechanical equipment.
*/         
/*!
**  @addtogroup Enc1_module Enc1 module documentation
**  @{
*/         

/* MODULE Enc1. */

#include "Enc1.h"

/*
** ===================================================================
**     Method      :  Enc1_Enable (component PulseAccumulator)
*/
/*!
**     @brief
**         Enables the component.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
*/
/* ===================================================================*/
byte Enc1_Enable(void)
{
  setRegBitGroup(TMRB_0_CTRL,CM,0x04); /* Run counter */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  Enc1_Disable (component PulseAccumulator)
*/
/*!
**     @brief
**         Disables the component.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
*/
/* ===================================================================*/
byte Enc1_Disable(void)
{
  setRegBitGroup(TMRB_0_CTRL,CM,0);    /* Stop counter */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  Enc1_ResetCounter (component PulseAccumulator)
*/
/*!
**     @brief
**         This method resets the pulse accumulator counter (sets to
**         zero).
*/
/* ===================================================================*/
void Enc1_ResetCounter(void)
{
  setReg(TMRB_0_CNTR,0x00);            /* Reset counter register */
}

/*
** ===================================================================
**     Method      :  Enc1_GetCounterValue (component PulseAccumulator)
*/
/*!
**     @brief
**         This method returns the content of the pulse accumulator
**         counter.
**     @param
**         Value           - Pointer to returned value. [Note:]
**                           Parameter type depends on the width of the
**                           selected counter (may be 8, 16, 32 or 64
**                           bits long). See the method prototype in the
**                           generated module.
*/
/* ===================================================================*/
void Enc1_GetCounterValue(Enc1_TCounterValue *Value)
{
  *Value = getReg(TMRB_0_CNTR);        /* Return counter value */
}

/*
** ===================================================================
**     Method      :  Enc1_Init (component PulseAccumulator)
**
**     Description :
**         Initializes the associated peripheral(s) and the beans 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void Enc1_Init(void)
{
  /* TMRB_0_CTRL: CM=0,PCS=0,SCS=1,ONCE=0,LENGTH=0,DIR=0,COINIT=0,OUTMODE=0 */
  setReg(TMRB_0_CTRL,0x80);            /* Set up mode */
  /* TMRB_0_SCTRL: TCF=0,TCFIE=0,TOF=0,TOFIE=0,IEF=0,IEFIE=0,IPS=0,INPUT=0,CAPTURE_MODE=0,MSTR=0,EEOF=0,VAL=0,FORCE=0,OPS=0,OEN=0 */
  setReg(TMRB_0_SCTRL,0x00);
  setReg(TMRB_0_CNTR,0x00);            /* Reset counter register */
  setReg(TMRB_0_LOAD,0x00);            /* Reset load register */
  setReg(TMRB_0_COMP1,0xFFFF);         /* Set up compare 1 register */
  setReg(TMRB_0_COMP2,0x00);           /* Set up compare 2 register */
  /* TMRB_0_CSCTRL: DBG_EN=0,FAULT=0,ALT_LOAD=0,ROC=0,TCI=0,UP=0,??=0,TCF2EN=0,TCF1EN=0,TCF2=0,TCF1=0,CL2=0,CL1=0 */
  setReg(TMRB_0_CSCTRL,0x00);
  /* TMRB_0_FILT: ??=0,??=0,??=0,??=0,??=0,FILT_CNT=0,FILT_PER=0 */
  setReg(TMRB_0_FILT,0x00);            /* Set up input filter register */
  setRegBitGroup(TMRB_0_CTRL,CM,0x04); /* Run counter */
}

/* END Enc1. */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/