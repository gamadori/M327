/*
 * PDO.c
 *
 *  Created on: Jan 5, 2016
 *      Author: gianluca
 */

#include "cpu.h"
#include "can.h"
#include "CanDic.h"
#include "CanObjects.h"
#include "CANOPEN.h"

#include "NMT.h"
#include "PDO.h"
#include "utility.h"
#include "plc.h"

#include "Timer.h"

#define FILL_DATA 0, 0, 0, 0, 0

typedef struct td_s_pdo_transmit_transfer_table
{
	CanMessage canMsg; // holds the whole canmessage including RTR, len, data, ...
	byte transmissionType; // represents the transmission type of a PDO
	bool sent; // if true, then it is allready sent
	byte syncCount; // count of syncs since last send (only important iftransmissiontype 1-240)
	byte prevData[8]; // holds the previous Datavalue
	bool dataValid; // unless the first time data has be set to valid data, this is set to FALSE
	dword txTimer;
} td_pdo_transmit_transfer_table;


td_pdo_receive_transfer_table RxPDOTransferTable[MAX_COUNT_RX_PDO] =
{
	{0, TRUE, 0, {0, 0, 0, 0}, { FILL_DATA } }
};
	#undef FILL_DATA
	
void CopyBits(int NbBits,
				char* SrcByteIndex, // source Byte index
				int SrcBitIndex, // source bit index
				bool SrcLittleEndian, // is source data littleendian? if yes: true, else false
				char* DestByteIndex,
				int DestBitIndex,
				bool DestLittleEndian); // is destination data little endian?

/** some data definitions */

bool firstTime = TRUE; // because the first time this function is called, the stored pdo-data

/** for the next two vars: adjust the count of lines according to the value of MAX_COUNT_OF_PDO, to avoid
 * unexpected things... */
#define FILL_IN_DATA 0, 0, 0, 0, 0, 0, 0, 0
#define FILL_IN_MESSAGE_DATA 0, 0, 0, 0
td_pdo_transmit_transfer_table TxPDOTransferTable[MAX_COUNT_TX_PDO] =
{
		{ { FILL_IN_MESSAGE_DATA }, 0 ,0 , 0, { FILL_IN_DATA } ,TRUE }
};
#undef FILL_IN_DATA
#undef FILL_IN_MESSAGE_DATA


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


void PDOInit(byte nodeId)
{
	short i;

	RxPDO1.cob_id &= ~0xF;
	RxPDO1.cob_id |= nodeId; 
	
	RxPDO2.cob_id &= ~0xF;
	RxPDO2.cob_id |= nodeId; 
	
	RxPDO3.cob_id &= ~0xF;
	RxPDO3.cob_id |= nodeId; 
	
	RxPDO4.cob_id &= ~0xF;
	RxPDO4.cob_id |= nodeId; 
	
	TxPDO1.cob_id &= ~0x0F;
	TxPDO1.cob_id |= nodeId;
	
	TxPDO2.cob_id &= ~0x0F;
	TxPDO2.cob_id |= nodeId;
	
	TxPDO3.cob_id &= ~0x0F;
	TxPDO3.cob_id |= nodeId;
	
	TxPDO4.cob_id &= ~0x0F;
	TxPDO4.cob_id |= nodeId;
	
	firstTime = TRUE;
	
	for (i = 0; PDORxChannels[i] != NULL; ++i)
	{
		CanFlush(PDORxChannels[i]);			
	}
	
}

/*
 * Funzione di gestione della ricezione
 * di Messaggi PDO
 */
void PDOTransmitServer()
{	
	processSendPDO();
	
}

 
/*====================================
 *
 * void processSendPDO(void) 
 *
 *====================================*/
