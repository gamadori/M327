/*
 * SDO.c
 *
 *  Created on: Jan 2, 2016
 *      Author: gianluca
 */
#include "Cpu.h"
#include "Can.h"
#include "Utility.h"
#include "Timer.h"
#include "SDO.h"
#include "CanDic.h"
#include "CanOpen.h"
#include "Alfacol.h"

enum SDOTransferType
{
	SdoNormalTransfer,
	SdoExpeditedTransfer
};

MessageBuffer sndSDOMess;

MessageBuffer rcvSDOsMess;

SDOsInitUploadReq0Data data1;


word stepSDOs = 0;

word numSDOSupperted = 0;


// Initiate Block Download
td_SDObInitDownloadReq buffInitDownloadReq;	// Buffer per la richiesta al Server

td_SDObInitDownloadConf buffInitDpwnloadConf;	// Buffer di conferma dal Server


word sdoCurrProtocol = SDONull;		// Contiene il protocollo SDO corrente


word sdoBuffSegments[SDO_BUFFER_SIZE];

byte *sdopBuffSeg;

word sdoIdBuff = 0;	// Indice del blocco libero

/* Variabili Comandi */
word stepSDOc = 0;
word sdoIndex;
byte sdoSubIndex;
byte sdoDeviceNodeId;
bool sdoExpedited;		// Indica se in corso un expedited transfer

byte sdoWrBuff[8];

byte sdoDataRec[8];		// Buffer di ricezione

byte sdoBlkSize;	// Dimensione di ciascun blocco impostato dal Server

byte *sdoBuffData;
dword sdoBuffLenght;
dword sdoLenData;
dword sdoNumByte;

byte sdoToggleBit = 0;

word currSDOFrame = 0;

/*************************************
 * Dichiarazione Funzioni uso modulo 
 *===================================*/

void SDOsInitiateUploadRequest();

void SDOAbortTransferProtocol(word index, byte subIndex, dword code);

void SDOServerDownloadProtocol();

void SDOServerDownloadSendConfirm();

void SDOClientDownloadProtocol();

void SDOClientUploadProtocol();

void SDOServerUploadProtocol();

void SDOsUploadResponse();

void SDOsInitiateUploadConfirm();

void SDOUploadRequest();

void SDOUploadConfirm();




dword VerSDODownloadRequest(CanMessage *m);

bool VerSDOBlockDownloadConfirm(CanMessage *m);

bool VerSDODownloadConfirm(CanMessage *m);

bool VerSDOInitDownloadConfirm(CanMessage *m);

bool VerSDOIndexSubIndex(byte *buff);

dword VerSDOUploadRequest(CanMessage *m);

dword VerSDOInitUploadConfirm(CanMessage *m);

dword VerSDOUploadConfirm(CanMessage *m);

bool VerSDOInitDownloadReq(CanMessage *m);


void FormattIndexSubIndex(byte *buff, word index, byte subIndex);

void GetIndexSubIndex(byte *buff, word *index, byte *subIndex);

void SendSDO(byte *buff, short len);

void SendSDOCobId(byte cobId, byte *buff, short len);

void SDOProtocolTerminate();

void CmdSDOBlockDownload(byte nodeId, word ptc, word index, byte subIndex);



short stepSDO = 0;

/*=======================================
 * FINE Dichiarazione Funzioni uso modulo 
 ****************************************/

/*=======================================
 *
 * Iniziazione delle variabili del modulo
 *
 *=======================================*/
void SDOInit()
{
	
	
	sndSDOMess.valid = FALSE;
	rcvSDOsMess.valid = FALSE;	

}


/*
 * Comando di invio dato alla slave
 */
void SDOCommandDownload(int numStation, byte *buffOr, word index, byte subIndex, word size)
{
	
	sdoNumByte = size << 1;
	
	sdoBuffData = sdoWrBuff;
	
	
	CopyDataToCAN(buffOr, sdoBuffData, size);
	
	sdoIndex = index;
	sdoSubIndex = subIndex;
	sdoBuffLenght = size;
	sdoDeviceNodeId = numStation;
	stepSDOc = 0;
	sdoCurrProtocol = SDOsDownloadClient;
}


void SDOCommandUpload(int numStation, word index, byte subIndex)
{
	stepSDOc = 0;
	
	sdoIndex = index;
	sdoSubIndex = subIndex;
	sdoDeviceNodeId = numStation;
	
	if (stepSDOc > 0)
		stepSDOc = 0;
	sdoCurrProtocol = SDOsUploadClient;
}

/*=====================================================
 * 
 * bool SDOAbortTransferProtocol()
 * 
 *	Questo protocollo è usato per abortire un processo
 * del protocollo SDO
 *
 *=====================================================*/
void SDOAbortTransferProtocol(word index, byte subIndex, dword code)
{
	td_SDOAbortSDO buff;
	
	buff.byte0.cs = 4;
	buff.byte0.X = 0;
	FormattIndexSubIndex(&buff.m0, index, subIndex);
	
	BuffCopyValue(code, buff.d, 4);
	
	SendSDO((byte *)&buff, 8);
						
	stepSDOc = 0;
	sdoCurrProtocol = SDONull;

}


/*==========================================
 * void SDOServer()
 * 
 * 	Funzione Principale del modulo SDO
 *	Richiama i vari protocolli SDO
 *
 *=========================================*/
