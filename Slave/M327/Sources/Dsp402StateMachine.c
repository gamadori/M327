/** ###################################################################
**     Filename  : 402StateMachine.C
**     Project   : MP_M307_SLAVE
**     Processor : 56F8323
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 31/05/2011, 16.14
**     Contents  :
**         User source code
** 
** ###################################################################*/

/* MODULE 402StateMachine */
#include "cpu.h"
#include "Bits.h"
#include "Dsp402StateMachine.h"
#include "Dsp402PositionMode.h"
#include "Dsp402Homing.h"
#include "Encoder.h"
#include "Pid.h"
#include "Plc.h"
#include "Drive.h"
#include "Ramp.h"

td_DSP402 devices402[NUM_MOTORS];

word macStatus = 0;

void DSP402InitMotor(td_DSP402 *pDev);

void OperationEnableServer(td_DSP402 *pDev);

void ResetOperationEnable(td_DSP402 *pDev);

/*
 * Inizializza la macchina DSP402
 */
void DSP402InitMachine()
{
	word i;
	
	for (i = 0; i < NUM_MOTORS; ++i)
	{
		
	
		PositionInit(&devices402[i], i);
		HomingInit(&devices402[i], i);
	}
}


void DSP402InitMotor(td_DSP402 *pDev)
{
	pDev->modeOperation = -1;
	pDev->controlWord.w = 0;
	pDev->statusWord.w = 0;
		
	
	SetDeviceNotReady(pDev->statusWord.w);
}

void DSP402Server()
{
	short i;
	
	for (i = 0; i < NUM_MOTORS; ++i)
	{
		DeviceControlServer(&devices402[i]);
		
		devices402[0].positionActual = rpos[0];
		
	}
}
/*
 * Verifica se si verifica l'evento Shutdown
 */
bool CWShutdown(td_DSP402 *pDev)
{
	if (!pDev->controlWord.pp.faultReset && pDev->controlWord.pp.quickStop == 1 && 
		pDev->controlWord.pp.enableVoltage == 1
			&& !pDev->controlWord.pp.switchOn)
		return TRUE;
	else
		return FALSE;
}

bool CWSwitchOn(td_DSP402 *pDev)
{
	if (!pDev->controlWord.pp.faultReset && !pDev->controlWord.pp.enableOperation && 
		pDev->controlWord.pp.quickStop == 1 &&
		pDev->controlWord.pp.enableVoltage == 1 && pDev->controlWord.pp.switchOn == 1)
		return TRUE;
	else
		return FALSE;
}


bool CWDisableVoltage(td_DSP402 *pDev)
{
	if (!pDev->controlWord.pp.faultReset && !pDev->controlWord.pp.enableVoltage)
		return TRUE;
	else
		return FALSE;
}


bool CWQuickStop(td_DSP402 *pDev)
{
	if (!pDev->controlWord.pp.faultReset && !pDev->controlWord.pp.quickStop && 
		pDev->controlWord.pp.enableVoltage == 1)
		return TRUE;
	else
		return FALSE;
}

bool CWDisableOperation(td_DSP402 *pDev)
{
	if (!pDev->controlWord.pp.faultReset && pDev->controlWord.pp.quickStop && 
		pDev->controlWord.pp.enableVoltage == 1 && pDev->controlWord.pp.enableOperation == 0)
		return TRUE;
	else
		return FALSE;
	
}


bool CWEnableOperation(td_DSP402 *pDev)
{
	if (!pDev->controlWord.pp.faultReset && pDev->controlWord.pp.enableOperation == 1 && 
		pDev->controlWord.pp.quickStop == 1 && pDev->controlWord.pp.enableVoltage == 1 &&
			pDev->controlWord.pp.switchOn == 1)
		return TRUE;
	else
		return FALSE;
		
}


bool CWFaultRest(td_DSP402 *pDev)
{
	if (pDev->controlWord.pp.faultReset)
		return TRUE;
	else
		return FALSE;
}


void DeviceControlServer(td_DSP402 *pDev)
{
	long status;
	
	if ((pDev->statusWord.w & DeviceSwitchOnDisable) == DeviceSwitchOnDisable)
	{
		
		
		if (inpPwrOn)
		{
			
			SetDeviceOperationEnable(pDev->statusWord.w);	//(4)
			DriveCommandEnable(0, TRUE);
			
		}
		else
		{
			
			DriveCommandEnable(0, FALSE);	
		}
	}
	else if ((pDev->statusWord.w & MskDeviceOperationEnable) == DeviceOperationEnable)
	{ 
		
		if (!inpPwrOn)
		{
			PositionReset(pDev);
			SetDeviceSwitchOnDisable(pDev->statusWord.w);	// (1)
		}
		else
		{
			OperationEnableServer(pDev);
		}
	}
	
	else
	{
		SetDeviceSwitchOnDisable(pDev->statusWord.w);	// (1)
	}
	

}

