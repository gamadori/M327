/*
 * PDO.c
 *
 *  Created on: Jan 5, 2016
 *      Author: gianluca
 */

#include "cpu.h"
#include "Timer.h"
#include "CanSettings.h"
#include "Utility.h"
#include "Can.h"
#include "CanOpen.h"
#include "CanDic.h"
#include "CanObjects.h"
#include "Pdo.h"

#define FILL_DATA 0, 0, 0, 0, 0

typedef struct td_s_variable_mapping
{
	word* pObject;	// pointer to the object of the corresponding variable for this mapping entry
	
	word bitsIni;	// value for the variable defined with pObject
	
	byte size;		// size of BITs!!! to write
} td_variable_mapping;

typedef struct td_s_pdo_receive_transfer_table
{	
	byte transmissionType;	// Represents the transmission type of a PDO	
	bool processed;			// if this entry has been proccessed or not	
	byte count;				// count of used PDO mappinf parameters		
	byte buffValue[8];	
	td_variable_mapping mappingEntry[MAPPING_PARAMETER_COUNT];
	
} td_pdo_receive_transfer_table;

typedef struct td_s_pdo_transmit_transfer_table
{
	CanMessage canMessage; 	// Canopen's message
	byte transmissionType; 	// Represents the transmission type of a PDO
	bool sent; 				// if true, then it is already sent
	byte syncCount; 		// count of syncs since last send (only important if transmission type 1-240)
	byte prevData[8]; 		// holds the previous Message Data value
	bool dataValid; 		// unless the first time data has be set to valid data, this is set to FALSE
	dword txTimer;
} td_pdo_transmit_transfer_table;


td_pdo_receive_transfer_table RxPDOTransferTable[NUM_CAN_BOARDS][MAX_COUNT_RX_PDO] =
{
	{0, TRUE, 0, {0, 0, 0, 0}, { FILL_DATA } }
};

td_pdo_transmit_transfer_table TxPDOTransferTable[NUM_CAN_BOARDS][MAX_COUNT_TX_PDO];


#undef FILLDATA

word PDORxChannels[] = 
{
	CanPDO1RxChannel,		//5	
	CanPDO2RxChannel,		//7	
	CanPDO3RxChannel,		//9	
	CanPDO4RxChannel,
	NULL
};

word PDOTxChannels[] = 
{
	CanPDO1TxChannel,		//5	
	CanPDO2TxChannel,		//7	
	CanPDO3TxChannel,		//9	
	CanPDO4TxChannel,
	NULL
};

bool firstTime = TRUE;

void processSendPDO(word idBoard, short typeBoard, short nGroup);

void processReceivedPDO(CanMessage* m);


/*
 * Funzione di gestione della ricezione
 * di Messaggi PDO
 */

void PDOTransmitServer()
{	
	short i;
	
	
	for (i = 0; i < NUM_CAN_BOARDS; ++i)
	{

		processSendPDO(canIDBoards[i], BoardsAxe, i);
		
	}
	
	firstTime = FALSE;	
}

