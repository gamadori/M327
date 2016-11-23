/*
 * CanObjects.c
 *
 *  Created on: Jan 3, 2016
 *      Author: gianluca
 */
#include "cpu.h"
#include "CanDic.h"
#include "CanObjects.h"
#include "NMT.h"
#include "Plc.h"
#include "Current.h"
#include "EEprom.h"
#include "Version.h"

/*
 * index 0x1000: device type. 
 */
dword deviceType = 0x00020192;

td_subIndex Index1000[] = 
{
	{dicRO, tUNSIGNED32, SIZEOFW_DWORD, (byte *)&deviceType}
};


/*
 * index 1001: error register.
 */
byte errorRegister;
 
const td_subIndex Index1001[] = 
{
	{dicRO, tUNSIGNED8, SIZEOFW_BYTE, (byte *)&errorRegister }
};
 
 /*
  * index 1002: Manufactory status register
  */
dword statusRegister;
 
const td_subIndex Index1002[] = 
{
	{dicRO, tUNSIGNED32, SIZEOFW_DWORD, (byte *)&statusRegister}
};
 

/*
 *  index 1003: Pre-defined error field
 */
td_error_queue queueError = 
{
	{0}
};

const td_subIndex Index1003[] = 
{
	{dicRO, tUNSIGNED8, SIZEOFW_BYTE, (byte *)&queueError.count},
	
	{dicRO, ARRAY, SIZEOFW_DWORD, (byte *)&queueError.objects}
	
};

/*
 * Index 1007: Window Lenght
 *
const td_subIndex Index1007[] = 
{
	{dicRW, tUNSIGNED32, SIZEOFW_DWORD, (word *)&windowsLenght}
		
};
*/

/*
 * index 1008: Manufacturer device name
 */
const td_subIndex Index1008[] = 
{
	{dicRO, tUNICODE_STRING, 0, (byte *)deviceName}
};

/*
 * index 1009: Manufacturer hardware version
 */
const td_subIndex Index1009[] = 
{
	{dicRO, tUNICODE_STRING, 0, (byte *)hwVersion}
};

/*
 * index 1009: Manufacturer software version
 */
const td_subIndex Index100A[] = 
{
	{dicRO, tUNICODE_STRING, 0, (byte *)swVersion}
};

/*
 * Index 100C: Guard Time
 */
const td_subIndex Index100C[] = 
{
	{dicRW, tUNSIGNED16, SIZEOFW_WORD, (byte *)&nmtGuardTime}
};

/*
 * Index 100D: Life Time Factor
 */
const td_subIndex Index100D[] = 
{
	{dicRW, tUNSIGNED8, SIZEOFW_BYTE, (byte *)&nmtLifeTimeFactor}
};


/*
 * index 1014: COB-ID Emergency
 */
const dword emgcyCobId = 0;

const td_subIndex Index1014[] = 
{
	{dicRW, tUNSIGNED32, SIZEOFW_DWORD, (byte *)&emgcyCobId }
};

/*
 * index 1015: Inibit Time Emergency
 */
const word emgcyInibitTime = 0;

const td_subIndex Index1015[] = 
{
	{dicRW, tUNSIGNED16, SIZEOFW_BYTE, (byte *)&emgcyInibitTime}
};

td_canCommands canCommands = 
{
	3, eFlashNull, 0, 0
};

td_subIndex Index1021[] = 
{
	{dicRO, tUNSIGNED8, SIZEOFW_BYTE, (byte *)&canCommands.count},
	{dicRW, tUNSIGNED16, SIZEOFW_WORD, (byte *)&canCommands.flashCmdVar},
	{dicRW, tUNSIGNED16, SIZEOFW_WORD, (byte *)&canCommands.cmdNominalCurr},
	{dicRW, tUNSIGNED16, SIZEOFW_WORD, (byte *)&canCommands.cmdMaxCurr},
};



const td_indexTable CommunicationProfileArea[] =
{
	DeclareIndexTableEntry(Index1000, VAR), // creates a line like: { Index1000, 1 },
	DeclareIndexTableEntry(Index1001, VAR),
	DeclareIndexTableEntry(Index1002, VAR),		// 1002
	{NULL, 0},		// 1003
	{NULL, 0},		// 1004
	{NULL, 0},		// 1005
	{NULL, 0},		// 1006
	{NULL, 0},		// 1007	//DeclareIndexTableEntry(Index1007, VAR),		// 1007
	DeclareIndexTableEntry(Index1008, VAR),
	DeclareIndexTableEntry(Index1009, VAR),
	DeclareIndexTableEntry(Index100A, VAR),
	{NULL, 0},		// 100B
	DeclareIndexTableEntry(Index100C, VAR),
	DeclareIndexTableEntry(Index100D, VAR),
	{NULL, 0},		// 100E
	{NULL, 0},		// 100F
	{NULL, 0},		// 1010
	{NULL, 0},		// 1011
	{NULL, 0},		// 1012
	{NULL, 0},		// 1013
	DeclareIndexTableEntry(Index1014, VAR),		// 1014
	DeclareIndexTableEntry(Index1015, VAR),		// 1015
	
};