void SDOServer()
{
	CanMessage m;
	
	//sdoCurrProtocol = SDOsUploadClient;
	//sdoIndex = 0x1000;
	//sdoSubIndex = 0;
	
	switch (sdoCurrProtocol)
	{
		case SDONull:		
			// Nessun protocollo attivo
			if (CanReceivedPop(CanSDORxChannel, &m))
			{
				CopyDataFromCAN(m.data, sdoDataRec, 8);
				
				switch ((m.data[0] & 0xE0) >> 5)
				{
					case 1:
						// Ricevuta Request Initiate SDO Download  Segment da un Client
						
						sdoCurrProtocol = SDOsDownloadServer;
						
					
						
						break;
						
					case 2:
						// Ricevuto Request Initiate SDO Upload Segment da un client
						
						sdoCurrProtocol = SDOsUploadServer;
						
						
						break;
						
					case 4:
						// Abort SDO Transfer Protocol
						sdoCurrProtocol = SDONull;
						break;
					default:
						sdoCurrProtocol = SDONull;
						break;
				}
			}
			break;
			
		case SDOsDownloadClient:	
			// SDO Segment Download Client
			SDOClientDownloadProtocol();
			break;
			
		case SDOsDownloadServer:	
			// SDO Segment Download Server
			SDOServerDownloadProtocol();
			break;
			
		case SDOsUploadClient:		
			// SDO Segment Upload Client
			SDOClientUploadProtocol();
			break;
			
		case SDOsUploadServer:		
			// SDO Segment Upload Server
			SDOServerUploadProtocol();
			break;
			
		case SDObDownloadClient:	
			// SDO Block Download
			break;
		
		case SDObDownloadServer:	
			// SDO Block Download
			break;
			
			
		case SDObUploadClient:		
			// SDO Block Upload Client
			break;
			
		case SDObUploadServer:		
			// SDO Block Upload Server
			break;
			
	}
}
/*=========================================================================
 *
 *	*******	D O W N L O A D   S D O  S E G M E N T  P R O T O C O L	*******
 *
 *=========================================================================*/


/*================================================================================
 *
 * void SDOServerDownloadProtocol() LATO SERVER
 *
 *	Protocollo Server Download:
 *	I dati vengono ricevuti dal Server dal Client, in questo caso la scheda svolge
 *	la funzione di Server (riceve i dati)
 *	1) Il server riceve dal client un Initiate SDO Download Require
 *	2) Il server invia al client un Initiate SDO Download Confirm
 *	3) Se si tratta di un expedited transfer il trasferimento è terminato, altrimenti
 *		Il server si mette in attesa dei dati
 *	4)Il server riceve i pacchetto dei dati e conferma la ricezione, se non ci sono
 *		altri pacchetti il processo termina altrimenti il server ritorna allo stato 3
 *
 *===================================================================================*/
void SDOServerDownloadProtocol()
{
	td_SDOsInitDownloadReq *pReqInit;
	td_SDOsInitDownloadConf buffConfInit;
	td_SDOsDownloadReq *pReq;
	short len;
	CanMessage m;
	short i;
	dword resp;
	
	switch (stepSDOc)
	{
		case 0:
			// Ricezione di un Initiate SDO Download Required
			stepSDOc++;
			
			// Traduzione del pacchetto ricevuto
			pReqInit = (td_SDOsInitDownloadReq *)sdoDataRec;
			
			
			// Lettura dell'indice e sub-indice del dato del dizionario in cui si vuole scrivere
			GetIndexSubIndex(&pReqInit->m0, &sdoIndex, &sdoSubIndex);
			
			
			
			if (pReqInit->byte0.e == 1)
			{
				// expedited transfer i dati sono gia presenti nel comando ricevuto
				
				sdoExpedited = TRUE;
				
				if (pReqInit->byte0.s == 1)
				{
					// La dimensione del dato è nota
					sdoBuffLenght = 4 - pReqInit->byte0.n;
					
				}
				else
				{
					// La dimensione del dato non è nota
					sdoBuffLenght = 0;
					
				}
				
				
				setODEntry(sdoIndex, sdoSubIndex, (byte *)pReqInit->d, sdoBuffLenght);
				
				
			}
			else
			{
				sdoExpedited = FALSE;
				
				if (pReqInit->byte0.s == 1)
				{
					// Dimensione del dato la trovo in d
					sdoLenData = sdoBuffLenght = DWGetValueFromBuff(pReqInit->d);
					
					
					
				}
				else
					sdoBuffLenght = 0;	// Dimensione del dato non nota
			}
			break;
			
		case 1:
			// Trasmissione di Initiate SDO Download Response
			buffConfInit.byte0.scs = 3;
			buffConfInit.byte0.X = 0;
			
			FormattIndexSubIndex(&buffConfInit.m0 , sdoIndex, sdoSubIndex);
						
			
			DWSetValueToBuffer((dword)0, (byte *)&buffConfInit.reserved0, 4);
			
			
			
			SendSDO((byte *)&buffConfInit, 8);
			
			
			
			if (sdoExpedited)
			{
				// Terminato
				SDOProtocolTerminate();
				
			}
			else
			{
				sdoToggleBit = 1;
				sdoIdBuff = 0;
				stepSDOc++;
			}
			break;
			
		case 2:
			// Sono in attesa di un segmento del dato
			if (CanReceivedPop(CanSDORxChannel, &m))
			{
				if ((resp = VerSDODownloadRequest(&m)) != SDOErrorOK)
				{
					// Devo inviare il comando Abort SDO Transfer
					SDOAbortTransferProtocol(sdoIndex, sdoSubIndex, resp);
									
				}
				else
				{
					pReq = (td_SDOsDownloadReq *)&m;
					len = 8 - pReq->byte0.n;		// numero di byte contenuti in segdata
					
					sdoToggleBit = pReq->byte0.t;
					
					if (sdoBuffLenght >= len && sdoIdBuff < SDO_BUFFER_SIZE - len)
					{							
						for (i = 0; i < len; ++i)			
							sdoBuffSegments[sdoIdBuff++] = pReq->segData[i];
							
						sdoBuffLenght -= len;
						
						// Invio Confirm Download SDO Segment
						SDOServerDownloadSendConfirm();
						
						// Verifico se l'ultimo pacchetto dei dati o devo rimanere ancora in attesa
						if (pReq->byte0.c == 0)
						{
							// terminato la ricezione posso scrivere il dato sul dizionario
							
							setODEntry(sdoIndex, sdoSubIndex, (byte *)sdoBuffSegments, sdoLenData);
							
							stepSDOc = 0;
							sdoCurrProtocol = SDONull;	
						}
						else
						{
							
							sdoCurrProtocol = SDONull;	
						}
					}
					else
					{
						// Devo inviare il comando Abort SDO Transfer
						SDOAbortTransferProtocol(sdoIndex, sdoSubIndex, resp);
												
					}
				}
				
			}
			break; 
	}
}

