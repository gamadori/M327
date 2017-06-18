/*
 * BMProd.c
 *
 *  Created on: Jun 18, 2017
 *      Author: info
 */
#include "cpu.h"

#include "Bits.h"
#include "plc.h"
#include "Pid.h"
#include "Timer.h"
#include "Ramp.h"
#include "Bus.h"

#include "BM/BMPlc.h"
#include "BM/BMTimers.h"
#include "BM/BMCycle.h"
#include "BM/BMAlarms.h"
#include "BM/BMSettings.h"
#include "BM/BMPos.h"
#include "BM/BMProd.h"


short stepCycle = 0;				// Stato del ciclo di produzione

short currMoulding = 0;				// Asta corrente

// Produzione

long codeProg;

byte seqProd[NMAX_SEQ];				// Sequenze di produzione

long prodFrameDes;					// Produzione desiderata in cornici

long prodGlobalMade[NUM_MOULDING];	// Produzione Globale

long prodDes[NUM_MOULDING];			// Per ciascuna asta la produzione desiderata

long prodMade[NUM_MOULDING];		// Per ciascuna asta la produzione eseguita 

long prodCurr = 1;					// Numero di aste corrente prodotte


short InAutomatic;					// Macchina in Automatico o Manuale

short idPhase;						// Fase della sequenza corrente di produzione

ProdSetting prodSet;				// Settaggi della modalita' e sequenze di taglio

word oldSequence;					// Memorizza l'ultima sequenza di taglio impostato

short oldMoulding;					// Ultima asta eseguita

short cmdAskMoulding = MouldNone;	// Richiesta asta fuori sequenza

short unita;

// Posizione di sicurezza

double mmSicMinMould;				// Asta minima che non riciede spostamento in mm

long sicMinMouldImp;				// Asta minima che non richiede spostamento di sicurezza

long sicMinMouldImpAbs; 			// Impulsi Asta minima che non richiede spostamento di sicurezza


double mmDiffNeedSic;

dword diffNeedSicImp;				// Quando la differenza della posizione attuale a quella successiva e inferiore a tale valore deve effettuare il posizionamento in sicurezza

// Velocita' e Accelerazioni
short speedWorkAxe[BM_NUM_AXES];		// Velocita' di lavoro degli assi

short speedWorkAxeLow[BM_NUM_AXES];	// Velocita' di lavoro bassa

short accWorkAxe[BM_NUM_AXES];			// Accelerazioni di lavoro

// Posizioni di lavoro
long protPosWork;					// Posizione di lavoro della protezione in impulsi.


// Dimensioni dell'asta

long mouldSizeImp[NUM_MOULDING];	// Dimensioni delle aste e dei fuori misura in Impulsi

long mouldDepthImp[NUM_DEPTH];		// Dimensioni dello spessore dell'asta in impulsi

double mmMouldSize[NUM_MOULDING];	// Dimensioni delle aste e dei fuori misura in mm

double mmMouldDept[NUM_DEPTH];	// Dimensioni dello spessore dell'asta in mm

double mmCorrProt;					// Correzione sulla protezione

short waitInSic;

long offsetSicImp;

double mmOffsetSic;


/* Dichiarazione funzioni */
void LoadSequence();

void CalculatePos();

void VerifyNeedDeviation();

bool ProdOversize();

void ProdNextPosition();

void ProdMoulding();

bool CompMouldNeedDev();


/*======================================
 * Module:
 *	void ProductionServer()
 * Description:
 *	Ciclo di produzione della macchina
 *=======================================*/ 