void processSendPDO(word idBoard, short typeBoard, short nGroup)
{	
	byte psp_i, psp_j, psp_k;
	byte copiedBytes;

	dword *pSize;
	dword size;
	dword nByte;
	
	byte  *pMappingCount; // number of available mapping parameters
	dword *pMappedAppObject; // e.g. 60000108
	byte *pObject; // the object to send (the pointer to the object)
	word *pwCobId; // cob-id of the variable
	byte *pbTransmissionType; // e.g. 255
	word *pbCommParamCount; // count of pdo communication parameter (subindex 0)
	word *inhibitTime;
  	word inhibit;
	pSize = &size;

	// first make sure we are in OPERATIONAL state. only in OPERATIONAL state PDO transfer is allowed
	if (stateMachine == (byte)OPERATIONAL)
	{
		// processes all available mapping parameters...
		for (psp_i = (byte)0x00; psp_i < (byte)MAX_COUNT_TX_PDO; psp_i++)
		{
			// Recupero tutte le variabili mappate per ciascun TxPDO
			if (typeBoardTxPDO[psp_i] == typeBoard && getODEntry((word)0x1A00 | psp_i, (byte)0, (byte **)&pMappingCount, pSize, &nByte) == SUCCESSFUL)
			{ 
				
				// ok. we've got the count of mapping paramters for the actual TransmitPDO
				// now we are getting each mapping parameter...
				
				copiedBytes = (byte)0;
				
				for (psp_k = (byte)0x00; psp_k < *pMappingCount; psp_k++)
				{ 
					// gets the mapping parameter e.g. 0x60000108
					if (getODEntry((word)0x1A00 | (byte)psp_i, (byte)((byte)psp_k + (byte)0x01), (byte **)&pMappedAppObject, pSize, &nByte) == (byte)SUCCESSFUL)
					{
						// gets the value of the process var...
						if (getODEntry((word)((*pMappedAppObject & 0xFFFF0000) >> 16),
							(byte)((*pMappedAppObject & 0x0000FF00) >> 8), (byte **)&pObject, pSize, &nByte) == SUCCESSFUL)
						{
							pObject = pObject + ((nGroup) * *pSize); 
						 
						 // now we've got the value. we have to copy it into the can message...
						 	CopyDataToCAN(pObject, &TxPDOTransferTable[idBoard - 1][psp_i].canMessage.data[copiedBytes], size);
							
							copiedBytes += (byte)nByte; // increse count of copied bytes
						}
					}
				} // now the data should be in the can-message struct... (m.data)
				
				if (copiedBytes > (byte)0x00)
				{ 
					// if there were no mapping params, we don't have to send anything
					// test wheter the entries in the Object dictionary are valid...
					if (getODEntry((word)0x1800 | (byte)psp_i, (byte)0, (byte **)&pbCommParamCount, pSize, &nByte) == SUCCESSFUL) // gets the tranmission type
					{
						if (*pbCommParamCount >= (byte)0x02)
						{
							if (getODEntry((word)0x1800 | (byte)psp_i, (byte)2, (byte **)&pbTransmissionType, pSize, &nByte) == SUCCESSFUL) // gets the tranmission type
								TxPDOTransferTable[idBoard - 1][psp_i].transmissionType = *pbTransmissionType;
							
							TxPDOTransferTable[idBoard - 1][psp_i].sent = TRUE; // normally this command will be executed (except the first time)

							if (firstTime == FALSE)
							{
								if (TxPDOTransferTable[idBoard - 1][psp_i].dataValid == TRUE)
								{
									for (psp_j = (byte)0x00; psp_j<copiedBytes; psp_j++)
									{ 
										// checks wheter the data has changed since last time or not...
										if (TxPDOTransferTable[idBoard - 1][psp_i].canMessage.data[psp_j] != TxPDOTransferTable[idBoard - 1][psp_i].prevData[psp_j])
										{ 
											// if a databyte has changed since last time, set the sent flag to FALSE, so it will be sent now...
											TxPDOTransferTable[idBoard - 1][psp_i].sent = FALSE;
											TxPDOTransferTable[idBoard - 1][psp_i].prevData[psp_j] = TxPDOTransferTable[idBoard - 1][psp_i].canMessage.data[psp_j];
										}
										
										TxPDOTransferTable[idBoard - 1][psp_i].dataValid = TRUE;
									}
								}
								else
								{
									TxPDOTransferTable[idBoard - 1][psp_i].sent = FALSE;
								}
							}
							else
							{
								// the first time, we have to send the PDO
								
								// copy the data to the prevData-field.
								for (psp_j=(byte)0x00; psp_j < copiedBytes; psp_j++)
								{ 
									// checks wheter the data has changed since last time or not...
									TxPDOTransferTable[idBoard - 1][psp_i].prevData[psp_j] = (byte)TxPDOTransferTable[idBoard - 1][psp_i].canMessage.data[psp_j];
									TxPDOTransferTable[idBoard - 1][psp_i].dataValid = TRUE;
								}
								TxPDOTransferTable[idBoard - 1][psp_i].sent = FALSE;
							}
							TxPDOTransferTable[idBoard - 1][psp_i].canMessage.len = copiedBytes;
							// workaround: SUCCESSFUL was deleted and (dword)0x8 written instead...
							
							if (getODEntry((word)0x1800 | (byte)psp_i, (byte)0x01, (byte **)&pwCobId, pSize, &nByte) == SUCCESSFUL) // gets the cob_id
								TxPDOTransferTable[idBoard - 1][psp_i].canMessage.cob_id = (*pwCobId & 0x1FFFFFFF) | idBoard;
							
							TxPDOTransferTable[idBoard - 1][psp_i].canMessage.rtr = 0x00;
							// sets the can-rtr bit to 0
						}
						
					}
				} // end test if PDO communication parameter are valid (count at least 2)
			
			
				if (getODEntry((word)0x1800 | (byte)psp_i, (byte)3, (byte **)&inhibitTime, pSize, &nByte) != SUCCESSFUL)
				{
					inhibit = 0;
				}
				else
				{					
					inhibit = MSEC(*inhibitTime);
				}
				if ((TxPDOTransferTable[idBoard - 1][psp_i].sent == FALSE) && (TxPDOTransferTable[idBoard - 1][psp_i].transmissionType == (byte)255) && 
					(dword)(timerSys - TxPDOTransferTable[idBoard - 1][psp_i].txTimer) >= inhibit)
				{ 
					CanTransmitPush(PDORxChannels[psp_i], &TxPDOTransferTable[idBoard - 1][psp_i].canMessage);
		
					TxPDOTransferTable[idBoard - 1][psp_i].txTimer = timerSys;
					TxPDOTransferTable[idBoard - 1][psp_i].sent = TRUE;
					
				}
					
			}
		}
	}
}

