/*
 * pwm.c
 *
 *  Created on: Feb 7, 2016
 *      Author: gianluca
 */

 
#include "cpu.h"
#include "PWMC1.h"
#include "mlib.h"

#include "bldcUser.h"
#include "pwm.h"

#define SM0SEL_NEG_PWM23	0x0004
#define SM1SEL_NEG_PWM23	0x0040
#define SM2SEL_NEG_PWM23	0x0400


/*word DTSRCSELTable[] = 
{
		(0x00),									// 0
		(SM2SEL_NEG_PWM23),						// 1
		(SM2SEL_NEG_PWM23 | SM0SEL_NEG_PWM23),	// 2
		(SM0SEL_NEG_PWM23),						// 3
		(SM1SEL_NEG_PWM23 | SM0SEL_NEG_PWM23),	// 4
		(SM1SEL_NEG_PWM23),						// 5
		(SM2SEL_NEG_PWM23 | SM1SEL_NEG_PWM23)	// 6
};*/


/*
word MASKTable[] = 
{
		(MASK_MASKA_MASK|MASK_MASKB_MASK),		// 0
		(MASK_MASKA_SUB1|MASK_MASKB_SUB1),		// 1
		(MASK_MASKA_SUB0| MASK_MASKB_SUB0),		// 2
		(MASK_MASKA_SUB2| MASK_MASKB_SUB2),		// 3
		(MASK_MASKA_SUB1| MASK_MASKB_SUB1),		// 4
		(MASK_MASKA_SUB0| MASK_MASKB_SUB0),		// 5
		(MASK_MASKA_SUB2| MASK_MASKB_SUB2) 		// 6
};*/

word DTSRCSELTable[] = 
{
		(0x00),									// 0
		(SM0SEL_NEG_PWM23 | SM1SEL_NEG_PWM23),	// 1
		(SM0SEL_NEG_PWM23 | SM2SEL_NEG_PWM23),	// 2
		(SM0SEL_NEG_PWM23),						// 3
		(SM1SEL_NEG_PWM23 | SM2SEL_NEG_PWM23),	// 4
		(SM1SEL_NEG_PWM23),						// 5
		(SM2SEL_NEG_PWM23)						// 6
};

word MASKTable[] = 
{
		(MASK_MASKA_MASK|MASK_MASKB_MASK),		// 0
		(MASK_MASKA_SUB1|MASK_MASKB_SUB1),		// 1
		(MASK_MASKA_SUB0| MASK_MASKB_SUB0),		// 2
		(MASK_MASKA_SUB2| MASK_MASKB_SUB2),		// 3
		(MASK_MASKA_SUB2| MASK_MASKB_SUB2),		// 4
		(MASK_MASKA_SUB0| MASK_MASKB_SUB0),		// 5
		(MASK_MASKA_SUB1| MASK_MASKB_SUB1) 		// 6
};

void PWMInit()
{
	PWMCommutationHS(0);
}

void PWMCommutationHS(byte sector)
{
	PWMA_DTSRCSEL = DTSRCSELTable[sector];
	PWMA_MASK = MASKTable[sector];
	
	/* for s/w manually triggered commutation, submodule 0 FORCE = 1 */
	PWMA_SM0CTRL2 |= 0x0040;
	PWMA_SM1CTRL2 |= 0x0040;
	PWMA_SM2CTRL2 |= 0x0040;
}

/******************************************************************************* 
* 
* Module: pwmUpdateValueRegistersIndependent(pwm_sIndependentValues *pDutyCycle)
* 
* Description: 
*     	The method calculates the actual contents of the data registers
*       based on the required percentage of duty cycles and the PWM modulus value. 
* 		This values are load after updating of the value registers. 
*		This function is used in the Independent PWM operating mode.
*		
* Returns: None
* 
* Arguments:
*		word axe: indice dell'asse
*		pwm_sIndependentValues *pDutyCycle - value of the duty
* 
*******************************************************************************/
#pragma interrupt saveall
void pwmUpdateValueRegistersIndependent(pwm_sIndependentValues *pDutyCycle)
{
	word pwmModulus;

	pwmModulus = (word)((Int32)getReg16(PWMA_SM0VAL1) - (Int16)getReg16(PWMA_SM0INIT) + 0x01);
	//pwmModulus = (Word16) getReg(PWMA_PWMCM); /* get Counter Modulo Register */	
  	
	
	PWMC1_SetDuty(0, MLIB_MulSat_F16(pwmModulus, pDutyCycle->pwmChannel_0_Value)); /* set Duty of the channel 0 */
	PWMC1_SetDuty(1, MLIB_MulSat_F16(pwmModulus, pDutyCycle->pwmChannel_1_Value)); /* set Duty of the channel 1 */
	PWMC1_SetDuty(2, MLIB_MulSat_F16(pwmModulus, pDutyCycle->pwmChannel_2_Value)); /* set Duty of the channel 2 */
	PWMC1_SetDuty(3, MLIB_MulSat_F16(pwmModulus, pDutyCycle->pwmChannel_3_Value)); /* set Duty of the channel 3 */
	PWMC1_SetDuty(4, MLIB_MulSat_F16(pwmModulus, pDutyCycle->pwmChannel_4_Value)); /* set Duty of the channel 4 */
	PWMC1_SetDuty(5, MLIB_MulSat_F16(pwmModulus, pDutyCycle->pwmChannel_5_Value)); /* set Duty of the channel 5 */
	
	PWMC1_Load();                                                        /* load new values */


} 

/*********************************************************
 * Module: void UpdatePWMControl(void)
 *+
 * Desription:
 *	Aggiorna il valore di uscita del PWM per il controllo
 * degli assi
 *
 **********************************************************/
#pragma interrupt called
void UpdatePWMControl(byte axe, bool status)
{
	//BldcPWMOut(axe, status);
		
}

#pragma interrupt called
void PwmUpdateOutput(byte axe, long speed)
{

	BldcUpdateSpeed(axe, speed);
}
