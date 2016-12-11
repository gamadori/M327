/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : Out1.h
**     Project     : M327
**     Processor   : MC56F84789VLL
**     Component   : BitIO
**     Version     : Component 02.086, Driver 01.20, CPU db: 3.50.001
**     Compiler    : CodeWarrior DSP C Compiler
**     Date/Time   : 2016-11-16, 17:56, # CodeGen: 171
**     Abstract    :
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Note: This component is set to work in Output direction only.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings    :
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       40            |  GPIOB10/ANC22/XB_IN8/MOSI2
**             ----------------------------------------------------
**
**         Port name                   : GPIOB_High
**
**         Bit number (in port)        : 10
**         Bit mask of the port        : 0400
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 0
**         Initial pull option         : off
**
**         Port data register          : GPIOB_DR  [E211]
**         Port control register       : GPIOB_DDR [E212]
**         Port function register      : GPIOB_PER [E213]
**
**         Optimization for            : speed
**     Contents    :
**         GetVal - bool Out1_GetVal(void);
**         PutVal - void Out1_PutVal(bool Val);
**         ClrVal - void Out1_ClrVal(void);
**         SetVal - void Out1_SetVal(void);
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
** @file Out1.h
** @version 01.20
** @brief
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Note: This component is set to work in Output direction only.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
*/         
/*!
**  @addtogroup Out1_module Out1 module documentation
**  @{
*/         

#ifndef __Out1_H
#define __Out1_H

/* MODULE Out1. */

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "Cpu.h"


#define Out1_PIN_MASK           ((word)0x0400) /* Pin mask */
#define Out1_PIN_MASK_SHIFTED   ((byte)0x04) /* Shifted pin mask by 8, if pin mask is greater than 128 */


/*
** ===================================================================
**     Method      :  Out1_GetVal (component BitIO)
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**         Note: This component is set to work in Output direction only.
**     Parameters  : None
**     Returns     :
**         ---             - Input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)

** ===================================================================
*/
#define Out1_GetVal() ((bool)((getReg(GPIOB_DR) >> 8) & Out1_PIN_MASK_SHIFTED))

/*
** ===================================================================
**     Method      :  Out1_PutVal (component BitIO)
**     Description :
**         This method writes the new output value.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Val             - Output value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
**     Returns     : Nothing
** ===================================================================
*/
void Out1_PutVal(bool Val);

/*
** ===================================================================
**     Method      :  Out1_ClrVal (component BitIO)
**     Description :
**         This method clears (sets to zero) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define Out1_ClrVal() (clrRegBits(GPIOB_DR,Out1_PIN_MASK))

/*
** ===================================================================
**     Method      :  Out1_SetVal (component BitIO)
**     Description :
**         This method sets (sets to one) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define Out1_SetVal() (setRegBits(GPIOB_DR,Out1_PIN_MASK))


/* END Out1. */

#endif /* __Out1_H*/
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