/*=======================================================================================
 *
 * void SDOClientDownloadProtocol() LATO CLIENT
 *
 *	Protocollo Client Download:
 *	I dati vengono inviati dal Client al Server, in questo caso la scheda svolge
 *	la funzione di Client (invia i dati)
 *	1) Il Client invia una richiesta di invio dati: Initiate SDO Download Request
 *	2) Il Client si mette in attesa della conferma dal Server (Initiate SDO Download Confirm)
 *	  Se si tratta di un expedited transfer il trasferimento è terminato.
 *	3) Il Client procede all'invio dei dati tramite un Download SDO Segment Request
 *	4)Il Client si mette in attesa del risposta dal Server (Download SDO Segment Confirm)
 *		se i dati sono stati trasmessi il protocollo termina, altrimenti si torna alpunto 3
 *
 *===================================================================================*/
void SDOClientDownloadProtocol()
{
	CanMessage m;
	td_SDOsInitDownloadReq buff;
	td_SDOsDownloadReq buffReq;
	word tmp;
	short i;
	dword nByte;
	
	switch (stepSDOc)
	{
		case 0:
			
			
			sdoToggleBit = 0;
			
			//getODEntry(sdoIndex, sdoSubIndex, (word **)&sdoBuffData, &sdoBuffLenght, &nByte);
			
			//sdoBuffLenght = DicGetSize(sdoIndex, sdoSubIndex);
			
			// Invio del Request di download al server
			buff.byte0.ccs = 1;	// Initiate download request						
			buff.byte0.X = 0;	// Not used always 0
			
			sdoExpedited = FALSE;
			
			// Se la dimensione dei dati da inviare <= posso usare la expedited transfer
			if (sdoBuffLenght > 0 && sdoBuffLenght <= 4)
			{
				// expedited 
				tmp = 8 - sdoBuffLenght;	// Se non uso questa variabile temporanea va in crash il compilatore	
				buff.byte0.n = tmp;
				buff.byte0.e = 1;
				buff.byte0.s = 1;
				
				for (i = 0; i < sdoBuffLenght; ++i)
				{
					((byte *)&buff.d[0])[i] = sdoBuffData[i];
				}
				
				sdoExpedited = TRUE;
			}
			else if (sdoBuffLenght > 4)
			{
				// Indicata la dimensione dei byte per il download
				buff.byte0.n = 0;
				buff.byte0.e = 0;
				buff.byte0.s = 1;
				
				BuffCopyValue(sdoBuffLenght, buff.d, 4);
				
			}
			else 
			{
				// Non è indicata la dimensione in byte
				buff.byte0.n = 0;
				buff.byte0.e = 0;
				buff.byte0.s = 0;
			}
					
			FormattIndexSubIndex(&buff.m0, sdoIndex, sdoSubIndex);			
			
			SendSDOCobId(sdoDeviceNodeId, (byte*)&buff, 8);
			stepSDOc += 2;
			
			break;
			
		case 1:
			
			sdoToggleBit = 0;
			
			getODEntry(sdoIndex, sdoSubIndex, (byte **)&sdoBuffData, &sdoBuffLenght, &nByte);
			
			//sdoBuffLenght = DicGetSize(sdoIndex, sdoSubIndex);
			
			// Invio del Request di download al server
			buff.byte0.ccs = 1;	// Initiate download request						
			buff.byte0.X = 0;	// Not used always 0
			
			sdoExpedited = FALSE;
			
			// Se la dimensione dei dati da inviare <= posso usare la expedited transfer
			if (sdoBuffLenght > 0 && sdoBuffLenght <= 4)
			{
				// expedited 
				tmp = 8 - sdoBuffLenght;	// Se non uso questa variabile temporanea va in crash il compilatore	
				buff.byte0.n = tmp;
				buff.byte0.e = 1;
				buff.byte0.s = 1;
				
				for (i = 0; i < sdoBuffLenght; ++i)
				{
					((byte *)buff.d)[i] = sdoBuffData[i];
				}
				
				sdoExpedited = TRUE;
			}
			else if (sdoBuffLenght > 4)
			{
				// Indicata la dimensione dei byte per il download
				buff.byte0.n = 0;
				buff.byte0.e = 0;
				buff.byte0.s = 1;
				
				BuffCopyValue(sdoBuffLenght, (byte *)buff.d, 4);
				
			}
			else 
			{
				// Non è indicata la dimensione in byte
				buff.byte0.n = 0;
				buff.byte0.e = 0;
				buff.byte0.s = 0;
			}
					
			FormattIndexSubIndex(&buff.m0, sdoIndex, sdoSubIndex);			
			
			SendSDO((byte*)&buff, 8);
			stepSDOc++;
			
			break;
			
		case 2:
		
			// Ricezione del Confirm download dal server
			if (CanReceivedPop(CanSDORxChannel, &m))
			{
				// Verifico se si tratta di confirm download dal server
				if (VerSDOInitDownloadConfirm(&m))
				{
					if (sdoExpedited)
					{	
						// Download Terminato				
						stepSDOc = 0;
						sdoCurrProtocol = SDONull;
						
						if (currSDOFrame == sdoIndex)
						{
							AlfacolSendResponse(0, 0,  FALSE, 0);

							currSDOFrame = 0;
						}
					}
					else
					{
						stepSDOc++;
					}
				}					
				else
				{
					// Devo inviare il comando Abort SDO Transfer
					SDOAbortTransferProtocol(sdoIndex, sdoSubIndex, SDOErrorProtocolTimeOut);																
				}
			}
			break;
			
		case 3:
			// Download SDO Segment
			buffReq.byte0.ccs = 0;
			buffReq.byte0.t = sdoToggleBit;
			
			if (sdoBuffLenght > 7)
			{			
				buffReq.byte0.n = 0;
				sdoBuffLenght -= 7;
			}
			else
			{
				tmp = 8 - buffReq.byte0.n;
				buffReq.byte0.n = tmp;
				sdoBuffLenght = 0;
			}
			
			SendSDO((byte *)&buffReq, 8);
			
			if (++sdoToggleBit > 1)
				sdoToggleBit = 0;
			
			stepSDOc++;
			break;
			
		case 4:
			// Attendo la conferma dal client 
			if (CanReceivedPop(CanSDORxChannel, &m))
			{
				if (VerSDODownloadConfirm(&m))
				{
					if (sdoBuffLenght == 0)
					{
						// Download Terminato
						stepSDOc = 0;
						sdoCurrProtocol = SDONull;
					}
					else
					{
						stepSDOc = 3;
					}
						
				}
				else
				{
					// Devo inviare il comando Abort SDO Transfer
					SDOAbortTransferProtocol(sdoIndex, sdoSubIndex, SDOErrorProtocolTimeOut);
					
				}
			}
			break;
	}
}