/*
void DeviceControlServer(td_DSP402 *pDev)
{
	short tmp = 0;

	if (pDev->controlWord.pp.enableVoltage)
		pDev->statusWord.pp.voltageEnabled = 1;
	else
		pDev->statusWord.pp.voltageEnabled = 0;

	if ((pDev->statusWord.w & MskDeviceNotReady) == DeviceNotReady)
	{
		// Inizializzazione e Test del Dispositivo
		macStatus = DeviceNotReady;
		
		pDev->controlWord.w |= 0x0040;
		SetDeviceSwitchOnDisable(pDev->statusWord.w);	// (1)
	
	}
	else if ((pDev->statusWord.w & DeviceSwitchOnDisable) == DeviceSwitchOnDisable)
	{
		// Switch On Disable
		macStatus = DeviceSwitchOnDisable;
		
		if (CWShutdown(pDev))
		{
			SetDeviceReadytoSwitchOn(pDev->statusWord.w); // (2)
		}
	}
	else if ((pDev->statusWord.w & MskDeviceReadytoSwitchOn) == DeviceReadytoSwitchOn)
	{
		// Ready To Switch On
		
		macStatus = DeviceReadytoSwitchOn;
		
		if (CWDisableVoltage(pDev)) // (7)
		{
			SetDeviceSwitchOnDisable(pDev->statusWord.w);
		}
		else if (CWSwitchOn(pDev))	// (3)
		{
			SetDeviceSwitchOn(pDev->statusWord.w);
		}
	}
	
	else if ((pDev->statusWord.w & MskDeviceSwitchOn) == DeviceSwitchOn)
	{
		// Switched On
		
		macStatus = DeviceSwitchOn;
		
		if (CWShutdown(pDev))	// (6)
		{
			SetDeviceReadytoSwitchOn(pDev->controlWord.w);	//(6)
		}
		else if (CWEnableOperation(pDev))
		{
			ResetOperationEnable(pDev);
			SetDeviceOperationEnable(pDev->statusWord.w);	//(4)
		}
		else if (CWDisableVoltage(pDev))
		{
			SetDeviceSwitchOnDisable(pDev->statusWord.w);
		}
		
	}
	else if ((pDev->statusWord.w & MskDeviceOperationEnable) == DeviceOperationEnable)
	{
		
		macStatus = DeviceOperationEnable;
		
	
		// Operation Enable
		if (CWDisableOperation(pDev))
		{
			
			SetDeviceSwitchOn(pDev->controlWord.w);	// (5)
		}
		else if (CWShutdown(pDev) || CWDisableVoltage(pDev))
		{
			
			SetDeviceSwitchOnDisable(pDev->statusWord.w);	//(8) (9)
		}
		else if (CWQuickStop(pDev))
		{
			
			SetDeviceQuickStopActive(pDev->statusWord.w);	// (11)
		}
		else
		{
			OperationEnableServer(pDev);
		}
		
	}
	
	else if ((pDev->statusWord.w & MskDeviceFaultReactionActive) == DeviceFaultReactionActive)
	{
		
	}
	else if ((pDev->statusWord.w & MskDeviceFault) == DeviceFault)
	{
		
	}
	
	if ((pDev->statusWord.w & MskDeviceOperationEnable) != DeviceOperationEnable)
	{
		HaltCommand(pDev);
	}
	
	pDev->modeOperationDisplay = pDev->modeOperation;
	
	if (pDev->modeOperationDisplay != 0)
		pDev->modeOperationDisplay = pDev->modeOperation;

	
}

*/

/*=======================================
 *
 * Richiama le funzioni che devono
 * essere eseguite quando viene lasciato
 * lo stato "Operation Enable"
 *
 *======================================*/
void ResetOperationEnable(td_DSP402 *pDev)
{

	PositionReset(pDev);
	HomingReset(pDev);
}


void OperationEnableServer(td_DSP402 *pDev)
{
	if (pDev->controlWord.pp.halt)
	{
		HomingReset(pDev);
		HaltCommand(pDev);
	}
	else
	{
		switch (pDev->modeOperation)
		{
			
			case moJogModeNeg:
				pDev->modeOperationDisplay = pDev->modeOperation;
				JogModeServerNeg(pDev);
				
				break;
			
			
			case moJogModePos:
				pDev->modeOperationDisplay = pDev->modeOperation;
				JogModeServerPos(pDev);
				
				break;
				
			case moProfilePositionMode:
				pDev->modeOperationDisplay = pDev->modeOperation;
				PositionModeServer(pDev);
				break;
			
				
			case moHomingMode:
				
				
				if (pDev->modeOperation != pDev->modeOperationDisplay)
				{
					pDev->modeOperationDisplay = pDev->modeOperation;
					HomingReset(pDev);
					
				}
				HomingServer(pDev);
				break;
				
			case moHomingSearch:
			
				if (pDev->modeOperation != pDev->modeOperationDisplay)
				{
					pDev->modeOperationDisplay = pDev->modeOperation;
					HomingReset(pDev);
					
				}
				HomingServer(pDev);
				break;
				
			case moResetEncoder2:
				rpos[1] = 0;
				break;
				
			
			
			
		}
	}
	
//	AlarmServer(pDev);
}

void HaltCommand(td_DSP402 *pDev)
{
	RampStopCommand(0, pDev->pp->maxAcceleration);
}




/* END 402StateMachine */
