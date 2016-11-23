/** ###################################################################
**
**     (c) Freescale Semiconductor, Inc.
**     2004 All Rights Reserved
**
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

/*****************************************************************************
*
* bldcdrv.c - Brushless DC Motor driver implementation
*
*****************************************************************************/

#include "port.h"
//#include "pwm.h"
#include "bldcdrv.h"

#include "bldc.h"

#if 0   /* the following constant declarations have been moved to const.h */

/*****************************************************************************/
/** Commutation table for EMV Motor Kit and independed PWM mode        **/
/*****************************************************************************/

const mc_tPWMSignalMask   bldcCommutationTableInd[8] = { MC_PWM_ALL_SIGNALS,
   (MC_PWM_SIGNAL_5 | MC_PWM_SIGNAL_3 | MC_PWM_SIGNAL_2 | MC_PWM_SIGNAL_0),
   (MC_PWM_SIGNAL_4 | MC_PWM_SIGNAL_3 | MC_PWM_SIGNAL_1 | MC_PWM_SIGNAL_0),
   (MC_PWM_SIGNAL_5 | MC_PWM_SIGNAL_4 | MC_PWM_SIGNAL_3 | MC_PWM_SIGNAL_0),
   (MC_PWM_SIGNAL_5 | MC_PWM_SIGNAL_4 | MC_PWM_SIGNAL_2 | MC_PWM_SIGNAL_1),
   (MC_PWM_SIGNAL_5 | MC_PWM_SIGNAL_2 | MC_PWM_SIGNAL_1 | MC_PWM_SIGNAL_0),
   (MC_PWM_SIGNAL_4 | MC_PWM_SIGNAL_3 | MC_PWM_SIGNAL_2 | MC_PWM_SIGNAL_1),
                                  MC_PWM_ALL_SIGNALS
                                };

/*****************************************************************************/
/** Commutation table for EMV Motor Kit and complementary PWM mode      **/
/*****************************************************************************/

const pwm_sChannelControl bldcCommutationTableComp[8] = {
            { MC_PWM_ALL_SIGNALS, PWM_ZERO_MASK },
      { (MC_PWM_SIGNAL_3 | MC_PWM_SIGNAL_2), (PWM_SWAP_1 | PWM_SWAP_0) },
      { (MC_PWM_SIGNAL_1 | MC_PWM_SIGNAL_0), (PWM_SWAP_2 | PWM_SWAP_0) },
      { (MC_PWM_SIGNAL_5 | MC_PWM_SIGNAL_4), (PWM_SWAP_0) },
      { (MC_PWM_SIGNAL_5 | MC_PWM_SIGNAL_4), (PWM_SWAP_2 | PWM_SWAP_1) },
      { (MC_PWM_SIGNAL_1 | MC_PWM_SIGNAL_0), (PWM_SWAP_1) },
      { (MC_PWM_SIGNAL_3 | MC_PWM_SIGNAL_2), (PWM_SWAP_2) },
       { MC_PWM_ALL_SIGNALS, PWM_ZERO_MASK },
                            };


/*****************************************************************************/
/** Commutation table for sensorless control                                **/
/**               EMV,LV,HV Motor Kits and independed PWM mode              **/
/*****************************************************************************/
const mc_tPWMSignalMask  BldcZC_Cmt_StepTable[] = {
       (MC_PWM_SIGNAL_5 | MC_PWM_SIGNAL_4 | MC_PWM_SIGNAL_2 | MC_PWM_SIGNAL_1),
       (MC_PWM_SIGNAL_5 | MC_PWM_SIGNAL_2 | MC_PWM_SIGNAL_1 | MC_PWM_SIGNAL_0),
       (MC_PWM_SIGNAL_5 | MC_PWM_SIGNAL_3 | MC_PWM_SIGNAL_2 | MC_PWM_SIGNAL_0),
       (MC_PWM_SIGNAL_5 | MC_PWM_SIGNAL_4 | MC_PWM_SIGNAL_3 | MC_PWM_SIGNAL_0),
       (MC_PWM_SIGNAL_4 | MC_PWM_SIGNAL_3 | MC_PWM_SIGNAL_1 | MC_PWM_SIGNAL_0),
       (MC_PWM_SIGNAL_4 | MC_PWM_SIGNAL_3 | MC_PWM_SIGNAL_2 | MC_PWM_SIGNAL_1)
                                                    };