/*=======================================================
 * bool VerSDOInitDownloadReq(Message *m)
 *
 * Verifica se il Messaggio ricevuto è del tipo
 * confirm download del server
 * 
 * Inputs:
 *		Message *m: messaggio da analizzare 
 * Return 
 *		TRUE: se il messaggio è del tipo confirm download
 *		FALSE: altrimenti
 *=======================================================*/
bool VerSDOInitDownloadReq(CanMessage *m)
{
	td_SDOsInitDownloadReq* p; 

	if (m->len != 8)
		return FALSE;
	
	
	p = (td_SDOsInitDownloadReq *)m->data;
	
	if (p->byte0.ccs != 1 || p->byte0.X != 0 || !VerSDOIndexSubIndex(&p->m0))
		return FALSE;
	
	return TRUE;
	
}

/*=======================================================
 * bool VerSDOInitDownloadConfirm(Message *m)
 *
 * Verifica se il Messaggio ricevuto è del tipo
 * confirm download del server
 * 
 * Inputs:
 *		Message *m: messaggio da analizzare 
 * Return 
 *		TRUE: se il messaggio è del tipo confirm download
 *		FALSE: altrimenti
 *=======================================================*/
bool VerSDOInitDownloadConfirm(CanMessage *m)
{
	td_SDOsInitDownloadConf* p; 

	if (m->len != 8)
		return FALSE;
	
	
	p = (td_SDOsInitDownloadConf *)m->data;
	
	if (p->byte0.scs != 3 || p->byte0.X != 0 || !VerSDOIndexSubIndex(&p->m0))
		return FALSE;
	
	return TRUE;
	
}

/*=======================================================
 * bool VerSDOInitDownloadRequest(Message *m)
 *
 * Verifica se il Messaggio ricevuto è del tipo
 * confirm download del server
 * 
 * Inputs:
 *		Message *m: messaggio da analizzare 
 * Return 
 *		TRUE: se il messaggio è del tipo confirm download
 *		FALSE: altrimenti
 *=======================================================*/
dword VerSDODownloadRequest(CanMessage *m)
{
	td_SDOsDownloadReq* p; 

	if (m->len != 8)
		return FALSE;
	
	
	p = (td_SDOsDownloadReq *)m->data;
	
	if (p->byte0.ccs != 0)
		return SDOErrorProtocolTimeOut;
	else if (p->byte0.t == sdoToggleBit)
		return SDOErrorToggleBit;
	
	else 
		return SDOErrorOK;
	
}

/*=======================================================
 * bool VerSDOInitDownloadConfirm(Message *m)
 *
 * Verifica se il Messaggio ricevuto è del tipo
 * confirm download del server
 * 
 * Inputs:
 *		Message *m: messaggio da analizzare 
 * Return 
 *		TRUE: se il messaggio è del tipo confirm download
 *		FALSE: altrimenti
 *=======================================================*/