void ProductionServer()
{
	short speed;
	long pos;
	
	switch (stepCycle)
	{
		case 0:
															
			currMoulding = MouldA;
			
			prodCurr = 1;
			idPhase = 0;
			stepCycle = 2;
			
			oldSequence = prodSet.sequence;
			
			//currSeqProd = MouldA;
			LoadSequence();
			
			cmdAskMoulding = MouldNone;
			oldMoulding = MouldA;
			
			
			break;
			
		case 2:
		
			CalculatePos();
												
			VerifyNeedDeviation();
			macStatus.statusMac = MacInProduction;
			break;
			
		case 3:
			// Posizionamento degli assi
			CalculatePos();
		
			// Devo muovermi lentamente se:
			// il movimento e' verso le lame
			// Se la posizione in cui mi trovo o devo andare e sotto alle soglie di
			// sicurezza
			
			if (mouldSizeImp[currMoulding] < vpos[Axe1] || 
				vpos[Axe1] < sicMinMouldImpAbs || 
				mouldSizeImp[currMoulding] < sicMinMouldImpAbs)
				speed = speedWorkAxeLow[Axe1];
			else
				speed = speedWorkAxe[Axe1];
				
			
			BusMove(Axe1, speed, mouldSizeImp[currMoulding], accWorkAxe[Axe1], 0);
						
			
			if (macOptions.Protection)
			{
				// Devo posizionare la protezione
							
				BusMove(AxeProtection, speedWorkAxe[AxeProtection], protPosWork, accWorkAxe[AxeProtection], 0);				
			}
			
			if (macStatus.statusMac != MacInProdTerminate)
			{
				stepCycle++;
			}
			else
			{
				stepCycle = 6;
			}
			
			break;
		case 4:
			
			if (inpStart && prodSet.enableBackMould && mouldSizeImp[currMoulding] < vpos[Axe1])
				stepCycle = 8;
			
			else if (vpos[Axe1] == bus_AxisPos[Axe1])
			{
				if (!ProdOversize())
				{				
					if (inpStart)
					{
						stepCycle++;
					}
				}
			}
			
			break;
						
		case 5:
			if (vpos[AxeProtection] == bus_AxisPos[AxeProtection])
			{
				stepCycle++;
			}
			
		case 6:
			
			if (vpos[Axe1] == bus_AxisPos[Axe1])
			{
				if (!ProdOversize())
				{				
					if (inpStart)
					{
						stepCycle++;
					}
				}
			}
			
			break;
			
		case 7:
			
			if (!inpStart)
			{
				
				prodMade[currMoulding]++;
				prodGlobalMade[currMoulding]++;
				
				
				if (InAutomatic)
				{
					ProdNextPosition();		
					
					VerifyNeedDeviation(); //stepCycle = 3;											
					
				}
				else
				{
					VerifyNeedDeviation();
				}
			
				ProdMoulding();
			}
			break;
			
			
		case 8:
			// Eseguo lo scostamento
			
			pos = vpos[Axe1] + offsetSicImp;
			
			BusMove(Axe1, speedWorkAxeLow[Axe1], pos, accWorkAxe[Axe1], 0);
						
			
			stepCycle++;
			break;
			
		case 9:
			if (vpos[Axe1] == bus_AxisPos[Axe1] || Alarms.AxeXLimSwPos)
			{
				// Inizializzo il Timer
				TMR_CYCLE1 = MSEC(waitInSic);
				
				stepCycle++;
			}
			break;
			
		case 10:
			if (!TMR_CYCLE1)
			{
				stepCycle = 3;
			}
			
			break;	
		
	}
					
	
}

/*
 * Carica le sequenze di taglio in base 
 * ai settaggi del ciclo
 */
 
void LoadSequence()
{
	switch (prodSet.sequence)
	{
		case SeqAB:
			seqProd[0] = MouldA;
			seqProd[1] = MouldB;
			
			//(maxOversize > 0) ? (seqProd[2] = OverSide1) : (seqProd[2] = MouldNone);
			
			//(maxOversize > 1) ? (seqProd[3] = OverSide2) : (seqProd[3] = MouldNone);
				
			seqProd[2] = MouldNone;
			seqProd[3] = MouldNone;
			
			break;
			
		case Seq2A2B:
			
			seqProd[0] = MouldA;
			seqProd[1] = MouldA;
			
			seqProd[2] = MouldB;
			seqProd[3] = MouldB;
			
			//(maxOversize > 0) ? (seqProd[4] = OverSide1) : (seqProd[2] = MouldNone);
			
			//(maxOversize > 1) ? (seqProd[5] = OverSide2) : (seqProd[2] = MouldNone);
			
			break;
			
		case SeqnAnB:
			seqProd[0] = MouldA;
			seqProd[1] = MouldNone;
			seqProd[2] = MouldNone;
			seqProd[3] = MouldNone;
			
			break;
			
			
	
	}
}

/* 
 * Calcolo delle produzioni e posizioni
 * per avviare la sequenza di taglio
 */