/*****************************************************************************/
/** Commutation table for sensorless control                                **/
/**               EMV,LV,HV Motor Kits and complementary PWM mode          **/
/*****************************************************************************/
const pwm_sChannelControl  BldcZC_Cmt_StepTableComp [MAX_STEP_CMT+1] = {
            { (MC_PWM_SIGNAL_5 | MC_PWM_SIGNAL_4), (PWM_SWAP_2 | PWM_SWAP_1) },
            { (MC_PWM_SIGNAL_1 | MC_PWM_SIGNAL_0), (PWM_SWAP_1) },
            { (MC_PWM_SIGNAL_3 | MC_PWM_SIGNAL_2), (PWM_SWAP_1 | PWM_SWAP_0) },
            { (MC_PWM_SIGNAL_5 | MC_PWM_SIGNAL_4), (PWM_SWAP_0) },
            { (MC_PWM_SIGNAL_1 | MC_PWM_SIGNAL_0), (PWM_SWAP_2 | PWM_SWAP_0) },
            { (MC_PWM_SIGNAL_3 | MC_PWM_SIGNAL_2), (PWM_SWAP_2) }
                                                                        };


/**************************************************************************************/
/** Zero Crossing tables for sensorless Zero Crossing EMV,LV,HV Motor Kits PWM mode  **/
/** for (level)Zero Crosing bldczc algorithms (used with HW comparator Zero Crossing)**/
/**************************************************************************************/

const UWord16 Mask_ZCInpTab[MAX_STEP_CMT+1] = { MASK_ZC_PHASE_C, MASK_ZC_PHASE_A, MASK_ZC_PHASE_B,\
                                                MASK_ZC_PHASE_C, MASK_ZC_PHASE_A, MASK_ZC_PHASE_B };

const UWord16 Expect_ZCInp_Tab [ MAX_STEP_CMT+1 ] [2] = \
{ {ZCINP_PHASE_C_Lev1,ZCINP_PHASE_C_Lev0},{ZCINP_PHASE_A_Lev0,ZCINP_PHASE_A_Lev1},\
  {ZCINP_PHASE_B_Lev1,ZCINP_PHASE_B_Lev0},{ZCINP_PHASE_C_Lev0,ZCINP_PHASE_C_Lev1},\
  {ZCINP_PHASE_A_Lev1,ZCINP_PHASE_A_Lev0},{ZCINP_PHASE_B_Lev0,ZCINP_PHASE_B_Lev1} };

/*************************************************************************************/
/** Zero Crossing tables for sensorless Zero Crossing EMV,LV,HV Motor Kits PWM mode **/
/** for edge Zero Crosing bldczc algorithms (used with ADC Zero Crossing)           **/
/*************************************************************************************/

const UWord16 SetADC_ZCInp_Tab [ MAX_STEP_CMT+1 ] [ 2 ] =
{ {ADCZC_PHASE_C_NEG_POS,ADCZC_PHASE_C_POS_NEG},{ADCZC_PHASE_A_POS_NEG,ADCZC_PHASE_A_NEG_POS},\
  {ADCZC_PHASE_B_NEG_POS,ADCZC_PHASE_B_POS_NEG},{ADCZC_PHASE_C_POS_NEG,ADCZC_PHASE_C_NEG_POS},\
  {ADCZC_PHASE_A_NEG_POS,ADCZC_PHASE_A_POS_NEG},{ADCZC_PHASE_B_POS_NEG,ADCZC_PHASE_B_NEG_POS} };

const Word16 ZC_Phase_Tab [] = { INDEX_ZC_PHASE_C, INDEX_ZC_PHASE_A, INDEX_ZC_PHASE_B, \
                                 INDEX_ZC_PHASE_C, INDEX_ZC_PHASE_A, INDEX_ZC_PHASE_B };
const int Expect_ZCInpFlag_Tab [ MAX_STEP_CMT+1 ] [2] = \
{ {1,0},{0,1},\
  {1,0},{0,1},\
  {1,0},{0,1} };

#endif
