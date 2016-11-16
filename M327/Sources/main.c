/* ###################################################################
**     Filename    : main.c
**     Project     : M327
**     Processor   : MC56F84789VLL
**     Version     : Driver 01.16
**     Compiler    : CodeWarrior DSP C Compiler
**     Date/Time   : 2016-06-12, 15:52, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.16
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "AS1.h"
#include "SM1.h"
#include "CSInput.h"
#include "Out0.h"
#include "Out1.h"
#include "Out2.h"
#include "Out3.h"
#include "Out4.h"
#include "Out5.h"
#include "Out6.h"
#include "Out7.h"
#include "Out8.h"
#include "Out9.h"
#include "Out10.h"
#include "Out11.h"
#include "Out12.h"
#include "Out13.h"
#include "Out14.h"
#include "Out15.h"
#include "Inp16.h"
#include "Inp1.h"
#include "TI1.h"
#include "PWMA.h"
#include "SDA.h"
#include "SCL.h"
#include "CSPot.h"
#include "CSFlash.h"
#include "CAN1.h"
#include "AD1.h"
#include "MCBLDC.h"
#include "IFsh1.h"
#include "TMRA0.h"
#include "TMRA1.h"
#include "TMRA2.h"
#include "PWMC1.h"
#include "Enc1.h"
#include "Enc2.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "mlib.h"
#include "GFLib.h"

#include "Timer.h"
#include "Plc.h"
#include "Serial.h"
#include "EEprom.h"
#include "Pid.h"
#include "Alfacol.h"
#include "bldcUser.h"
#include "pwm.h"
#include "Tuning.h"
#include "Current.h"
#include "Bits.h"
#include "CanOpen.h"
#include "Can.h"
#include "NMT.h"

#define faultInpEnable		((inputBuffer & BIT2) == BIT2)

void main(void)
{
	short i;
	
	/* Write your local variable definition here */
	
	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */	
	
	
	TimersInit();
	EEpromInit();
	AlfacolGetFromEeprom();
	PlcInit();
	PidInit();
	CurrInit();
	CanInit();
	NMTInit();
	AnalogicInit();
	CAN1_Enable();
	for(;;)
	{
		CanServerTrasmit();
		
		FlashVariableServer();
		PlcServer();
		AnalogicServer();
		SerialReceve();
		CANOPENServer();
		CurrServer();
		
	}
}

/* END main */
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