void CalculatePos()
{
	short i;
	
	if (unita == 1)
	{
		for (i = 0; i < NUM_MOULDING; ++i)
		{
			mouldSizeImp[i] = CalcEncoderFromMm(Axe1, mmMouldSize[i] + 2 * mmMouldDept[DepthSideB]);		
		}
			
		sicMinMouldImpAbs = CalcEncoderFromMm(Axe1, mmSicMinMould);
	
		diffNeedSicImp = (long)(mmDiffNeedSic * impMm[Axe1]);
		
		offsetSicImp = (long)(mmOffsetSic * impMm[Axe1]);
		
		protPosWork = CalcEncoderFromMm(AxeProtection, mmMouldDept[DepthSideC] + mmCorrProt);
	}
	else
		sicMinMouldImpAbs = CalcEncoder(Axe1, sicMinMouldImp);
	
	prodDes[MouldB] = prodDes[MouldA] = prodFrameDes << 1;	    	
}

/*======================================
 * Module:
 *	void VerifyNeedDeviation()
 *
 * Description:
 *	Verifica se e' necessario il ciclo di scostamento
 *	
 *=======================================*/ 

void VerifyNeedDeviation()
{

	if (prodSet.enableMoveSic && (((mouldSizeImp[currMoulding] < sicMinMouldImp && 
		posVirt[0] >= mouldSizeImp[currMoulding])  || 
		
			(CompMouldNeedDev() && InAutomatic) ||
		
			posVirt[0] > mouldSizeImp[currMoulding])))
		stepCycle = 8;
	else
		stepCycle = 3;
}

/*======================================
 * Module:
 *	void CompMouldNeedDev()
 *
 * Description:
 *	Verifica se la differenza dell'asta da 
 *	tagliare e' inferiore della soglia di sicurezza
 *=======================================*/ 

bool CompMouldNeedDev()
{
	long diff;
	diff = mouldSizeImp[currMoulding] - posVirt[Axe1];
	
	if (diff != 0 && diff <= diffNeedSicImp)
		return TRUE;
	else
		return FALSE;
}



/*
 * Richiesta di un fuori misura
 */
bool ProdOversize()
{
	if (cmdAskMoulding >= MouldA && cmdAskMoulding <= OverSize2)
	{
		
		
		
		currMoulding = cmdAskMoulding;
		
		cmdAskMoulding = MouldNone;
		
		ProdMoulding();
		VerifyNeedDeviation();
		
		return TRUE;
	}
	else
		return FALSE;
}


/*************************************************
 * Determina la successiva asta da tagliare
 *
 * 
 **************************************************/
void ProdNextPosition()
{
	bool mA = FALSE;
	bool mB = FALSE;
	short nextMoulding;
	
	if (prodSet.sequence != oldSequence)
	{
		LoadSequence();
		oldSequence = prodSet.sequence;
	}
	
	if (currMoulding < OverSize1)			
		idPhase++;
	
	if (idPhase >= NMAX_SEQ || seqProd[idPhase] >= MouldNone)
	{
		idPhase = 0;
		
	}
	
	
	
	//currMoulding = seqProd[idPhase];
	
	nextMoulding = seqProd[idPhase];
	
	/*
	if (currMoulding != currSeqProd)
	{
		prodCurr = 1;
		currSeqProd = currMoulding;
	}
	else
		prodCurr++;
	*/
	
	switch (prodSet.modality)
	{
		case programmed:
		
			if (prodSet.sequence == SeqnAnB)
			{
				if (prodMade[seqProd[idPhase]] >= prodDes[seqProd[idPhase]])
				{
					if (seqProd[idPhase] == MouldA)
					{
						seqProd[idPhase] = MouldB;
						//currMoulding = MouldB;
						nextMoulding = MouldB;
					}
					else 
					{
						
						macStatus.statusMac = MacInProdTerminate;
						return; // Produzione Terminata
					}
				}
			}
			else 
			{
				if (prodMade[MouldA] >= prodDes[MouldA])
				{
					mA = TRUE;
					//currMoulding = MouldB;	
					nextMoulding = MouldB;
				}
				
				if (prodMade[MouldB] >= prodDes[MouldB])
				{
					mB = TRUE;
					//currMoulding = MouldA;
					nextMoulding = MouldA;
				}
				
				if (mA && mB)
				{
					
					macStatus.statusMac = MacInProdTerminate;
					
					return;
				}
			}
			break;
			
									
	}
	
	currMoulding = nextMoulding;

	
}

/*
 * Calcola la produzione corrente
 */
void ProdMoulding()
{
	if (currMoulding != oldMoulding)
	{
		oldMoulding = currMoulding;
		prodCurr = 1;
	}
	else
		prodCurr++;
}

/***********************************
 *
 * Reset della produzione corrente
 *
 ***********************************/
void ResetProd()
{
	short i;
	
	for (i = 0; i < NUM_MOULDING; ++i)
		prodMade[i] = 0;
}
