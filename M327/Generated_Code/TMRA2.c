/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : TMRA2.c
**     Project     : M327
**     Processor   : MC56F84789VLL
**     Component   : Init_TMR
**     Version     : Component 01.161, Driver 02.09, CPU db: 3.50.001
**     Compiler    : CodeWarrior DSP C Compiler
**     Date/Time   : 2016-07-04, 14:29, # CodeGen: 52
**     Abstract    :
**         This "Init_TMR" Peripheral Inspector implements the
**         Quad Timer module (TMR), basic initialization
**         and settings.
**     Settings    :
**          Component name                                 : TMRA2
**          Device                                         : TMRA2
**          Settings                                       : 
**            Clock settings                               : 
**              Primary source                             : 
**                Primary source                           : prescaler (IP BUS clock / 128)
**              Secondary source                           : 
**                Secondary source                         : counter 2 input pin
**            Operation mode                               : Count mode
**            Count once                                   : count repeatedly
**            Count length                                 : roll-over
**            Count direction                              : up
**            Master mode                                  : Disabled
**            External OFLAG force                         : Disabled
**            Forced OFLAG value                           : 0
**            Force OFLAG output                           : no
**            Output enable                                : no
**            Output polarity                              : true
**            Input polarity                               : true
**            Co-channel initialization                    : Disabled
**            Input capture mode                           : Enabled
**              Load capture register                      : on any edge of input
**            Reload on capture                            : no
**            Triggered count initialization               : Stop counter
**            OutputMode                                   : asserted while counter is active
**            Compare load control 1                       : Disabled
**            Compare load control 2                       : Disabled
**            Debug mode action                            : Normal operation
**            Fault                                        : Disabled
**            Alternative load                             : Disabled
**            Input filter                                 : Disabled
**          Pins                                           : 
**            Output                                       : Disabled
**            Input 0                                      : Disabled
**            Input 1                                      : Disabled
**            Input 2                                      : Enabled
**              Pin                                        : GPIOC6/TA2/XB_IN3/CMP_REF
**              Pin signal                                 : 
**            Input 3                                      : Disabled
**          Interrupts / DMA                               : 
**            Timer Channel                                : 
**              Interrupt                                  : INT_TMRA_2
**              Timer compare interrupt                    : Disabled
**              Timer overflow interrupt                   : Disabled
**              Input edge interrupt                       : Enabled
**              Compare 1 interrupt                        : Disabled
**              Compare 2 interrupt                        : Disabled
**              Interrupt priority                         : 2
**              ISR name                                   : ISRQTimer1
**              Input edge DMA                             : Disabled
**              Compare 1 DMA                              : Disabled
**              Compare 2 DMA                              : Disabled
**          Registers                                      : 
**            Timer Compare register 1                     : 0000
**            Timer Compare register 2                     : 0000
**            Timer Capture register                       : 0000
**            Timer Load register                          : 0000
**            Timer Counter register                       : 0000
**            Timer Comparator Load register 1             : 0000
**            Timer Comparator Load register 2             : 0000
**          Initialization                                 : 
**            Call Init method                             : yes
**            Enable peripheral clock                      : yes
**            Enable channel                               : yes
**     Contents    :
**         Init - void TMRA2_Init(void);
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
** @file TMRA2.c
** @version 02.09
** @brief
**         This "Init_TMR" Peripheral Inspector implements the
**         Quad Timer module (TMR), basic initialization
**         and settings.
*/         
/*!
**  @addtogroup TMRA2_module TMRA2 module documentation
**  @{
*/         

/* MODULE TMRA2. */

#include "TMRA2.h"

/*
** ===================================================================
**     Method      :  TMRA2_Init (component Init_TMR)
**     Description :
**         This method initializes registers of the TMR module
**         according to this Peripheral Initialization Bean settings.
**         Call this method in the user code to initialize the
**         module. By default, the method is called by PE
**         automatically; see "Call Init method" property of the
**         bean for more details.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TMRA2_Init(void)
{
  /* TMRA_2_CTRL: CM=0,PCS=0,SCS=0,ONCE=0,LENGTH=0,DIR=0,COINIT=0,OUTMODE=0 */
  setReg16(TMRA_2_CTRL, 0x00U);        /* Disable timer */ 
  setReg16(TMRA_2_COMP1, 0x00U);       /* Set the Compare register 1 */ 
  setReg16(TMRA_2_COMP2, 0x00U);       /* Set the Compare register 2 */ 
  setReg16(TMRA_2_CAPT, 0x00U);        /* Set the Capture register */ 
  setReg16(TMRA_2_LOAD, 0x00U);        /* Set the Load register */ 
  setReg16(TMRA_2_CNTR, 0x00U);        /* Set the Counter register */ 
  /* TMRA_2_SCTRL: TCF=0,TCFIE=0,TOF=0,TOFIE=0,IEF=0,IEFIE=1,IPS=0,INPUT=1,CAPTURE_MODE=3,MSTR=0,EEOF=0,VAL=0,FORCE=0,OPS=0,OEN=0 */
  setReg16(TMRA_2_SCTRL, 0x05C0U);     /* Set the Status and control register */ 
  /* TMRA_2_CMPLD1: COMPARATOR_LOAD_1=0 */
  setReg16(TMRA_2_CMPLD1, 0x00U);      /* Set the Comparator load register 1 */ 
  /* TMRA_2_CMPLD2: COMPARATOR_LOAD_2=0 */
  setReg16(TMRA_2_CMPLD2, 0x00U);      /* Set the Comparator load register 2 */ 
  /* TMRA_2_CSCTRL: DBG_EN=0,FAULT=0,ALT_LOAD=0,ROC=0,TCI=0,UP=0,??=0,TCF2EN=0,TCF1EN=0,TCF2=0,TCF1=0,CL2=0,CL1=0 */
  setReg16(TMRA_2_CSCTRL, 0x00U);      /* Set the Comparator status/control register */ 
  /* TMRA_2_FILT: ??=0,??=0,??=0,??=0,??=0,FILT_CNT=0,FILT_PER=0 */
  setReg16(TMRA_2_FILT, 0x00U);        /* Set Filter register */ 
  /* TMRA_2_DMA: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,CMPLD2DE=0,CMPLD1DE=0,IEFDE=0 */
  setReg16(TMRA_2_DMA, 0x00U);         /* Set the DMA register */ 
  /* TMRA_2_CTRL: CM=1,PCS=0x0F,SCS=2,ONCE=0,LENGTH=0,DIR=0,COINIT=0,OUTMODE=0 */
  setReg16(TMRA_2_CTRL, 0x3F00U);      /* Set the Control register */ 
}


/*
** ###################################################################
**
**  The interrupt service routine(s) must be implemented
**  by user in one of the following user modules.
**
**  If the "Generate ISR" option is enabled, Processor Expert generates
**  ISR templates in the CPU event module.
**
**  User modules:
**      main.c
**      Events.c
**
** ###################################################################
#if defined(ISRQTimer1_FAST_INT)
#pragma interrupt fast
#elif defined(ISRQTimer1_VECT_TABLE_ISR_FAST_INT)
#pragma define_section interrupt_fast "interrupt_fast.text"  RX
#pragma section interrupt_fast begin
#pragma interrupt fast
#else
#pragma interrupt saveall
#endif
void ISRQTimer1(void)
{
// NOTE: The routine should include actions to clear the appropriate
//       interrupt flags.
//
}
*/
/* END TMRA2. */

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