/*
 * Funzione di gestione della ricezione
 * di Messaggi PDO
 */
void PDOReceiveServer()
{
	short i;
	CanMessage m;
	
	for (i = 0; PDORxChannels[i] != NULL; ++i)
	{
		if (CanReceivedPop(PDORxChannels[i], &m))
		{
			processReceivedPDO(&m);
		}
	}
	
}

void processReceivedPDO(CanMessage* m)
{
	byte prp_i; // number of the acutal processed pdo-nr.
	byte prp_j;
	short nBoard = 0;
	word *pMappingCount = NULL; // count of mapped objects...
	word *pMappedAppObject = NULL; // pointer to the var which is mapped to a pdo...
	dword *pMappingParameter = NULL; // pointer fo the var which holds the mapping parameter of an mapping entry
	
	word *pTransmissionType = NULL; // pointer to the transmissiontype...
	word *pwCobId = NULL;

	byte writtenBits = (byte)0x00;
	byte writtenMappingEntries = (byte)0x00;

	dword *pSize;
	dword size;
	dword nByte;
	
	word idBoard;
	short typeBoard;
	pSize = &size;

	// first make sure we are in OPERATIONAL state. only in OPERATIONAL state PDO transfer is allowed
	if (stateMachine == (byte)OPERATIONAL)
	{
		
		for (prp_i = (byte)0x00; prp_i < MAX_COUNT_RX_PDO; prp_i++)
		{ // check wheter the received COBs are interesting for us01:
			if (getODEntry((word)0x1400 + prp_i, (byte)1, (byte **)&pwCobId, pSize, &nByte) == SUCCESSFUL)
			{
			
				if (*pwCobId == (m->cob_id & 0xFF0))
				{
					idBoard = (m->cob_id) & 0x0F;
					
					typeBoard = typeBoardRxPDO[prp_i]; 
					
					switch (typeBoard)
					{
						case BoardsAxe:
							nBoard = idBoard - 1;
							
							break;
						default:
							nBoard = idBoard - 1;
						
					}
					
					if (nBoard < 0)
						nBoard = 0;
					
					if (idBoard > 0)
					{ // the received PDO matches one of the PDOs defined in the object directory
					
						Move(m->data, RxPDOTransferTable[idBoard - 1][prp_i].buffValue, m->len);
										
						if (getODEntry((word)0x1600 + prp_i, (byte)0, (byte **)&pMappingCount, pSize, &nByte) == (dword)SUCCESSFUL)
						{ // Lettura del numero di parametri mappate
							
							for (prp_j = (byte)0x00; prp_j < *pMappingCount; prp_j++)
							{	
							// for each mapping parameter: get the parameter and of course: write the vars...
							
								// Per ciasun parametro mappato: recupero l'indice e il subindice
								if (getODEntry((word)0x1600 + prp_i, prp_j + (byte)1, (byte **)&pMappingParameter, pSize, &nByte) == SUCCESSFUL)
								{
									// Recupero il parametro
									if (getODEntry((*pMappingParameter) >> (byte)16, ((*pMappingParameter) >> (byte)8) & (dword)0x000000FF, (byte **)&pMappedAppObject, pSize, &nByte) == SUCCESSFUL)
									{
										pMappedAppObject = pMappedAppObject + ((nBoard) * *pSize);
										// now copy: the value of the mapped var. the count of bits. the pointre of the destination var.
										RxPDOTransferTable[idBoard - 1][prp_i].mappingEntry[prp_j].pObject = pMappedAppObject;
										
											
										RxPDOTransferTable[idBoard - 1][prp_i].mappingEntry[prp_j].bitsIni = writtenBits;
										RxPDOTransferTable[idBoard - 1][prp_i].mappingEntry[prp_j].size = (*pMappingParameter & 0x000000FF);
										
										// increase writteBits to the new value.
										writtenBits += *pMappingParameter & 0x000000FF;
										writtenMappingEntries++;
									}
								}
							}
						}
					} // end: we've found a matching pdo.
				}
				if (writtenBits != (byte)0x00)
				{
					RxPDOTransferTable[idBoard - 1][prp_i].processed = FALSE;
					RxPDOTransferTable[idBoard - 1][prp_i].count = writtenMappingEntries;
					
					// getting the transmissiontype...
					if (getODEntry((word)0x1400 + prp_i, (byte)2, (byte **)&pTransmissionType, pSize, &nByte) == SUCCESSFUL)
					{
						RxPDOTransferTable[idBoard - 1][prp_i].transmissionType = *pTransmissionType;
					}
					else
					{ // getting of the correct transmissiontype failed: so assign a standard transmissiontype 255
					}
				
					// resetting counting vars...
					writtenBits = (byte)0x00; 		
					writtenMappingEntries = (byte)0x00;
				}
			}
		}
	// process all data in the receiveTransferTable...
		for (prp_i = (byte)0x00; prp_i < MAX_COUNT_RX_PDO; prp_i++)
		{
			if ((RxPDOTransferTable[idBoard - 1][prp_i].transmissionType == (byte)255) && (RxPDOTransferTable[idBoard - 1][prp_i].processed == FALSE))
			{
		
				for (prp_j=(byte)0x00; prp_j < RxPDOTransferTable[idBoard - 1][prp_i].count; prp_j++)
				{
					
					CopyBitsFromCan((byte *)&RxPDOTransferTable[idBoard - 1][prp_i].buffValue, 
						(byte *)RxPDOTransferTable[idBoard - 1][prp_i].mappingEntry[prp_j].pObject, 
						RxPDOTransferTable[idBoard - 1][prp_i].mappingEntry[prp_j].bitsIni, RxPDOTransferTable[idBoard - 1][prp_i].mappingEntry[prp_j].size);
					
					
				}
				RxPDOTransferTable[idBoard - 1][prp_i].processed = TRUE;
			}
		}
	}
}