bool VerSDODownloadConfirm(CanMessage *m)
{
	td_SDOsDownloadConf* p; 

	if (m->len != 8)
		return FALSE;
	
	
	p = (td_SDOsDownloadConf *)m->data;
	
	if (p->byte0.scs == sdoToggleBit || p->byte0.X != 0)
		return FALSE;
	
	return TRUE;
}



/*===========================================================
 * void SDOServerDownloadSendConfirm()
 *
 *	Invia la converma della ricezione di Download SDO Segment
 *
 *==========================================================*/
void SDOServerDownloadSendConfirm()
{
	td_SDOsDownloadConf buff;
	
	buff.byte0.scs = 1;
	buff.byte0.t = sdoToggleBit;
	buff.byte0.X = 0;
	
	
	SendSDO((byte *)&buff, 8);
	
}

/*=====================================================================
 *
 *	*******	U P L O A D  S D O  S E G M E N T  P R O T O C O L	*******
 *
 *=====================================================================
 
 /*================================================================================
 *
 * void SDOServerUploadProtocol() LATO SERVER
 *
 *	Protocollo Server Upload:
 *	I dati vengono inviati dal Server al Client, in questo caso la scheda svolge
 *	la funzione di Server (invia i dati)
 *	1) Il server riceve dal client un Initiate SDO Upload Require
 *	2) Il server invia al client un Initiate SDO Upload Confirm
 *	3) Se si tratta di un expedited transfer il trasferimento è terminato, altrimenti
 *		Il server si mette in attesa  di alrìtre request
 *	4)Il server riceve il requst e invia i dati al Client, se non ci sono
 *		altri pacchetti il processo termina altrimenti il server ritorna allo stato 3
 *
 *===================================================================================*/
void SDOServerUploadProtocol()
{
	td_SDOsInitUploadReq *pReqInit;
	td_SDOsInitUploadConf buffConfInit;
	td_SDOsUploadReq *pReq;
	
	CanMessage m;
	short i;
	dword resp;
	word tmp;

	switch (stepSDOc)
	{
		case 0:
			// Ricezione di un Initiate SDO Download Required
			stepSDOc++;
			
			// Traduzione del pacchetto ricevuto
			pReqInit = (td_SDOsInitUploadReq *)sdoDataRec;
			
			// Lettura dell'indice e sub-indice del dato del dizionario in cui si vuole scrivere
			GetIndexSubIndex(&pReqInit->m0, &sdoIndex, &sdoSubIndex);
			
			if (getODEntry(sdoIndex, sdoSubIndex, (byte **)&sdopBuffSeg, &sdoLenData, &sdoNumByte) != SUCCESSFUL)		
			{
				// Devo inviare il comando Abort SDO Transfer
				SDOAbortTransferProtocol(sdoIndex, sdoSubIndex, 0x06090011);
				
			}
												
			break;
			
		case 1:
			// Trasmissione di Initiate SDO Download Confirm
			buffConfInit.byte0.scs = 2;
			buffConfInit.byte0.X = 0;
			
			FormattIndexSubIndex(&buffConfInit.m0 , sdoIndex, sdoSubIndex);
						
						
			if (sdoLenData <= 4)
			{
				sdoExpedited = TRUE;
				
				// Expedited transfer
				buffConfInit.byte0.e = 1;
				buffConfInit.byte0.s = 1;

				
				tmp = 4 - sdoNumByte;
				buffConfInit.byte0.n = tmp;
				
				CopyDataToCAN(sdopBuffSeg, buffConfInit.d, sdoLenData);
				
			}
			else
			{
				sdoExpedited = FALSE;
				buffConfInit.byte0.e = 0;
				buffConfInit.byte0.s = (sdoLenData > 0);
				
				if (sdoLenData > 0)
				{
					// d deve contenere la dimensione del dato
					BuffCopyValue((dword)sdoLenData, buffConfInit.d, 4);
				}
				
			}
			
			SendSDO((byte *)&buffConfInit, 8);			
			
		
			
			if (sdoExpedited)
			{
				// Terminato
				//SDOProtocolTerminate();				
				stepSDOc = 0;
				sdoCurrProtocol = SDONull;
			}
			else
			{
				sdoToggleBit = 1;
				sdoIdBuff = 0;
				stepSDOc++;
			}
			break;
			
		case 2:
			// Sono in attesa di un segmento del dato
			if (CanReceivedPop(CanSDORxChannel, &m))
			{
				if ((resp = VerSDOUploadRequest(&m)) != SDOErrorOK)
				{
					// Devo inviare il comando Abort SDO Transfer
					SDOAbortTransferProtocol(sdoIndex, sdoSubIndex, resp);
					
				}
				else
				{
					SDOsUploadResponse();
				}
				
			}
			break; 
	}
}


/*================================================================================
 *
 * void SDOClientUploadProtocol() LATO CLIENT
 *
 *	Protocollo Client Upload:
 *	Il client richiede i dati al server, in questo caso la scheda svolge
 *	la funzione di Client (riceve i dati)
 *	1) Il Client invia al server un Initiate SDO Upload Request
 *	2) Il Client attende dal Server un Initiate SDO Upload Confirm
 *	3) Se si tratta di un expedited transfer il trasferimento è terminato, altrimenti
 *		il client invia una Richiesta di dati al server e di mette in attesa
 *	4) Il Client riceve i pacchetto, se non ci sono
 *		altri pacchetti il processo termina altrimenti il server ritorna allo stato 3
 *
 *===================================================================================*/
