/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : TMRA1.h
**     Project     : M327
**     Processor   : MC56F84789VLL
**     Component   : Init_TMR
**     Version     : Component 01.161, Driver 02.09, CPU db: 3.50.001
**     Compiler    : CodeWarrior DSP C Compiler
**     Date/Time   : 2016-11-16, 17:56, # CodeGen: 171
**     Abstract    :
**         This "Init_TMR" Peripheral Inspector implements the
**         Quad Timer module (TMR), basic initialization
**         and settings.
**     Settings    :
**          Component name                                 : TMRA1
**          Device                                         : TMRA1
**          Settings                                       : 
**            Clock settings                               : 
**              Primary source                             : 
**                Primary source                           : prescaler (IP BUS clock / 128)
**              Secondary source                           : 
**                Secondary source                         : counter 1 input pin
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
**            Input 1                                      : Enabled
**              Pin                                        : GPIOC4/TA1/CMPB_O/XB_IN8/EWM_OUT_B
**              Pin signal                                 : 
**            Input 2                                      : Disabled
**            Input 3                                      : Disabled
**          Interrupts / DMA                               : 
**            Timer Channel                                : 
**              Interrupt                                  : INT_TMRA_1
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
**         Init - void TMRA1_Init(void);
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
** @file TMRA1.h
** @version 02.09
** @brief
**         This "Init_TMR" Peripheral Inspector implements the
**         Quad Timer module (TMR), basic initialization
**         and settings.
*/         
/*!
**  @addtogroup TMRA1_module TMRA1 module documentation
**  @{
*/         

#ifndef __TMRA1
#define __TMRA1

/* MODULE TMRA1. */

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "Cpu.h"

/* PESL device mapping  */
#define TMRA1_DEVICE TMRA1


void TMRA1_Init(void);
/*
** ===================================================================
**     Method      :  TMRA1_Init (component Init_TMR)
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

void ISRQTimer1(void);
/*
** ===================================================================
** The interrupt service routine must be implemented by user in one
** of the user modules (see TMRA1.c file for more information).
** ===================================================================
*/

/* END TMRA1. */

#endif /* ifndef __TMRA1 */
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
