/** ###################################################################
**     Filename  : 402objDict.C
**     Project   : MP_M307_SLAVE
**     Processor : 56F8323
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 31/05/2011, 17.05
**     Contents  :
**         User source code
**
** ###################################################################*/

/* MODULE 402objDict */



#include "cpu.h"
#include "utility.h"
#include "CanDic.h"
#include "CAN.h"
#include "NMT.h"
#include "CanObjects.h"
#include "pid.h"
#include "Dsp402StateMachine.h"
#include "Dsp402homing.h"
#include "Dsp402PositionMode.h"
#include "Dsp402objdict.h"
#include "pid.h"
#include "Current.h"
#include "Encoder.h"
#include "plc.h"
#include "bldcUser.h"
#include "Ramp.h"
#include "Drive.h"

td_subIndex Index6040[] = 
{
	{dicRW, tUNSIGNED16, SIZEOFW_WORD, (byte *)&devices402[0].controlWord}
};

td_subIndex Index6041[] = 
{
	{dicRO, tUNSIGNED16, SIZEOFW_WORD, (byte *)&devices402[0].statusWord}
};


td_subIndex Index6060[] = 
{
	{dicRW, tINTEGER8, SIZEOFW_BYTE, (byte *)&devices402[0].modeOperation}
};

td_subIndex Index6061[] = 
{
	{dicRW, tINTEGER8, SIZEOFW_BYTE, (byte *)&devices402[0].modeOperationDisplay}
};

td_subIndex Index6063[] = 
{
	{dicRO, tINTEGER32, SIZEOFW_DWORD, (byte *)&rpos[0]}
};


td_subIndex Index6064[] = 
{
	{dicRO, tINTEGER32, SIZEOFW_DWORD, (byte *)&vpos[0]}
};

td_subIndex Index6065[] = 
{
	
	{dicRO, tINTEGER16, SIZEOFW_DWORD,	(byte *)&drvAxeRegState[0]},
	
};

td_subIndex Index607A[] = 
{

	{dicRW, tINTEGER32, SIZEOFW_DWORD, (byte *)&pp402[0].targetPosition}
};


td_subIndex Index607B[] = 
{
	{dicRO,	tUNSIGNED8,	SIZEOFW_BYTE,	(byte *)&pp402[0].posRangeLimit.count},
	{dicRW,	tINTEGER32,	SIZEOFW_DWORD,	(byte *)&pp402[0].posRangeLimit.min},
	{dicRW,	tINTEGER32,	SIZEOFW_DWORD,	(byte *)&pp402[0].posRangeLimit.max},
};

td_subIndex Index607D[] = 
{
	{dicRO,	tUNSIGNED8,	SIZEOFW_BYTE,	(byte *)&pp402[0].posLimit.count},
	{dicRW,	tINTEGER32,	SIZEOFW_DWORD,	(byte *)&pp402[0].posLimit.min},
	{dicRW,	tINTEGER32,	SIZEOFW_DWORD,	(byte *)&pp402[0].posLimit.max},
};

td_subIndex Index607C[] = 
{
	{dicRO,	tUNSIGNED8,	SIZEOFW_BYTE,  	(byte *)&hm402[0].off.count},				// 0
	{dicRW, tINTEGER32, SIZEOFW_DWORD, 	(byte *)&hm402[0].off.offset},				// 1
	{dicRW, tINTEGER32, SIZEOFW_DWORD,	(byte *)&hm402[0].off.homSrvErrBatt},		// 2
	{dicRW, tINTEGER32, SIZEOFW_DWORD, 	(byte *)&hm402[0].off.homMaxSrvErrBatt},	// 3
	{dicRW, tINTEGER16, SIZEOFW_WORD, 	(byte *)&hm402[0].off.homMaxErrBatt},		// 4
	{dicRW, tINTEGER32, SIZEOFW_DWORD, 	(byte *)&hm402[0].off.homDistBatt},			// 5
	{dicRW, tINTEGER32, SIZEOFW_DWORD, 	(byte *)&hm402[0].off.homPosAfterHome},		// 6
};


td_subIndex Index607E[] = 
{
	{dicRW,	tUNSIGNED32,	SIZEOFW_DWORD,	(byte *)&pp402[0].maxProfileVelocity}
};



td_subIndex Index6080[] = 
{
	{dicRW,	tUNSIGNED32,	SIZEOFW_DWORD,	(byte *)&pp402[0].maxMotorSpeed}
};

td_subIndex Index6081[] = 
{
	{dicRW,	tUNSIGNED32,	SIZEOFW_DWORD,	(byte *)&pp402[0].profileVelocity}
};