void SDOClientUploadProtocol()
{
	td_SDOsInitUploadReq *buff;
	
	switch (stepSDOc)
	{
		case 0:
			// Il Client invia al server un Initiate SDO Upload Request
			SDOsInitiateUploadRequest();
			sdoToggleBit = 0;
		
			
			break;
			
		case 1:
			// Il client si mette attesa di ricevere un Initiate SDO Upload Confirm
				// Ricezione del Confirm download dal server
			SDOsInitiateUploadConfirm();
			
			
			break;
			
		case 2:
			// Il client far richiesta del successivo segmento dei dati
			SDOUploadRequest();
			
			break;
			
		case 3:
			// Il client rimane in attesa del successivo segmento
			SDOUploadConfirm();
			break;
			
	}
}

/*===================================
 *
 * void SDOsInitiateUploadRequest()
 *
 * 	Il client invia al server 
 *	Initiate SDO Upload Request:
 *		
 *
 *==================================*/
 
void SDOsInitiateUploadRequest()
{
	td_SDOsInitUploadReq buff;
	
	buff.byte0.ccs = 2;
	buff.byte0.X = 0;
	buff.reserved0 = 0;
	buff.reserved1 = 0;
	buff.reserved2 = 0;
	buff.reserved3 = 0;
	
	FormattIndexSubIndex(&buff.m0, sdoIndex, sdoSubIndex);
			
	SendSDOCobId(sdoDeviceNodeId, (byte *)&buff,8);
	
	TMR_CAN_SDO = MSEC(2000);
	stepSDOc++;
			
}

/*=================================
 * void SDOsInitiateUploadConfirm()
 *
 * Il client riceve la conferma dal 
 * Server
 *
 *=================================*/
 
 long numRespSdo = 0;
void SDOsInitiateUploadConfirm()
{
	CanMessage m;
	td_SDOsInitUploadConf *buff;
	dword resp;
	
	
	if (CanReceivedPop(CanSDORxChannel, &m))
	{
		// Verifico se si tratta di confirm download dal server
		if ((resp = VerSDOInitUploadConfirm(&m)) == SDOErrorOK)
		{
			buff = (td_SDOsInitUploadConf *)&(m.data);	
					
			sdoExpedited = FALSE;		
			
			// Dimensione del dato la trovo in d
			sdoLenData = sdoBuffLenght = DWGetValueFromBuff((byte *)buff->d);
			
			if (buff->byte0.e == 1)
			{
				// expedited transfer i dati sono gia presenti nel comando ricevuto
				
				sdoExpedited = TRUE;
				
				if (buff->byte0.s == 1)
				{
					// La dimensione del dato è nota
					sdoBuffLenght = 4 - buff->byte0.n;
					
				}
				else
				{
					// La dimensione del dato non è nota
					sdoBuffLenght = 0;
					
				}
				
				if (currSDOFrame == sdoIndex)
				{					
					CopyDataFromCAN(buff->d, (byte *)&resp, sdoBuffLenght);
					numRespSdo++;
					
					AlfacolSendResponse(0, resp,  FALSE, sdoBuffLenght);
					
					sdoCurrProtocol = SDONull;
					currSDOFrame = 0;
				}				
				else if (setODEntry(sdoIndex, sdoSubIndex, buff->d, sdoBuffLenght) == SUCCESSFUL)					
					SDOProtocolTerminate();
				else
					currSDOFrame = 0;
				
			}
			else
			{
				sdoExpedited = FALSE;
				
				if (buff->byte0.s == 1)
				{
					// Dimensione del dato la trovo in d
					sdoLenData = sdoBuffLenght = DWGetValueFromBuff(buff->d);
					
					
					
				}
				else
					sdoBuffLenght = 0;	// Dimensione del dato non nota
					
				stepSDOc++;
			}
			
			
		}
		else
		{
			// Devo inviare il comando Abort SDO Transfer
			SDOAbortTransferProtocol(sdoIndex, sdoSubIndex, resp);
		}
	}
	else if (!TMR_CAN_SDO)
	{
		stepSDOc = 0;
		sdoCurrProtocol = SDONull;
		
	}
}

/*=================================
 * void SDOUploadRequest()
 *
 * Il client richiede l'invio del 
 *	successivo segmento di dati
 *
 *=================================*/
void SDOUploadRequest()
{
	td_SDOsUploadReq buff;
	
	buff.byte0.ccs = 3;
	buff.byte0.t = sdoToggleBit;
	buff.byte0.X = 0;
	buff.reserved0 = 0;
	buff.reserved1 = 0;
	buff.reserved2 = 0;
	buff.reserved3 = 0;
	buff.reserved4 = 0;
	buff.reserved5 = 0;
	buff.reserved6 = 0;
	
	SendSDO((byte *) &buff, 8);
	
	stepSDOc++;
	
}

/*================================
 *
 * void SDOUploadConfirm()
 *
 * Il client rimane in attesa
 * successivo swgmento dal Server
 *
 *================================*/
