/*
 * bldcUser.c
 *
 *  Created on: Feb 6, 2016
 *      Author: info
 */

#include "cpu.h"
/*#include "TMRPWM1.h"
#include "TMRPWM2.h"
#include "TMRPWM3.h"*/
#include "MCBLDC.h"
#include "PWMC1.h"


#include "Settings.h"
#include "pwm.h"
#include "bldcUser.h"

//#define InpHall0_PIN_MASK       ((byte)0x08) /* Pin mask */

#define InpHall0Val ((bool)(getRegBits(TMRA_0_SCTRL, TMRA_0_SCTRL_INPUT_MASK) != 0))
#define InpHall1Val ((bool)(getRegBits(TMRA_1_SCTRL, TMRA_1_SCTRL_INPUT_MASK) != 0))
#define InpHall2Val ((bool)(getRegBits(TMRA_2_SCTRL, TMRA_2_SCTRL_INPUT_MASK) != 0))

/*
#define InpHall0Val (Bit1_GetVal() != 0)
#define InpHall1Val (Bit2_GetVal() != 0)
#define InpHall2Val (Bit3_GetVal() != 0)
*/

byte bldcSensoresState[NUM_AXES];						/* Stato dei Hall Sensor */			

short bldcVoltageDes[NUM_AXES];					/* Contiene il valore di uscita del PWM */

bool bldcEnabled[NUM_AXES];

bool bldcStarted[NUM_AXES];



/*
 * 
 */
void BldcInit(void)
{
	short i;
	pwm_sChannelControl	PWMsStates;
	
	for (i = 0; i < NUM_AXES; ++i)
	{
		bldcEnabled[i] = FALSE;
		bldcStarted[i] = FALSE;
	}
	//TMRPWM1_Init();
	//TMRPWM2_Init();
	//TMRPWM3_Init();
	
	BldcGetHall();
	
	PWMInit();
	
  	
}

#pragma interrupt called
void BldcTimer()
{
	BldcGetHall();
	
	if (bldcEnabled[0])
		PWMCommutationHS(bldcSensoresState[0]);
	else
		PWMCommutationHS(0);
		
}
#pragma interrupt saveall
void ISRQTimer1()
{
	BldcTimer();
	
	clrRegBits(TMRA_0_SCTRL, TMRA_0_SCTRL_IEF_MASK);
	clrRegBits(TMRA_1_SCTRL, TMRA_0_SCTRL_IEF_MASK);
	clrRegBits(TMRA_2_SCTRL, TMRA_0_SCTRL_IEF_MASK);
}

#pragma interrupt called
void BldcGetHall()
{
	
	bldcSensoresState[0] = ((InpHall0Val  << 2) | (InpHall1Val << 1) | 
							(InpHall2Val)) & 0x07;
}

#pragma interrupt called
void BldcPWMOut(byte axe, bool status)
{
	pwm_sIndependentValues	DutyCycleValues;
		
	if (bldcStarted[axe] != bldcEnabled[axe])
	{		
		BldcTimer();	
		bldcStarted[axe] = bldcEnabled[axe];
	}
		
	DutyCycleValues.pwmChannel_0_Value = VOLTAGE_OFFSET + bldcVoltageDes[axe];
  	DutyCycleValues.pwmChannel_1_Value = 32767 - VOLTAGE_OFFSET - bldcVoltageDes[axe];
  	DutyCycleValues.pwmChannel_2_Value = VOLTAGE_OFFSET + bldcVoltageDes[axe];
  	DutyCycleValues.pwmChannel_3_Value = 32767 - VOLTAGE_OFFSET - bldcVoltageDes[axe];
  	DutyCycleValues.pwmChannel_4_Value = VOLTAGE_OFFSET + bldcVoltageDes[axe];
  	DutyCycleValues.pwmChannel_5_Value = 32767 - VOLTAGE_OFFSET - bldcVoltageDes[axe];
  
  	/* set new PWM duty cycle */
  	pwmUpdateValueRegistersIndependent(&DutyCycleValues);	
}

#pragma interrupt called
void BldcUpdateSpeed(byte axe, long speed)
{
	if (speed > VOLTAGE_UP_LIMIT)
		speed = VOLTAGE_UP_LIMIT;
	else if (speed < VOLTAGE_DOWN_LIMIT)
		speed = VOLTAGE_DOWN_LIMIT;
	
	
	bldcVoltageDes[axe] =  (short) speed;	
	
	BldcPWMOut(axe, TRUE);
	
}

void BldcSetStatus(byte axe, bool status)
{
	bldcEnabled[axe] = status;
}
