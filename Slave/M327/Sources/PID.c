/*
 * PID.c
 *
 *  Created on: Feb 15, 2016
 *      Author: info
 */


#include "cpu.h"
#include "mlib.h"
#include "GFLib.h"
#include "PWMC1.h"
#include "settings.h"
#include "bldcUser.h"
#include "Ramp.h"
#include "Encoder.h"
#include "PID.h"
#include "Pwm.h"
#include "Plc.h"
#include "Timer.h"

bool kiDisable = FALSE;		// Disable the KI parameter, it used during the homing


frac16_t f16Result[NUM_AXES];		// PID Output value
frac16_t f16InitVal[NUM_AXES];		// PID initial value
frac16_t f16InErr[NUM_AXES];		// Input error for PI
frac16_t f16InErrD[NUM_AXES]; 		// Input error for D
frac16_t f16InErrPrev[NUM_AXES];
bool_t pidStopFlag[NUM_AXES]; 		// PID Flag stop	
GFLIB_CTRL_PID_P_AW_T_A32 pidParam[NUM_AXES];	// PID Parameters

frac16_t f16CurrentResult[NUM_AXES];		// PID Output value
frac16_t f16CurrentInitVal[NUM_AXES];		// PID initial value
frac16_t f16CurrentInErr[NUM_AXES];		// Input error for PI
bool_t pidCurrentStopFlag[NUM_AXES]; 		// PID Flag stop
GFLIB_CTRL_PI_P_AW_T_A32 pidCurrentParam[NUM_AXES];	// PID Current Parameters
word thresholdCurrMaxSup[NUM_AXES];
word thresholdCurrMaxInf[NUM_AXES];
short pidCurrTimeOut[NUM_AXES];
short currentPIDState[NUM_AXES];
long pidErrPos[NUM_AXES];

void PidCurrentServer(byte axe);

void PidInit()
{
	byte i;
	
	for (i = 0; i < NUM_AXES; ++i)
	{
		f16InitVal[i] = FRAC16(0.0);
		pidStopFlag[i] = FALSE;
		GFLIB_CtrlPIDpAWInit_F16(f16InitVal[i], pidParam + i);
		f16InErrPrev[i] = 0;
		
		f16CurrentInitVal[i] = FRAC16(0.0);
		pidCurrentStopFlag[i] = FALSE;
		GFLIB_CtrlPIpAWInit_F16(f16CurrentInitVal[i], pidCurrentParam + i);		
		
		currentPIDState[i] = 0;
	}
	
	

}

void PidReset(byte axe)
{
	f16InitVal[axe] = FRAC16(0.0);
	GFLIB_CtrlPIDpAWInit_F16(f16InitVal[axe], pidParam + axe);
	f16InErrPrev[axe] = 0;
	
	f16CurrentInitVal[axe] = FRAC16(0.0);
	GFLIB_CtrlPIpAWInit_F16(f16CurrentInitVal[axe], pidCurrentParam + axe);	
}

#pragma interrupt called
void PidServer(byte axe)
{	 	
	frac16_t out;
	
	//f16InErr[axe] = MLIB_RndSat_F16l(MLIB_Sub_F32(vpos[axe], rpos[axe]));
	 pidErrPos[axe] = vpos[axe] - rpos[axe];
	 
	 if(pidErrPos[axe] > MAX_16)               /* input error is greater than 0x00007fff = 32767  - 32bit range */
		 f16InErr[axe] = MAX_16;               /* input error = max. positive 16 bit signed value */
	 else if(pidErrPos[axe] < MIN_16)             /* input error is less than 0xffff7fff = -32768    - 32bit range */
		 f16InErr[axe] = MIN_16;             /* input error = min. negative 16 bit signed value */
	 else
		 f16InErr[axe] = pidErrPos[axe] & 0xFFFF; /* input error - 16bit range */

	 	
	f16InErrD[axe] = MLIB_Sub_F16(f16InErr[axe], f16InErrPrev[axe]);
	f16InErrPrev[axe] = f16InErr[axe];
	 
	f16Result[axe] = GFLIB_CtrlPIDpAW_F16(f16InErr[axe], f16InErrD[axe], pidStopFlag + axe, pidParam + axe);
			
	PidCurrentServer(axe);
	
	/*if (f16Result[axe] > 0)
	{
		out = f16Result[axe] + f16CurrentResult[axe];
		
		if (out < 0)
			out = 0;
	}
	else if (f16Result[axe] < 0)
	{
		out = f16Result[axe] - f16CurrentResult[axe];
		
		if (out > 0)
			out = 0;
	}
	else*/
		out = f16Result[axe];
	
	
	
	PwmUpdateOutput(axe, out);
}

void PidCurrentServer(byte axe)
{
	word an0;
	word err;
	
	an0 = ADCValues[axe];
	
	switch (currentPIDState[axe])
	{
	case 0:
		err = thresholdCurrMaxSup[axe] - an0;
		
		if (f16CurrentResult[axe] != 0)
		{
			currentPIDState[axe]++;
			TmrPidCurr(axe) = MSEC(pidCurrTimeOut[axe]);
		}
		
		
		break;
		
	case 1:
		err = thresholdCurrMaxSup[axe] - an0;
		
		
		if (f16CurrentResult[axe] == 0)
			currentPIDState[axe] = 0;
		else if (!TmrPidCurr(axe))
			currentPIDState[axe]++;
		
		break;
		
	case 2:
		err = thresholdCurrMaxInf[axe] - an0;
		if (f16CurrentResult[axe] != 0)
			TmrPidCurr(axe) = MSEC(pidCurrTimeOut[axe]);
		else if (!TmrPidCurr(axe))
			currentPIDState[axe] = 0;
		break;
	}
	
	
		 f16CurrentInErr[axe] = err & 0xFFFF; /* input error - 16bit range */

	
	f16CurrentResult[axe] = GFLIB_CtrlPIpAW_F16(f16CurrentInErr[axe], pidStopFlag + axe, pidCurrentParam + axe);
		
	
	
}