void SDOUploadConfirm()
{
	td_SDOsUploadConf *buff;
	CanMessage m;
	dword resp;
	word len;
	short i;
	
	if (CanReceivedPop(CanSDORxChannel, &m))
	{
		resp = (VerSDOUploadConfirm(&m));
		
		if (resp == SDOErrorOK)
		{			
			
			buff = (td_SDOsUploadConf *)&m;
			len = 8 - buff->byte0.n;
					
			sdoToggleBit = buff->byte0.t;
					
			if (sdoBuffLenght >= len && sdoIdBuff < SDO_BUFFER_SIZE - len)
			{							
				for (i = 0; i < len; ++i)
					sdoBuffSegments[sdoIdBuff++] = ((word *)(&buff->segData0))[i];
							
				sdoBuffLenght -= len;
				// Verifico se l'ultimo pacchetto dei dati o devo rimanere ancora in attesa
				if (buff->byte0.c == 0)
				{
					// terminato la ricezione posso scrivere il dato sul dizionario
				//	DicWrite(sdoIndex, sdoSubIndex, (word *)sdoBuffSegments, sdoLenData, sdoLenData);
					setODEntry(sdoIndex, sdoSubIndex, (byte *)sdoBuffSegments, sdoLenData);
						
					SDOProtocolTerminate();
							
					
				}
				else
					stepSDOc = 2;
				
			}
			else
			{
				// Devo inviare il comando Abort SDO Transfer
				SDOAbortTransferProtocol(sdoIndex, sdoSubIndex, resp);
										
			}			
		}
		else
		{
			// Devo inviare il comando Abort SDO Transfer
			SDOAbortTransferProtocol(sdoIndex, sdoSubIndex, resp);
		}
			
	}
}


/*=====================================
 *
 * void SDOsUploadResponse()
 *
 * Invia un Upload SDO Segment Confirm 
 * viene richiamata dal Server durante
 * l'upload di un dato
 *
 *=====================================*/
void SDOsUploadResponse()
{
	td_SDOsUploadConf buff;
	word *pDic;
	word size;
	dword resp;
	word tmp;
	
	buff.byte0.scs = 0;
	buff.byte0.t = sdoToggleBit;
	
	
	if (sdoBuffLenght > 7)
	{			
		buff.byte0.n = 0;
		buff.byte0.c = 1;
		sdoBuffLenght -= 7;
	}
	else
	{
		tmp = 8 - buff.byte0.n;
		buff.byte0.n = tmp;
		buff.byte0.c = 0;
		sdoBuffLenght = 0;
		
		SDOProtocolTerminate();
	}
	
	SendSDO((byte *)&buff, 8);
	
}

/*========================================================
 *
 * dword VerSDOInitDownloadRequest(CanMessage *m)
 *
 * Verifica se il Messaggio ricevuto è del tipo
 * confirm download del server
 * 
 * Inputs:
 *		CanMessage *m: messaggio da analizzare 
 * Return 
 *		TRUE: se il messaggio è del tipo confirm download
 *		FALSE: altrimenti
 *=======================================================*/
dword VerSDOUploadRequest(CanMessage *m)
{
	td_SDOsUploadReq* p; 

	if (m->len != 8)
		return FALSE;
	
	
	p = (td_SDOsUploadReq *)m->data;
	
	if (p->byte0.ccs != 3)
		return SDOErrorProtocolTimeOut;
	
	else if (p->byte0.t != sdoToggleBit)
	
		return SDOErrorToggleBit;
	
	else 
	{
		
		sdoToggleBit = p->byte0.t;
		return SDOErrorOK;
	}
		
}

/*=======================================================
 * bool VerSDOInitUploadConfirm(CanMessage *m)
 *
 * Verifica se il Messaggio ricevuto è del tipo
 * confirm Initiate upload del server
 * 
 * Inputs:
 *		CanMessage *m: messaggio da analizzare 
 * Return 
 *	Codice di Errore
 *=======================================================*/
dword VerSDOInitUploadConfirm(CanMessage *m)
{
	td_SDOsInitUploadConf* p; 

	if (m->len != 8)
		return FALSE;
	
	
	p = (td_SDOsInitUploadConf *)m->data;
	
	if (p->byte0.scs != 2 || p->byte0.X != 0)
		return SDOErrorProtocolTimeOut;
	else
		return SDOErrorOK;
	
}

/*=======================================================
 * bool VerSDOInitUploadConfirm(CanMessage *m)
 *
 * Verifica se il Messaggio ricevuto è del tipo
 * confirm Initiate upload del server
 * 
 * Inputs:
 *		CanMessage *m: messaggio da analizzare 
 * Return 
 *	Codice di Errore
 *=======================================================*/
dword VerSDOUploadConfirm(CanMessage *m)
{
	td_SDOsUploadConf* p; 

	if (m->len != 8)
		return FALSE;
	
	
	p = (td_SDOsUploadConf *)m->data;
	
	if (p->byte0.scs != 4 || p->byte0.t == sdoToggleBit)
		return SDOErrorProtocolTimeOut;
	else
		return SDOErrorOK;
	
}

/*==========================================
 *
 * bool InitSDOBlockDownloadProtocol()
 *
 * Funzione server per l'implementazione del
 * protocollo Initiate Block Download
 *
 *===========================================*/