td_subIndex Index6082[] = 
{
	{dicRW,	tUNSIGNED32,	SIZEOFW_DWORD,	(byte *)&pp402[0].endVelocity}
};

td_subIndex Index6083[] = 
{
	{dicRW,	tUNSIGNED32,	SIZEOFW_DWORD,	(byte *)&pp402[0].profileAcceleration}
};

td_subIndex Index6084[] = 
{
	{dicRW,	tUNSIGNED32,	SIZEOFW_DWORD,	(byte *)&pp402[0].profileDeceleration}
};

td_subIndex Index6085[] = 
{
	{dicRW,	tUNSIGNED32,	SIZEOFW_DWORD,	(byte *)&pp402[0].quickStopDeceleration}
};

td_subIndex Index6086[] = 
{
	{dicRW,	tINTEGER16,		SIZEOFW_DWORD,	(byte *)&pp402[0].motionProfileType}
};

td_subIndex Index6087[] = 
{
	{dicRW,	tUNSIGNED32,	SIZEOFW_DWORD,	(byte *)&pp402[0].maxAcceleration}
};



td_subIndex Index6098[] = 
{
	{dicRW, tINTEGER8, SIZEOFW_BYTE, (byte *)&hm402[0].method}	
};

td_subIndex Index6099[] = 
{
	{dicRW, tINTEGER8, SIZEOFW_BYTE, (byte *)&hm402[0].speed.count},
	{dicRW, tINTEGER32, SIZEOFW_DWORD, (byte *)&hm402[0].speed.speedsH},
	{dicRW, tINTEGER32, SIZEOFW_DWORD, (byte *)&hm402[0].speed.speedsL},
};

td_subIndex Index609A[] = 
{
	{dicRW, tINTEGER8, SIZEOFW_BYTE, (byte *)&hm402[0].acceleration}	
}; 



td_subIndex Index60C5[] = 
{
	{dicRW,	tUNSIGNED32,	SIZEOFW_DWORD,	(byte *)&pp402[0].maxDeceleration}
};


td_subIndex Index6863[] = 
{
	{dicRO, tINTEGER32, SIZEOFW_DWORD, (byte *)(rpos + 1)}
};
  
/*td_subIndex Index6865[] = 
{
	{dicRO, tINTEGER16, SIZEOFW_DWORD,	(byte *)&pid_axe_reg_state[1]},
};*/
 
byte pidNumParam = 18;

td_subIndex Index60F9[] = 
{		
	{dicRO, tINTEGER8, 		SIZEOFW_BYTE,	(byte *)&pidNumParam},						// 00
	{dicRW, tINTEGER32, 	SIZEOFW_DWORD,	(byte *)&pidParam[0].a32PGain},				// 01
	{dicRW, tINTEGER32, 	SIZEOFW_DWORD,	(byte *)&pidParam[0].a32IGain},				// 02
	{dicRW, tINTEGER32, 	SIZEOFW_DWORD,	(byte *)&pidParam[0].a32DGain},				// 03
	{dicRW, tINTEGER16, 	SIZEOFW_WORD,	(byte *)&pidParam[0].f16UpperLim},			// 04
	
	{dicRW, tINTEGER16,		SIZEOFW_WORD,	(byte *)&pidParam[0].f16LowerLim},			// 05
	{dicRW, tUNSIGNED32, 	SIZEOFW_DWORD,	(byte *)&srvErrorStatic[0]},				// 06
	{dicRW, tUNSIGNED32, 	SIZEOFW_DWORD,	(byte *)&srvErrorDynamic[0]},				// 07
	{dicRW, tUNSIGNED16, 	SIZEOFW_WORD,	(byte *)&srvErrorTimeOut[0]},				// 08

	{dicRW, tUNSIGNED16, 	SIZEOFW_WORD,	(byte *)&currentNominal},					// 09
	{dicRW, tUNSIGNED16, 	SIZEOFW_WORD,	(byte *)&currentMax},						// 10
	{dicRW, tUNSIGNED16, 	SIZEOFW_WORD,	(byte *)&timeOutCurrent},					// 11
			
	{dicRW, tINTEGER32, 	SIZEOFW_DWORD,	(byte *)&pidCurrentParam[0].a32PGain},		// 12	
	{dicRW, tINTEGER32, 	SIZEOFW_DWORD,	(byte *)&pidCurrentParam[0].a32IGain},		// 13	
	{dicRW, tINTEGER16, 	SIZEOFW_WORD,	(byte *)&pidCurrentParam[0].f16UpperLim},	// 14	
	{dicRW, tINTEGER16, 	SIZEOFW_WORD,	(byte *)&pidCurrentParam[0].f16LowerLim},	// 15
	
	{dicRW, tUNSIGNED16, 	SIZEOFW_WORD,	(byte *)&thresholdCurrMaxInf[0]},			// 16
	{dicRW, tUNSIGNED16, 	SIZEOFW_WORD,	(byte *)&thresholdCurrMaxSup[0]},			// 17
	{dicRW, tUNSIGNED16, 	SIZEOFW_WORD,	(byte *)&pidCurrTimeOut[0]}					// 18
};