const td_indexTable CommunicationProfileArea1[] =
{
	DeclareIndexTableEntry(Index1021, VAR)
};





/*************************************************************************
 * This is the part of the object directory which contains the
 * Receive PDO parameters...
 *************************************************************************/
/** Usually the ID of a PDO is 0x200 + device_node_id, but the node_id is not
 * known during compilation... so what to do?! the correct values have
 * to be setted up during bootup of the device...
 */
 
/** the following count of defined variables has to be the same as defined
 * in the objdict.h file with the define: RECEIVE_PDO_LAST
 * and the define: TRANSMIT_PDO_LAST
 */

td_pdo_communication_parameter RxPDO1 = { 3,  0x200, 255, 0 };
td_subIndex Index1400[] =
{
	{dicRO, tUNSIGNED8,		SIZEOFW_BYTE, 	(void*)&RxPDO1.count },
	{dicRO, tUNSIGNED32,	SIZEOFW_DWORD, 	(void*)&RxPDO1.cob_id },
	{dicRO, tUNSIGNED32,	SIZEOFW_DWORD, 	(void*)&RxPDO1.type },
	{dicRW, tUNSIGNED16,	SIZEOFW_WORD,	(void *)&RxPDO1.inhibit_time}

};

td_pdo_communication_parameter RxPDO2 = { 3, 0x300, 255, 0 };
td_subIndex Index1401[] =
{
	{dicRO, tUNSIGNED8,		SIZEOFW_BYTE, 	(void*)&RxPDO2.count },
	{dicRO, tUNSIGNED32,	SIZEOFW_DWORD, 	(void*)&RxPDO2.cob_id },
	{dicRO, tUNSIGNED32,	SIZEOFW_DWORD, 	(void*)&RxPDO2.type },
	{dicRW, tUNSIGNED16,	SIZEOFW_WORD,	(void *)&RxPDO2.inhibit_time}
};

td_pdo_communication_parameter RxPDO3 = { 3, 0x400, 255, 2000 };
td_subIndex Index1402[] =
{
	{dicRO, tUNSIGNED8,		SIZEOFW_BYTE, 	(void*)&RxPDO3.count },
	{dicRO, tUNSIGNED32,	SIZEOFW_DWORD, 	(void*)&RxPDO3.cob_id },
	{dicRO, tUNSIGNED32,	SIZEOFW_DWORD, 	(void*)&RxPDO3.type },
	{dicRW, tUNSIGNED16,	SIZEOFW_WORD,	(void *)&RxPDO3.inhibit_time}
};

td_pdo_communication_parameter RxPDO4 = { 3, 0x500, 255, 2000 };
td_subIndex Index1403[] =
{
	{dicRO, tUNSIGNED8,		SIZEOFW_BYTE, 	(void*)&RxPDO4.count },
	{dicRO, tUNSIGNED32,	SIZEOFW_DWORD, 	(void*)&RxPDO4.cob_id },
	{dicRO, tUNSIGNED32,	SIZEOFW_DWORD, 	(void*)&RxPDO4.type },
	{dicRW, tUNSIGNED16,	SIZEOFW_WORD,	(void *)&RxPDO4.inhibit_time}
};

td_pdo_communication_parameter RxPDO5 = { 0, 0x510, 255 };
td_subIndex Index1404[] =
{
	{dicRO, tUNSIGNED8,		SIZEOFW_BYTE, 	(void*)&RxPDO5.count },
	{dicRO, tUNSIGNED32,	SIZEOFW_DWORD, 	(void*)&RxPDO5.cob_id },
	{dicRO, tUNSIGNED32,	SIZEOFW_DWORD, 	(void*)&RxPDO5.type }
};

td_pdo_communication_parameter RxPDO6 = { 0, 0x520, 255 };
td_subIndex Index1405[] =
{
	{dicRO, tUNSIGNED8,		SIZEOFW_BYTE, 	(void*)&RxPDO6.count },
	{dicRO, tUNSIGNED32,	SIZEOFW_DWORD, 	(void*)&RxPDO6.cob_id },
	{dicRO, tUNSIGNED32,	SIZEOFW_DWORD, 	(void*)&RxPDO6.type }
};

td_pdo_communication_parameter RxPDO7 = { 0, 0x530, 255 };
td_subIndex Index1406[] =
{
	{dicRO, tUNSIGNED8,		SIZEOFW_BYTE, 	(void*)&RxPDO7.count },
	{dicRO, tUNSIGNED32,	SIZEOFW_DWORD, 	(void*)&RxPDO7.cob_id },
	{dicRO, tUNSIGNED32,	SIZEOFW_DWORD, 	(void*)&RxPDO7.type }
};





