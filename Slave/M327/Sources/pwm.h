/*
 * pwm.h
 *
 *  Created on: Feb 8, 2016
 *      Author: gianluca
 */

#ifndef PWM_H_
#define PWM_H_

#define VOLTAGE_OFFSET			   	0x3FFF	/* Zero voltage is at 50% duty cycle */
#define VOLTAGE_UP_LIMIT		    16383 - 100	/* max. output from PI controller */
#define VOLTAGE_DOWN_LIMIT		   -16383 + 100	/* min. output from PI controller */

#define DC_VOLTAGE_UP_LIMIT		    32767 - 100	/* max. output from PI controller */
#define DC_VOLTAGE_DOWN_LIMIT		-32767 + 100	/* min. output from PI controller */

#define SPEED_UP_LIMIT		    	32000	//21840 	/* max. allowable speed 1000 rpm (1000/1500*32767) */
#define SPEED_DOWN_LIMIT		   -32000	//21840 	/* min. allowable speed -1000 rpm (-1000/1500*32767) */

#define MASK_MASKA_MASK		0x0F00
#define MASK_MASKA_SUB3		0x0800
#define MASK_MASKA_SUB2		0x0400
#define MASK_MASKA_SUB1		0x0200
#define MASK_MASKA_SUB0		0x0100
#define MASK_MASKB_MASK		0x00F0
#define MASK_MASKB_SUB3		0x0080
#define MASK_MASKB_SUB2		0x0040
#define MASK_MASKB_SUB1		0x0020
#define MASK_MASKB_SUB0		0x0010


typedef struct
{
	PWMC1_TPWMMC_Duty  pwmChannel_0_Value;
	PWMC1_TPWMMC_Duty  pwmChannel_1_Value;
	PWMC1_TPWMMC_Duty  pwmChannel_2_Value;
	PWMC1_TPWMMC_Duty  pwmChannel_3_Value;
	PWMC1_TPWMMC_Duty  pwmChannel_4_Value;
	PWMC1_TPWMMC_Duty  pwmChannel_5_Value;
}pwm_sIndependentValues;


void pwmUpdateValueRegistersIndependent(pwm_sIndependentValues *pDutyCycle);

void PWMCommutationHS(byte sector);

void PWMInit();

void UpdatePWMControl(byte axe, bool status);

void PwmUpdateOutput(byte axe, long speed);

#endif /* PWM_H_ */
