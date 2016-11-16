/*
 * Encoder.h
 *
 *  Created on: Jun 23, 2016
 *      Author: gianluca
 */

#ifndef ENCODER_H_
#define ENCODER_H_

extern long rpos[];						// Actual Position

extern short encVel[];


void InitEncoder();

void GetEncoders();

#endif /* ENCODER_H_ */