/** Create the Receive PDO Parameter area.
 */
const td_indexTable receivePDOParameter[] =
{
	DeclareIndexTableEntry(Index1400, RECORD),
	DeclareIndexTableEntry(Index1401, RECORD),
	DeclareIndexTableEntry(Index1402, RECORD),
	DeclareIndexTableEntry(Index1403, RECORD),
	DeclareIndexTableEntry(Index1404, RECORD),
	DeclareIndexTableEntry(Index1405, RECORD),
	DeclareIndexTableEntry(Index1406, RECORD)
};

/*************************************************************************
 * This is the part of the object directory which contains the
 * Transmit PDO parameters...
 *************************************************************************/

/** Usually the ID of a transmitting PDO is 0x180 + device_node_id,
 * but the node_id is not known during compilation
 * the correct values have to be setted up during bootup of the device...
 */

td_pdo_communication_parameter TxPDO1 = { 3, 0x180, 255, 1 };

td_subIndex Index1800[] =
{
	{dicRW, tUNSIGNED8, 	SIZEOFW_BYTE, 	(void *)&TxPDO1.count},
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void *)&TxPDO1.cob_id},
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void *)&TxPDO1.type},
	{dicRW, tUNSIGNED16,	SIZEOFW_WORD,	(void *)&TxPDO1.inhibit_time}
};

td_pdo_communication_parameter TxPDO2 = { 3, 0x280, 255, 1};
td_subIndex Index1801[] =
{
	{dicRW, tUNSIGNED8, 	SIZEOFW_BYTE,	(void *)&TxPDO2.count },
	{dicRW, tUNSIGNED32, 	SIZEOFW_DWORD,	(void *)&TxPDO2.cob_id },
	{dicRW, tUNSIGNED32, 	SIZEOFW_DWORD,	(void *)&TxPDO2.type },
	{dicRW, tUNSIGNED16,	SIZEOFW_WORD, 	(void *)&TxPDO2.inhibit_time}
};


td_pdo_communication_parameter TxPDO3 = { 3, 0x380, 255, 1};

td_subIndex Index1802[] =
{
	{dicRW, tUNSIGNED8,		SIZEOFW_BYTE, 	(void *)&TxPDO3.count },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void *)&TxPDO3.cob_id },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void *)&TxPDO3.type },
	{dicRW, tUNSIGNED16,	SIZEOFW_WORD, 	(void *)&TxPDO3.inhibit_time}
};

td_pdo_communication_parameter TxPDO4 = { 3, 0x480, 255, 1 };
td_subIndex Index1803[] =
{
	{dicRO, tUNSIGNED8,		SIZEOFW_BYTE, 	(void *)&TxPDO4.count},
	{dicRO, tUNSIGNED32, 	SIZEOFW_DWORD,	(void *)&TxPDO4.cob_id},
	{dicRO, tUNSIGNED32, 	SIZEOFW_DWORD,	(void *)&TxPDO4.type},
	{dicRW, tUNSIGNED16,	SIZEOFW_WORD,	(void *)&TxPDO4.inhibit_time}
};


/** Create the Transmitting PDO Parameter area. 
*/
const td_indexTable transmitPDOParameter[] =
{
	DeclareIndexTableEntry(Index1800, RECORD),
	DeclareIndexTableEntry(Index1801, RECORD),
	DeclareIndexTableEntry(Index1802, RECORD),
	DeclareIndexTableEntry(Index1803, RECORD) 
	
};

/*************************************************************************
 * This is the part of the object directory which contains the
 * Receive PDO Mapping parameters...
 *************************************************************************/

/** GNU C-compiler accepts empty initialization of arrays like this one:..
 * s_pdo_mapping_parameter RxMap1 = { 0, { } };
 * but Microchip C18 doesn't... therefore create this define...
 */

/** the following count of defined variables has to be the same as defined
 * in the objdict.h file with the define: RECEIVE_PDO_MAPPING_LAST
 * and the define: TRANSMIT_PDO_MAPPING_LAST
 */

#define FILL_DATA_S_PDO_MAPPING 0, 0, 0, 0

td_pdo_mapping RxMap1 = { 3,	0x60400010,	0x60600008,	0x607A0020, 0 };	// Rd Control Word, Rd modeOperation, Rd targetPosition
td_pdo_mapping RxMap2 = { 2,	0x60810020,	0x60830020, 0, 			0 };	// Rd profileVelocity, Rd profileAcceleration
td_pdo_mapping RxMap3 = { 2,	0x60860010,	0x60FE0110, 0,			0 };	// Rd motionProfileType, Rd Outputs
td_pdo_mapping RxMap4 = { 0,	{ FILL_DATA_S_PDO_MAPPING } };

