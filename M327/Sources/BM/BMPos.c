/*
 * BMPos.c
 *
 *  Created on: Jun 18, 2017
 *      Author: info
 */


#include <math.h>
#include "cpu.h"

#include "Encoder.h"
#include "BM/BMSettings.h"
#include "BM/BMPos.h"

short unitDef;

short dirAxe[] =
{
	-1,
	1,
	1
};

short dirEnc[] =
{
	1,
	1,
	1,
	1
};

long posReal[BM_NUM_AXES];	// Posizione Reale degli assi

long posVirt[BM_NUM_AXES];	// Posizione Virtuale degli assi

long posErr[BM_NUM_AXES];		// Errore di Posizione degli assi

long impMinPos[BM_NUM_AXES];	// Posizione di Minimo

long impMinOffset[BM_NUM_AXES];


double mmPosReal[BM_NUM_AXES];	// Posizione Reale degli assi in unita'

double mmPosVirt[BM_NUM_AXES];	// Posizione Virtuale degli assi in unita'

double mmPosErr[BM_NUM_AXES];	// Errore di Posizione degli assi in unita'

double mmMinOffset[BM_NUM_AXES];	// Dimensione dell'asta nella unità predefinita

double impMm[BM_NUM_AXES];		// Rapporto mm/impulsi


double inchPosReal[BM_NUM_AXES];	// Posizione Reale degli assi in unita'

double inchPosVirt[BM_NUM_AXES];	// Posizione Virtuale degli assi in unita'

double inchPosErr[BM_NUM_AXES];	// Errore di Posizione degli assi in unita'

double inchMinOffset[BM_NUM_AXES];	// Dimensione dell'asta nella unità predefinita

double impInch[BM_NUM_AXES];		// Rapporto impulsi



/////////////////////////////////////////////////////////

double encMmPos[BM_NUM_ENC];

double encInchPos[BM_NUM_ENC];

long encImpMinPos[BM_NUM_ENC];	// Minima posizione


long encImpMinOffset[BM_NUM_ENC];	// Offset impulsi


double encImpMm[BM_NUM_ENC];		// Rapporto mm/impulsi

double encMmMinOffset[BM_NUM_ENC];


double encImpInch[BM_NUM_ENC];

double encInchMinOffset[BM_NUM_ENC];

long CalcEncoder(short axe, long posLame);

/*
 * Calcolo della posizione degli assi 
 * nell'unita' corrente
 */
void PositionServer()
{
	short axe;
	long pos;
	
	// Calcolo della posizione di ciascun asse
	for (axe = 0; axe < BM_NUM_AXES; ++axe)
	{
		
		
		
		posErr[axe] = posVirt[axe] - impMinPos[axe];						
		
		if (!isnan(impMm[axe]) && impMm[axe] != 0 && !isnan(mmMinOffset[axe]))
		{
			
			
			mmPosReal[axe] = (mmMinOffset[axe] + dirAxe[axe] * ((impMinPos[axe] - (double)(posReal[axe])) / impMm[axe]));			
			
			mmPosVirt[axe] = (mmMinOffset[axe] + dirAxe[axe] * ((impMinPos[axe] - (double)(posVirt[axe])) / impMm[axe]));
						 			
			mmPosErr[axe] = (long)(posErr[axe] / impMm[axe]);
			
		}
		else
		{
			// Parametro imp/Unita non settato correttamente
			mmPosReal[axe] = 0;
			mmPosVirt[axe] = 0;
			mmPosErr[axe] = 0;
		}
		
		if (!isnan(impInch[axe]) && impInch[axe] != 0 && !isnan(inchMinOffset[axe]))
		{			
					
			inchPosReal[axe] = (inchMinOffset[axe] + dirAxe[axe] * ((impMinPos[axe] - (double)(posReal[axe])) / impInch[axe]));
			
			inchPosVirt[axe] = (inchMinOffset[axe] + dirAxe[axe] * ((impMinPos[axe] - (double)(posVirt[axe])) / impInch[axe]));			
						 			
			inchPosErr[axe] = posErr[axe] / impInch[axe];
		}
		else
		{
			// Parametro imp/Unita non settato correttamente
			mmPosReal[axe] = 0;
			mmPosVirt[axe] = 0;
			mmPosErr[axe] = 0;
		}
		
	}
	
	for (axe = 0; axe < BM_NUM_ENC; ++axe)
	{
		
		encPosRel[axe] = encPos[axe] - encPos0[axe];
		
		if (!isnan(encImpMm[axe]) && encImpMm[axe] != 0)
		{
			pos = (long)(encMmMinOffset[axe] + dirEnc[axe] * ((double)(encPosRel[axe] - encImpMinPos[axe]) / encImpMm[axe]));
			
			encMmPos[axe] = pos;
			
		}
		else
		{
			// Parametro imp/Unita non settato correttamente
			encMmPos[axe] = 0;
			
		}
		
		if (!isnan(encImpInch[axe]) && encImpInch[axe] != 0)
		{
			pos = (long)(encInchMinOffset[axe] + dirEnc[axe] * ((double)(encPosRel[axe] - encImpMinPos[axe]) / encImpInch[axe]));
			encInchPos[axe] = pos;
			
		}
		else
		{
			// Parametro imp/Unita non settato correttamente
			encInchPos[axe] = 0;
			
		}
	}
		
}



long CalcEncoderFromMm(short axe, double posLame)
{
	long pos;
	
	if (!isnan(impMm[axe]))
	{
		impMinOffset[axe] = (long)(mmMinOffset[axe] * impMm[axe]);
		pos = (long)(posLame * impMm[axe]);
		
		return  CalcEncoder(axe, pos);
	}
	else
		return 0;
}

long CalcEncoderFromInch(short axe, double posLame)
{
	long pos;
	
	if (!isnan(impInch[axe]))
	{
		impMinOffset[axe] = (long)(inchMinOffset[axe] * impInch[axe]);
		pos = (long)(posLame * impInch[axe]);
		
		return  CalcEncoder(axe, pos);
	}
	else
		return 0;
}

long CalcEncoder(short axe, long posLame)
{
	
	long pos = ((impMinPos[axe] * dirAxe[axe] + impMinOffset[axe]) - posLame) * dirAxe[axe];
	return pos;
	
}
