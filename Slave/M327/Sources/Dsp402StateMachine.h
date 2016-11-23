/** ###################################################################
**     Filename  : 402StateMachine.H
**     Project   : BattutaMobile
**     Processor : 56F8323
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 31/05/2011, 16.14
**     Contents  :
**         User source code
**
** ###################################################################*/

#ifndef __402StateMachine_H
#define __402StateMachine_H

/* MODULE 402StateMachine */

/* MODULE 402StateMachine */



#define DeviceStart							0x0000
#define MskDeviceStart						0x0000


#define DeviceNotReady						0x0000
#define MskDeviceNotReady					0x004F
#define SetDeviceNotReady(status)			status = ((status & ~MskDeviceNotReady) | DeviceNotReady)

#define DeviceSwitchOnDisable				0x0040
#define MskDeviceSwitchOnDisable			0x004F
#define SetDeviceSwitchOnDisable(status)	status = ((status & ~MskDeviceSwitchOnDisable) | DeviceSwitchOnDisable)

#define DeviceReadytoSwitchOn				0x0021
#define MskDeviceReadytoSwitchOn			0x006F
#define SetDeviceReadytoSwitchOn(status)	status = ((status & ~MskDeviceReadytoSwitchOn) | DeviceReadytoSwitchOn)

#define DeviceSwitchOn						0x0023
#define MskDeviceSwitchOn					0x006F
#define SetDeviceSwitchOn(status)			status = ((status & ~MskDeviceSwitchOn) | DeviceSwitchOn)

#define DeviceOperationEnable				0x0027
#define MskDeviceOperationEnable			0x006F
#define SetDeviceOperationEnable(status)	status = ((status & ~MskDeviceOperationEnable) | DeviceOperationEnable)

#define DeviceQuickStopActive				0x0007
#define MskDeviceQuickStopActive			0x006F
#define SetDeviceQuickStopActive(status)	status = ((status & ~MskDeviceQuickStopActive) | DeviceQuickStopActive)

#define DeviceFaultReactionActive			0x000F
#define MskDeviceFaultReactionActive		0x004F
#define SetDeviceFaultReactionActive(status)		status = ((status & ~MskDeviceFaultReactionActive) | DeviceFaultReactionActive)

#define DeviceFault							0x0008
#define MskDeviceFault						0x004F
#define SetDeviceFault						status = ((status & ~MskDeviceFault) | DeviceFault)






enum Dsp402State
{
	dsp402DeviceStart,
	dsp402DeviceNoReady,
	dsp402SwitchOnDisable,
	dsp403ReadyToSwitchOn,
	dsp402SwitchedOn,
	dsp402OperationEnable,
	dsp402QuickStopActive,
	dsp402FaultReactionActive,
	dsp402Fault
};

enum FaultStates
{
	FaultReactionActive,
	Fault
};

enum ModesOperation
{
	moJogModeNeg = -2,
	moJogModePos = -1,
	moReserved1 = 0,
	moProfilePositionMode = 1,
	moVelocityMode = 2,
	moProfileVelocityMode = 3,
	moTorqueProfileMode = 4,
	moReserved2 = 5,
	moHomingMode = 6,
	moInterpolatedPositionMode = 7,
	moResetEncoder2 = 8,	
	moHomingSearch = 9
};

#define NUM_MOTORS	1

typedef struct
{
	dword negLimitSwitch:	1;
	dword posLimitSwitch:	1;
	dword homeSwitch:		1;
	dword interlock:		1;
	dword reserved:			12;
	dword inp:				16;				
	
}td_inpDigital;





typedef struct 
{
	word switchOn		 		:1;	// Bit 0
	word enableVoltage	 		:1; // Bit 1
	word quickStop		 		:1; // Bit 2
	word enableOperation 		:1; // Bit 3
	word newSetPoint			:1; // Bit 4
	word ChangeSetImmediatly	:1;	// Bit 5
	word Abs					:1;	// Bit 6
	word faultReset				:1; // Bit 7
	word halt					:1; // Bit 8
	word reserved				:2; // Bit9, Bit10
	word manufacturerSpecific	:5; // Bit 11 ... Bit 15
	
}td_ppControlWordBits;

typedef struct 
{
	word switchOn		 		:1;	// Bit 0
	word enableVoltage	 		:1; // Bit 1
	word quickStop		 		:1; // Bit 2
	word enableOperation 		:1; // Bit 3
	word homingOperationStart	:1; // Bit 4
	word Reserved1				:1;	// Bit 5
	word Reserved2				:1;	// Bit 6
	word faultReset				:1; // Bit 7
	word halt					:1; // Bit 8
	word reserved				:2; // Bit9, Bit10
	word manufacturerSpecific	:5; // Bit 11 ... Bit 15
	
}td_hmControlWordBits;


typedef union
{
	td_ppControlWordBits pp;
	td_hmControlWordBits hm;
	word w;
	
} td_ControlWord;


typedef struct 
{
	word readyToSwitchOn		:1;	// Bit0
	word switchOn				:1;	// Bit1
	word operationEnabled		:1;	// Bit2
	word fault					:1;	// Bit3
	word voltageEnabled			:1;	// Bit4
	word quickStop				:1;	// Bit5
	word switchOnDisabled		:1;	// Bit6
	word warning				:1;	// Bit7
	word manufacturerSpecific1	:1;	// Bit8
	word remote					:1;	// Bit9
	word targetReached			:1;	// Bit10
	word internalLimiteActive	:1;	// Bit11
	word SetPointAck			:1;	// Bit12 
	word FollowingError			:1;	// Bit13
	word manufacturerSpecific2	:2; // Bit14, Bit15
	
}td_ppStatusWordBits;

