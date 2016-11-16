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
* bldcdrv.h - header file for Brushless DC Motor driver
*
*****************************************************************************/


#ifndef __BLDCDRV_H
#define __BLDCDRV_H


#include "port.h"
//#include "pwm.h"
#include "bldc.h"

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/
/** Zero Crossing table for sensorless Zero Crossing EMV Motor Kit  PWM mode **/
/******************************************************************************/

/* Constant Definitions */
#define MAX_STEP_CMT        5
#define MIN_STEP_CMT        0

#define MASK_ZC_PHASE_A  0x0004       /* Input Zero crosing mask for phase A
                                         IMR register (ArchIO.Decoder.InputMonitorReg)
                                         flag FPHA */
#define MASK_ZC_PHASE_B  0x0002       /* Input Zero crosing mask for phase B
                                         IMR register (ArchIO.Decoder.InputMonitorReg)
                                         flag FPHB */
#define MASK_ZC_PHASE_C  0x0001       /* Input Zero crosing mask for phase C
                                         IMR register (ArchIO.Decoder.InputMonitorReg)
                                         flag FIND */
#define ZCINP_PHASE_A_Lev0  0x0000    /* Zero Crossing Sample Output Level0 */
#define ZCINP_PHASE_A_Lev1  0x0004    /* Zero Crossing Sample Output Level0 */
#define ZCINP_PHASE_B_Lev0  0x0000    /* Zero Crossing Sample Output Level0 */
#define ZCINP_PHASE_B_Lev1  0x0002    /* Zero Crossing Sample Output Level0 */
#define ZCINP_PHASE_C_Lev0  0x0000    /* Zero Crossing Sample Output Level0 */
#define ZCINP_PHASE_C_Lev1  0x0001    /* Zero Crossing Sample Output Level0 */

#define ADCZC_PHASE_C_ANY     0x0030       /* ADC sample2 Zero Crossing from Positive to Negative */
#define ADCZC_PHASE_C_POS_NEG 0x0010       /* ADC sample2 Zero Crossing from Positive to Negative */
#define ADCZC_PHASE_C_NEG_POS 0x0020       /* ADC sample2 Zero Crossing from Negative to Positive */
#define ADCZC_PHASE_B_ANY     0x000c       /* ADC sample1 Zero Crossing from Positive to Negative */
#define ADCZC_PHASE_B_POS_NEG 0x0004       /* ADC sample1 Zero Crossing from Positive to Negative */
#define ADCZC_PHASE_B_NEG_POS 0x0008       /* ADC sample1 Zero Crossing from Negative to Positive */
#define ADCZC_PHASE_A_ANY     0x0003       /* ADC sample0 Zero Crossing from Negative to Positive */
#define ADCZC_PHASE_A_POS_NEG 0x0001       /* ADC sample0 Zero Crossing from Positive to Negative */
#define ADCZC_PHASE_A_NEG_POS 0x0002       /* ADC sample0 Zero Crossing from Negative to Positive */
#define ADCZC_ALL_DISABLE     0x0000       /* ADC Zero Crossing all disabled */

#define INDEX_ZC_PHASE_A           0       /* phase (selection) index phase A */
#define INDEX_ZC_PHASE_B           1       /* phase (selection) index phase B */
#define INDEX_ZC_PHASE_C           2       /* phase (selection) index phase C */

/**************************************************************************************/
/** Zero Crossing tables for sensorless Zero Crossing EMV,LV,HV Motor Kits PWM mode  **/
/** for (level)Zero Crosing bldczc algorithms (used with HW comparator Zero Crossing)**/
/**************************************************************************************/
EXPORT const UWord16 Mask_ZCInpTab[ MAX_STEP_CMT+1 ];
EXPORT const UWord16 Expect_ZCInp_Tab [ MAX_STEP_CMT+1 ] [2];

/*****************************************************************************/
/** Commutation table for sensorless control                                **/
/**               EMV,LV,HV Motor Kits and independed PWM mode              **/
/*****************************************************************************/
EXPORT const mc_tPWMSignalMask  BldcZC_Cmt_StepTable[];

/*****************************************************************************/
/** Commutation table for sensorless control                                **/
/**               EMV,LV,HV Motor Kits and complementary PWM mode          **/
/*****************************************************************************/
EXPORT const pwm_sChannelControl  BldcZC_Cmt_StepTableComp[MAX_STEP_CMT+1];

/*************************************************************************************/
/** Zero Crossing tables for sensorless Zero Crossing EMV,LV,HV Motor Kits PWM mode **/
/** for edge Zero Crosing bldczc algorithms (used with ADC Zero Crossing)           **/
/*************************************************************************************/
EXPORT const UWord16 SetADC_ZCInp_Tab [ MAX_STEP_CMT+1 ] [ 2 ];
EXPORT const Word16 ZC_Phase_Tab [];
EXPORT const int Expect_ZCInpFlag_Tab [ MAX_STEP_CMT+1 ] [2];

#ifdef __cplusplus
}
#endif

#endif
