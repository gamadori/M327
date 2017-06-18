/*
 * BMAlfacol.c
 *
 *  Created on: Jun 18, 2017
 *      Author: info
 */
#include "cpu.h"
#include "plc.h"
#include "Alfacol.h"
#include "bldcUser.h"
#include "pid.h"
#include "EEprom.h"
#include "Encoder.h"
#include "Ramp.h"
#include "Drive.h"
#include "Tuning.h"
#include "Current.h"
#include "Sdo.h"
#include "Slave.h"
#include "Bus.h"
#include "Homing.h"
#include "CanOpen.h"
#include "Router.h"
#include "Current.h"
#include "Settings.h"
#include "BM/BMPos.h"
#include "BM/BMSettings.h"
#include "BM/BMAlfacol.h"

PtlRoadMap BMAlfaRoadMap[] =
{
	//	Address,							Type,	Size,				ReadOnly, EEprom, CMD
		
		{mmPosReal,							Double,	BM_NUM_AXES,			TRUE,	FALSE,	0x0004},	// Posizione Reale dell'asse in mm
		{mmPosVirt,							Double,	BM_NUM_AXES,			TRUE,	FALSE,	0x0005},	// Posizione Virtuale dell'asse in mm
		{mmPosErr,							Double,	BM_NUM_AXES,			TRUE,	FALSE,	0x0006},	// Errore di Posizione dell'asse in mm
		
		{NULL,								0,		0,					0,		0,		0}	
};