bool InitSDOBlockDownloadProtocol()
{
	CanMessage m;
	
	switch (stepSDOc)
	{
		case 0:
			
			// Invio del Request di download al server
			buffInitDownloadReq.byte0.ccs = 6;	// Block download specifer
			buffInitDownloadReq.byte0.X = 0;		// not used, always 0
			buffInitDownloadReq.byte0.cc = 0;		// 0: client does not support generating CRC on data, 1: Client supports generating CRC on data			
			buffInitDownloadReq.byte0.s = 0;
			buffInitDownloadReq.byte0.cs = 0;
			 
			FormattIndexSubIndex(buffInitDownloadReq.m, sdoIndex, sdoSubIndex);
			*((dword *)buffInitDownloadReq.size) = 0;
			
			
			SendSDO((byte *)&buffInitDownloadReq, 8);
			stepSDOc++;
			break;
			
		case 1:
		
			// Ricezione del Confirm download dal server
			if (CanReceivedPop(CanSDORxChannel, &m))
			{
				// Verifico se si tratta di confirm download dal server
				if (VerSDOBlockDownloadConfirm(&m))
					stepSDOc++;
				else
				{
					// Devo inviare il comando Abort SDO Transfer
					SDOAbortTransferProtocol(sdoIndex, sdoSubIndex, SDOErrorProtocolTimeOut);					
				}
			}
			break;
			
		case 2:
			break;
	}
}

/*=====================================================
 * Comando di Protocollo SDO
 *
 * Inputs:
 *		word ptc: protocollo SDO
 *		word index: Indice del Dizionario
 *		byte subIndex: Indice secondario del dizionario
 *======================================================*/
void CmdSDOBlockDownload(byte nodeId, word ptc, word index, byte subIndex)
{
	sdoDeviceNodeId = nodeId;
	sdoIndex = index;
	sdoSubIndex = subIndex;
	sdoCurrProtocol = ptc;
	stepSDOc = 0;
	
}

/*=======================================================
 * bool VerSDOBlockDownloadConfirm(CanMessage *m)
 *
 * Verifica se il Messaggio ricevuto è del tipo
 * confirm download del server
 * 
 * Inputs:
 *		CanMessage *m: messaggio da analizzare 
 * Return 
 *		TRUE: se il messaggio è del tipo confirm download
 *		FALSE: altrimenti
 *=======================================================*/
bool VerSDOBlockDownloadConfirm(CanMessage *m)
{
	td_SDObInitDownloadConf* p; 

	if (m->len != 8)
		return FALSE;
	
	
	p = (td_SDObInitDownloadConf *)m->data;
	
	if (p->byte0.scs != 5 || p->byte0.X != 0 || p->byte0.ss != 0 || !VerSDOIndexSubIndex(p->m) || p->reserved != 0)
		return FALSE;
	
	sdoBlkSize = p->blksize;
	
}















/*=============================================
 *
 *	*******	R O U T I N E  C O M U N I	*******
 *
 *=============================================*/
 
 
/*============================================
 * void SDOProtocolTerminate()
 *
 * Termina la sequenza del Protocollo corrente
 *
 *============================================*/
void SDOProtocolTerminate()
{
	stepSDOc = 0;
	sdoCurrProtocol = SDONull;
}

/*================================================
 *
 * Verifica se la stringa ricevuta corrisponde 
 * alla Index e sub index in attesa
 *
 *================================================*/
bool VerSDOIndexSubIndex(byte *buff)
{
	if (buff[0] == (sdoIndex & 0x00FF) && (buff[1] == ((sdoIndex & 0xFF00) >> 8) && 
		sdoSubIndex == ((buff[2] & 0x00FF))))
		return TRUE;
	else
		return FALSE;
}



/*=================================================================
 * void FormattIndexSubIndex(word *buff, word index, byte subIndex)
 *
 * Formattazione del index e subindex di un dizionario
 * per un comando SDO
 *
 * Inputs:
 *	word index: indice del dizionario
 *	word subIndex: sotto indice del dizionario
 *
 * Outputs:
 *	word *buff: valore formattato per il protocollo SDO
 *
 *================================================================*/ 
void FormattIndexSubIndex(byte *buff, word index, byte subIndex)
{
	
	buff[0] = index & 0x00FF;
	buff[1] = (index & 0xFF00) >> 8;
	buff[2] = subIndex;
}


/*===============================================================
 * void GetIndexSubIndex(word *buff, word *index, byte *subIndex)
 *	Recupera index e sub-index da un comando
 *
 * Inputs:
 *	word *buff: stringa di comando
 *	
 * Outputs:
 *	word *index: indice 
 *	word *subIndex: sub-indice
 *
 *==============================================================*/
void GetIndexSubIndex(byte *buff, word *index, byte *subIndex)
{
	*subIndex = buff[2];
	*index = buff[0] | (buff[1] << 8);
}
/*=======================================
 *
 * Invio Comando SDO tramite CAN
 *
 *	Inputs:
 *		word *buff: buffer da inviare
 *  	short len: numero di byte del dato
 *
 *========================================*/
void SendSDO(byte *buff, short len)
{
	short i;
	CanMessage m;
	
	m.cob_id = CAN_BUFF_SDO_TX | deviceNodeId;
	m.rtr = 0x00;
	m.len = len;
	
	for (i = 0; i < 8; ++i)
		m.data[i] = buff[i];

	CanTransmitPush(CanSDOTxChannel, &m);
	
	
}

void SendSDOCobId(byte cobId, byte *buff, short len)
{
	short i;
	CanMessage m;
	
	m.cob_id = CAN_BUFF_SDO_TX | cobId;
	m.rtr = 0x00;
	m.len = len;
	
	for (i = 0; i < 8; ++i)
		m.data[i] = buff[i];
	

	
	CanTransmitPush(CanSDOTxChannel, &m);
	
}