typedef struct 
{
	word readyToSwitchOn		:1;	// Bit0
	word switchOn				:1;	// Bit1
	word operationEnabled		:1;	// Bit2
	word fault					:1;	// Bit3
	word voltageEnabled			:1;	// Bit4
	word quickStop				:1;	// Bit5
	word switchOnDisabled		:1;	// Bit6
	word warning				:1;	// Bit7
	word manufacturerSpecific1	:1;	// Bit8
	word remote					:1;	// Bit9
	word targetReached			:1;	// Bit10
	word internalLimiteActive	:1;	// Bit11
	word homingAtteined			:1;	// Bit12 
	word homingError			:1;	// Bit13
	word manufacturerSpecific2	:2; // Bit14, Bit15
	
}td_hmStatusWordBits;



typedef union
{
	td_ppStatusWordBits pp;
	td_hmStatusWordBits hm;
	word w;
	
}td_StatusWord;

#define INTER_NMAX_POINT	10

typedef struct 
{
	byte count;
	long speedsH;
	long speedsL;
}td_hm_speed;



typedef struct 
{
	word count;
	long min;
	long max;
}td_limit;



typedef struct
{
	long targetPosition;	// Posizione desiderata
	
	td_limit posRangeLimit;	// Posizione minima e massima
	
	td_limit posLimit;		// Limite Sw minimo e massimo	
		
	long maxProfileVelocity; // maximum allowed speed in either direction during a profiled move.

	dword maxMotorSpeed;	// massima velocita' consentita per il motore in rpm

	long profileVelocity;	// velocity normally attained at the end of the acceleration ramp during a profiled move . 
						// The profile velocity is given in user defined speed units. 
						// It is converted to position increments per second using the velocity encoder factor
						
	dword endVelocity;	//  Velocity which the drive must have on reaching the target position. Normally, the drive stops at the target position, i.e. the end velocity = 0.

	dword profileAcceleration; // The profile acceleration is given in user defined acceleration units. It is converted to position
							// increments per second2 using the normalizing factors
	
	dword profileDeceleration; // The profile deceleration is given in user defined acceleration units. It is converted to position
							// increments per second2 using the normalizing factors
	
	word profileJerk;
				
	dword quickStopDeceleration;	// The quick stop deceleration is the deceleration used to stop the motor if the ‘Quick Stop’ command

	short motionProfileType;		// Select the type of motion profile used to perform a profiled move.

	dword maxAcceleration;			// To prevent the motor and the application from being destroyed can be used to limit the acceleration to an acceptable value.
	
	dword maxDeceleration;			// To prevent the motor and the application from being destroyed, the max deceleration can be used to limit the deceleration to an acceptable value.
	

	word stepTarget;
}td_pp;

typedef struct 
{
	byte count;
	long offset;
	long homSrvErrBatt;
	long homMaxSrvErrBatt;
	short homMaxErrBatt;
	
	long homDistBatt;
	long homPosAfterHome;		//Nuova posizione dopo home
	
}td_hm_offset;



typedef struct
{
	byte count;
	byte timeUnits;
	char timeIndex;
} td_ipTimePeriod;

typedef struct
{
	byte count;
	dword maxBufferSize;
	dword actualBufferSize;
	byte bufferOrganization;
	word bufferPosition;
	byte sizeDataRecord;
	byte bufferClear;
}td_ipConf;


typedef struct
{
	short modeSelect;
	byte numRecord;
	long dataRecord[INTER_NMAX_POINT];
	td_ipTimePeriod timePeriod;
	td_ipConf conf;
	
}td_ipDev;

typedef struct
{
	word stepEncoder;
	word stepInputs;
} td_modBus;


typedef struct 
{
	word axeCommand;
	word axeStatus;
	
	word inputs;

	word stepComand;
	word stepHoming;
	
}td_MotorDrive;

typedef struct
{
	word step;
	short quickStopOptional;
	
}td_QuickStop;

typedef struct
{
	word count;
	dword encoderRev;
	dword unitMinute;
}td_speedFactor;

typedef struct 
{
	td_speedFactor speedFactor;
	
}td_factorGroup;

typedef struct 
{
	td_hm_offset off;
	
	char method;
	td_hm_speed speed;
	dword acceleration;
	
	long posBatt;
	word step;	
	short numBatt;
	
}td_hm;

typedef struct
{
	word machineState;
	td_ControlWord controlWord;
	td_StatusWord statusWord;
	char modeOperation;
	char modeOperationDisplay;
	long positionActual;
	long positionReal;
	
	td_inpDigital inpDigital;
	
	
	td_ipDev ip;

	td_pp *pp;
	td_hm *hm;
	td_modBus *modBus;
	td_MotorDrive *drive;
	td_QuickStop *qs;
	td_factorGroup *fg;
	
	word step;
	
	
}td_DSP402;




extern td_DSP402 devices402[];



void DSP402InitMachine();

bool CWShutdown(td_DSP402 *pDev);

bool CWSwitchOn(td_DSP402 *pDev);

bool CWDisableVoltage(td_DSP402 *pDev);

bool CWQuickStop(td_DSP402 *pDev);

bool CWDisableOperation(td_DSP402 *pDev);

bool CWEnableOperation(td_DSP402 *pDev);

bool CWFaultRest(td_DSP402 *pDev);

void DeviceControlServer(td_DSP402 *pDev);

void DSP402Server();

void HaltCommand(td_DSP402 *pDev);

/* END 402StateMachine */

#endif