void processSendPDO(void)
{
	
	// are invalid and therefore they shouldn't be checked...
	
	byte psp_i, psp_j, psp_k;
	byte copiedBytes;

	dword *pSize;
	dword size;
	dword nByte;
	
	word  *pMappingCount; // number of available mapping parameters
	dword *pMappedAppObject; // e.g. 60000108
	byte *pObject; // the object to send (the pointer to the object)
	dword *pwCobId; // cob-id of the variable
	byte *pbTransmissionType; // e.g. 255
	byte *pbCommParamCount; // count of pdo communication parameter (subindex 0)
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
			if (getODEntry((word)0x1A00 | psp_i, (byte)0, (byte **)&pMappingCount, pSize, &nByte) == SUCCESSFUL)
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
						 // now we've got the value. we have to copy it into the can message...
						 	CopyDataToCAN(pObject, &TxPDOTransferTable[psp_i].canMsg.data[copiedBytes], size);
							
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
								TxPDOTransferTable[psp_i].transmissionType = *pbTransmissionType;
							
							TxPDOTransferTable[psp_i].sent = TRUE; // normally this command will be executed (except the first time)

							if (firstTime == FALSE)
							{
								if (TxPDOTransferTable[psp_i].dataValid == TRUE)
								{
									for (psp_j = (byte)0x00; psp_j < copiedBytes; psp_j++)
									{ 
										// checks wheter the data has changed since last time or not...
										if (TxPDOTransferTable[psp_i].canMsg.data[psp_j] != TxPDOTransferTable[psp_i].prevData[psp_j])
										{ 
											// if a databyte has changed since last time, set the sent flag to FALSE, so it will be sent now...
											TxPDOTransferTable[psp_i].sent = FALSE;
										}
										TxPDOTransferTable[psp_i].prevData[psp_j] = (byte)TxPDOTransferTable[psp_i].canMsg.data[psp_j];
										TxPDOTransferTable[psp_i].dataValid = TRUE;
									}
								}
								else
								{
									TxPDOTransferTable[psp_i].sent = FALSE;
								}
							}
							else
							{
								// copy the data to the prevData-field.
								for (psp_j=(byte)0x00; psp_j < copiedBytes; psp_j++)
								{ 
									// checks wheter the data has changed since last time or not...
									TxPDOTransferTable[psp_i].prevData[psp_j] = (byte)TxPDOTransferTable[psp_i].canMsg.data[psp_j];
									TxPDOTransferTable[psp_i].dataValid = TRUE;
								}
								TxPDOTransferTable[psp_i].sent = FALSE;
							}
							TxPDOTransferTable[psp_i].canMsg.len = copiedBytes;
							// workaround: SUCCESSFUL was deleted and (dword)0x8 written instead...
							
							if (getODEntry((word)0x1800 | (byte)psp_i, (byte)0x01, (byte **)&pwCobId, pSize, &nByte) == SUCCESSFUL) // gets the cob_id
								TxPDOTransferTable[psp_i].canMsg.cob_id = *pwCobId & 0x1FFFFFFF;
							
							TxPDOTransferTable[psp_i].canMsg.rtr = 0x00;
							// sets the can-rtr bit to 0
						}
						else // if no datas are copied into the transfertable, set sent flag to true, so the old data aren't sent
						{
							TxPDOTransferTable[psp_i].sent = TRUE;
						}
					}
				} // end test if PDO communication parameter are valid (count at least 2)
			}
			
			if (getODEntry((word)0x1800 | (byte)psp_i, (byte)3, (byte **)&inhibitTime, pSize, &nByte) != SUCCESSFUL)
			{
				inhibit = 0;
			}
			else
			{					
				inhibit = MSEC(*inhibitTime);
			}
			if ((TxPDOTransferTable[psp_i].sent == FALSE) && (TxPDOTransferTable[psp_i].transmissionType == (byte)255) && 
				((dword)(timerSys - TxPDOTransferTable[psp_i].txTimer) >= inhibit))
			{ 
			
				
				CanTransmitPush(PDOTxChannels[psp_i], &TxPDOTransferTable[psp_i].canMsg);
				
				TxPDOTransferTable[psp_i].txTimer = timerSys;
			
			}
		}
		firstTime = FALSE;
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

	word *pMappingCount = NULL; // count of mapped objects...
	word *pMappedAppObject = NULL; // pointer to the var which is mapped to a pdo...
	dword *pMappingParameter = NULL; // pointer fo the var which holds the mapping parameter of an mapping entry
	
	word *pTransmissionType = NULL; // pointer to the transmissiontype...
	dword *pwCobId = NULL;

	byte writtenBits = (byte)0x00;
	byte writtenMappingEntries = (byte)0x00;

	dword *pSize;
	dword size;
	dword nByte;
	

	
	pSize = &size;

	// first make sure we are in OPERATIONAL state. only in OPERATIONAL state PDO transfer is allowed
	if (stateMachine == (byte)OPERATIONAL)
	{
		for (prp_i = (byte)0x00; prp_i < MAX_COUNT_RX_PDO; prp_i++)
		{ // check wheter the received COBs are interesting for us01:
			if (getODEntry((word)0x1400 + prp_i, (byte)1, (byte **)&pwCobId, pSize, &nByte) == SUCCESSFUL)
			{

				if (*pwCobId == m->cob_id)
				{ // the received PDO matches one of the PDOs defined in the object directory
				
					
					Move(m->data, RxPDOTransferTable[prp_i].buffValue, m->len);
									
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
									// now copy: the value of the mapped var. the count of bits. the pointre of the destination var.
									RxPDOTransferTable[prp_i].mappingEntry[prp_j].pObject = pMappedAppObject;
																	
									RxPDOTransferTable[prp_i].mappingEntry[prp_j].bitsIni = writtenBits;
									RxPDOTransferTable[prp_i].mappingEntry[prp_j].size = (*pMappingParameter & 0x000000FF);
									
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
				RxPDOTransferTable[prp_i].processed = FALSE;
				RxPDOTransferTable[prp_i].count = writtenMappingEntries;
				
				// getting the transmissiontype...
				if (getODEntry((word)0x1400 + prp_i, (byte)2, (byte **)&pTransmissionType, pSize, &nByte) == SUCCESSFUL)
				{
					RxPDOTransferTable[prp_i].transmissionType = *pTransmissionType;
				}
				else
				{ // getting of the correct transmissiontype failed: so assign a standard transmissiontype 255
				}
			
				// resetting counting vars...
				writtenBits = (byte)0x00; 		
				writtenMappingEntries = (byte)0x00;
			}
		}
		// process all data in the receiveTransferTable...
		for (prp_i = (byte)0x00; prp_i < MAX_COUNT_RX_PDO; prp_i++)
		{
			if ((RxPDOTransferTable[prp_i].transmissionType == (byte)255) && (RxPDOTransferTable[prp_i].processed == FALSE))
			{
		
				for (prp_j=(byte)0x00; prp_j < RxPDOTransferTable[prp_i].count; prp_j++)
				{
					
					CopyBitsFromCan((byte *)&RxPDOTransferTable[prp_i].buffValue, 
						(byte *)RxPDOTransferTable[prp_i].mappingEntry[prp_j].pObject, 
						RxPDOTransferTable[prp_i].mappingEntry[prp_j].bitsIni, RxPDOTransferTable[prp_i].mappingEntry[prp_j].size);
					
					
				}
				RxPDOTransferTable[prp_i].processed = TRUE;
			}
		}
	}
}
