/*
 * PDO.h
 *
 *  Created on: Jan 5, 2016
 *      Author: gianluca
 */

#ifndef PDO_H_
#define PDO_H_

/** Each pdo can map up to 64 vars (8 vars per 8bit, and 8 * 1Byte = 64 possibilities
* "normally" i think you need a maximum of 8 time 1 Byte, so 8 mapping entries
* should be sufficient. note: for each PDO a PDO-mapping is mandatory!
*/
#define MAPPING_PARAMETER_COUNT 8

/** this struct is only for the receiving transfer-table. it is needed because of the different mapping
 * parameter for one pdo. this means: one pdo (up to 64bit) can mapp the value on different variables
 * e.g.: the value of bit 1 is for bit 1 of the variable UINT8 led1; and the value of bit 14 is for the
 * variable UINT8 led12; */
typedef struct td_s_variable_mapping
{
/** pointer to the object of the corresponding variable for this mapping entry
 */
	word* pObject;
	/** value for the variable defined with pObject
	 */
	word bitsIni;
	/** size of BITs!!! to write
	*/
	byte size;
} td_variable_mapping;

/** Struct to store received PDOs.
 */
typedef struct td_s_pdo_receive_transfer_table
{
	/** transmission type of the PDO: only transmissiontype 255 (0xFF) is supported yet.
	 */
	byte transmissionType;
	/** if this entry has been processed or not. TRUE = already processed.
	 */
	bool processed;
	/** count of used PDO mapping parameters
	 */
	byte count;
	/** mapping parameters for this PDO
	 */
	 
	byte buffValue[8];
	
	td_variable_mapping mappingEntry[MAPPING_PARAMETER_COUNT];
	
} td_pdo_receive_transfer_table;

extern bool firstTime;

void PDOInit(byte nodeId);

void processSendPDO( void );

void processReceivedPDO(CanMessage *canopenMessage );

void sendPDO( const word wID, const byte* pbData, const byte bLength, const bool bRemoteReceive );

bool resetTxPDOData( byte bPDONr );

void PDOReceiveServer();


void PDOTransmitServer();


#endif /* PDO_H_ */