#undef FILL_DATA_S_PDO_MAPPING

short typeBoardRxPDO[] = 
{
	BoardsAxe,
	BoardsAxe,
	BoardsAxe,
	BoardsIO
};

td_subIndex Index1600[] =
{
	{ dicRW, tUNSIGNED8,	SIZEOFW_BYTE, 	(void*)&RxMap1.count },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap1.object[0] },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap1.object[1] },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap1.object[2] },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap1.object[3] }
};

td_subIndex Index1601[] =
{
	{ dicRW, tUNSIGNED8,	SIZEOFW_BYTE, 	(void*)&RxMap2.count },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap2.object[0] },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap2.object[1] },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap2.object[2] },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap2.object[3] }
};

td_subIndex Index1602[] =
{
	{ dicRW, tUNSIGNED8,	SIZEOFW_BYTE, 	(void*)&RxMap3.count },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap3.object[0] },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap3.object[1] },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap3.object[2] },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap3.object[3] }
};

td_subIndex Index1603[] =
{
	{ dicRW, tUNSIGNED8,	SIZEOFW_BYTE, 	(void*)&RxMap4.count },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap4.object[0] },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap4.object[1] },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap4.object[2] },
	{ dicRW, tUNSIGNED32,	SIZEOFW_DWORD,	(void*)&RxMap4.object[3] }
};


const td_indexTable RxPDOMappingTable[] =
{
	DeclareIndexTableEntry(Index1600, RECORD),
	DeclareIndexTableEntry(Index1601, RECORD),
	DeclareIndexTableEntry(Index1602, RECORD),
	DeclareIndexTableEntry(Index1603, RECORD)
};


/*************************************************************************
 * This is the part of the object directory which contains the
 * Transmit PDO Mapping parameters...
 *************************************************************************/

/** All the same thing as described above (see Receiving PDO-Mapping */

#define FILL_DATA_S_PDO_MAPPING 0, 0, 0, 0

td_pdo_mapping TxMap1 = { 3,	0x60410010, 0x60610008, 0x60640020,	0 };	// Tx StatusWord, Tx modeOperationDisplay, Tx Virtual Pos
td_pdo_mapping TxMap2 = { 2,	0x60630020, 0x60650010, 0, 			0 };	// TX RealPos, Tx Error Pos
td_pdo_mapping TxMap3 = { 2,	0x68630020,	0x68650010,	0,			0 };	// TX Encoder
td_pdo_mapping TxMap4 = { 1,	0x60FD0210, 0,			0,			0 };	// TX Inputs

#undef FILL_DATA_S_PDO_MAPPING

short typeBoardTxPDO[] = 
{
	BoardsAxe,
	BoardsAxe,
	BoardsAxe,
	BoardsAxe
};


td_subIndex Index1A00[] =
{
	{dicRW, tUNSIGNED8, 	SIZEOFW_BYTE,  (void*)&TxMap1.count },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap1.object[0] },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap1.object[1] },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap1.object[2] },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap1.object[3] }
};

td_subIndex Index1A01[] =
{
	{dicRW, tUNSIGNED8, 	SIZEOFW_BYTE,  (void*)&TxMap2.count },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap2.object[0] },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap2.object[1] },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap2.object[2] },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap2.object[3] }
};

td_subIndex Index1A02[] =
{
	{dicRW, tUNSIGNED8, 	SIZEOFW_BYTE,  (void*)&TxMap3.count },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap3.object[0] },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap3.object[1] },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap3.object[2] },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap3.object[3] }
};

td_subIndex Index1A03[] =
{
	{dicRW, tUNSIGNED8, 	SIZEOFW_BYTE,  (void*)&TxMap4.count },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap4.object[0] },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap4.object[1] },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap4.object[2] },
	{dicRW, tUNSIGNED32,	SIZEOFW_DWORD, (void*)&TxMap4.object[3] }
};


const td_indexTable TxPDOMappingTable[ ] =
{
	DeclareIndexTableEntry(Index1A00, RECORD),
	DeclareIndexTableEntry(Index1A01, RECORD),
	DeclareIndexTableEntry(Index1A02, RECORD),
	DeclareIndexTableEntry(Index1A03, RECORD)
};

td_subIndex Index4000[] =
{
	{dicRW, tUNSIGNED16,	SIZEOFW_WORD, (void *)busOutputs },
};

td_subIndex Index4001[] =
{
	{dicRW, tUNSIGNED16,	SIZEOFW_WORD, (void*)busInputs },
};

const td_indexTable IO_MappingTable[ ] =
{
	DeclareIndexTableEntry(Index4000, RECORD),
	DeclareIndexTableEntry(Index4001, RECORD),
};