byte plcInpNumParam = 4;

td_subIndex Index60FD[] = 
{
	{dicRO, tINTEGER8, 		SIZEOFW_BYTE, 	(byte *)&plcInpNumParam},		// 0
	{dicRO, tUNSIGNED32,	SIZEOFW_DWORD,	(byte *)&inpValue},				// 1
	{dicRO, tUNSIGNED32,	SIZEOFW_WORD,	(byte *)&inpFrz},				// 2
	{dicRW,	tUNSIGNED32,	SIZEOFW_WORD,	(byte *)&inpFrzValue},			// 3
	{dicRW,	tUNSIGNED8,		SIZEOFW_BYTE,	(byte *)&bldcSensoresState[0]}	// 4
	
};

byte plcOutNumParam = 3;
			
td_subIndex Index60FE[] = 
{
	{dicRO, tUNSIGNED32, 	SIZEOFW_BYTE, 	(byte *)&outputBuffer},					// 0
	{dicRO, tUNSIGNED32,	SIZEOFW_WORD,	(byte *)&outValue},						// 1
	{dicRW,	tUNSIGNED32,	SIZEOFW_WORD,	(byte *)&outFrz},						// 2
	{dicRW,	tUNSIGNED32,	SIZEOFW_WORD,	(byte *)&outFrzValue}					// 3
	
};


const td_indexTable DSP402MappingTable4[] =
{
	DeclareIndexTableEntry(Index6040, VAR),
	DeclareIndexTableEntry(Index6041, VAR),
	
	
};


const td_indexTable DSP402MappingTable6[] =
{

	DeclareIndexTableEntry(Index6060, VAR),
	DeclareIndexTableEntry(Index6061, VAR),
	{NULL, 0, VAR},								// 6062
	DeclareIndexTableEntry(Index6063, VAR),		// 6063
	DeclareIndexTableEntry(Index6064, VAR),		// 6064
	DeclareIndexTableEntry(Index6065, VAR),		// 6065
};

const td_indexTable DSP402MappingTable7[] = 
{
	DeclareIndexTableEntry(Index607A, VAR),
	DeclareIndexTableEntry(Index607B, RECORD),
	DeclareIndexTableEntry(Index607C, RECORD),						
	DeclareIndexTableEntry(Index607D, RECORD),
	DeclareIndexTableEntry(Index607E, VAR),
	{NULL, 0, VAR},								// 607F
	DeclareIndexTableEntry(Index6080, VAR),
	DeclareIndexTableEntry(Index6081, VAR),
	DeclareIndexTableEntry(Index6082, VAR),
	DeclareIndexTableEntry(Index6083, VAR),
	DeclareIndexTableEntry(Index6084, VAR),
	DeclareIndexTableEntry(Index6085, VAR),
	DeclareIndexTableEntry(Index6086, VAR),
	DeclareIndexTableEntry(Index6087, VAR),
};

const td_indexTable DSP402MappingTable8[] = 
{
	
	DeclareIndexTableEntry(Index6098, VAR),
	DeclareIndexTableEntry(Index6099, RECORD),
	DeclareIndexTableEntry(Index609A, VAR)
};

const td_indexTable DSP402MappingTableC[] =
{
	DeclareIndexTableEntry(Index60C5, VAR),
};



const td_indexTable DSP402MappingTableF[] =
{
	DeclareIndexTableEntry(Index60F9, VAR),
	{NULL, 0, VAR},							// 60FA
	{NULL, 0, VAR},							// 60FB
	{NULL, 0, VAR},							// 60FC
	DeclareIndexTableEntry(Index60FD, VAR),	// 60FD
	DeclareIndexTableEntry(Index60FE, VAR)	// 60FE
};

const td_indexTable DSP402MappingTable686[] =
{	
	DeclareIndexTableEntry(Index6863, VAR),
	{NULL, 0, VAR},
};



/* END 402objDict */
