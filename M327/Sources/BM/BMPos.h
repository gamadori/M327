/*
 * BMPos.h
 *
 *  Created on: Jun 18, 2017
 *      Author: info
 */

#ifndef BMPOS_H_
#define BMPOS_H_

/* MODULE Positions */

extern long posReal[];	// Posizione Reale degli assi

extern long posVirt[];	// Posizione Virtuale degli assi

extern long posErr[];		// Errore di Posizione degli assi

extern long impMinPos[];

extern long impMinOffset[];


extern double mmPosReal[];	// Posizione Reale degli assi in unita'

extern double mmPosVirt[];	// Posizione Virtuale degli assi in unita'

extern double mmPosErr[];	// Errore di Posizione degli assi in unita'

extern double mmMinOffset[];	// Dimensione dell'asta nella unità predefinita

extern double impMm[];		// Rapporto mm/impulsi


extern double inchPosReal[];	// Posizione Reale degli assi in unita'

extern double inchPosVirt[];	// Posizione Virtuale degli assi in unita'

extern double inchPosErr[];	// Errore di Posizione degli assi in unita'

extern double inchMinOffset[];	// Dimensione dell'asta nella unità predefinita

extern double impInch[];		// Rapporto impulsi




/////////////////////////////////////////////////////////

extern double encMmPos[];

extern double encInchPos[];

extern long encImpMinPos[];	// Minima posizione

extern long encImpMinOffset[];	// Offset impulsi

extern double encImpMm[];		// Rapporto mm/impulsi

extern double encMmMinOffset[];

extern double encImpInch[];

extern double encInchMinOffset[];


void PositionServer();

long CalcEncoderFromMm(short axe, double posLame);

long CalcEncoderFromInch(short axe, double posLame);

long CalcEncoder(short axe, long posLame);

#endif /* BMPOS_H_ */
