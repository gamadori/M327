/*
 * Encoder.h
 *
 *  Created on: Jun 23, 2016
 *      Author: gianluca
 */

#ifndef ENCODER_H_
#define ENCODER_H_

extern long rpos[];						// Actual Position

extern long encPos[];

extern short encVel[];


extern long encPos0[];						// Posizione di zero

extern long encPosRel[];					// Posizione Relativa allo zero

void InitEncoder();

void GetEncoders();

#endif /* ENCODER_H_ */
