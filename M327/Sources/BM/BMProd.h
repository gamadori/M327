/*
 * BMProd.h
 *
 *  Created on: Jun 18, 2017
 *      Author: info
 */

#ifndef BMPROD_H_
#define BMPROD_H_


#define NUM_MOULDING	4

#define NUM_DEPTH		3

#define NMAX_SEQ		4

enum TypeMoulding
{
	
	MouldA,
	MouldB,
	OverSize1,
	OverSize2,
	MouldNone
	
};

enum ModProduction
{
	free,
	programmed
};

enum ProdSeq
{
	SeqAB,
	Seq2A2B,
	SeqnAnB
};

/*
			  A
	   _________________
      |					|
      |				____|
   C  |				|
      |				|	
      |_____________|
      
      		B
 */
enum DepthSide
{
	DepthSideA,
	DepthSideB,
	DepthSideC
};

typedef struct
{
	word modality			:1;
	word sequence			:2;		
	word enableMoveSic		:1;	// Esegue lo scostamento in manuale
	word enableBackMould 	:1;	// Durante movimento torna indietro se e' presente un'asta
} ProdSetting;

extern short unita;

extern double mmSicMinMould;				// Asta minima che non riciede spostamento in mm

extern long sicMinMouldImp;				// Asta minima che non richiede spostamento di sicurezza

extern long codeProg;

extern short stepCycle;

extern short speedWorkAxe[];

extern short speedWorkAxeLow[];

extern short accWorkAxe[];

extern long mouldDepthImp[];

extern dword diffNeedSicImp;				// Quando la differenza della posizione attuale a quella successiva e inferiore a tale valore deve effettuare il posizionamento in sicurezza

extern double mmDiffNeedSic;

extern ProdSetting prodSet;

extern short cmdAskMoulding;

extern short currMoulding;

extern long prodCurr;

extern long prodFrameDes;

extern short cmdCycle;

extern long mouldSizeImp[];

extern double mmMouldSize[];	// Dimensioni delle aste e dei fuori misura in mm

extern long prodMade[];

extern double mmMouldDept[];	// Dimensioni dello spessore dell'asta in mm

extern long prodGlobalMade[];

extern short InAutomatic;

extern long offsetSicImp;

extern double mmOffsetSic;

extern short waitInSic;

extern long protPosWork;

extern double mmCorrProt;

extern short idPhase;						// Fase della sequenza corrente di produzione


void ProductionServer();

void ResetProd();


#endif /* BMPROD_H_ */
