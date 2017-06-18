/*
 * BMPlc.h
 *
 *  Created on: Jun 18, 2017
 *      Author: info
 */

#ifndef BMPLC_H_
#define BMPLC_H_
#include "plc.h"
#include "Bits.h"
#include "Slave.h"


#define setOut(bits)		outValue |= bits
#define disOut(bits)		outValue &= ~bits

#define outProbe(value) (value) ? (outValue |= Bits[0]) : (outValue &= ~Bits[0])

#define outPressVert0(value)	(value) ?(outValue |= (Bits[0] | (Bits[8]))) : (outValue &= ~(Bits[0] | Bits[8]))

#define outPressVert1(value)	(value) ?(outValue |= (Bits[1] | (Bits[9]))) : (outValue &= ~(Bits[1] | Bits[9]))

#define outPressVert2(value)	(value) ?(outValue |= (Bits[2] | (Bits[10]))) : (outValue &= ~(Bits[2] | Bits[10]))

#define outPressVert3(value)	(value) ?(outValue |= (Bits[3] | (Bits[11]))) : (outValue &= ~(Bits[3] | Bits[11]))

#define outPressVert4(value)	(value) ?(outValue |= (Bits[4] | (Bits[12]))) : (outValue &= ~(Bits[4] | Bits[12]))

#define outPressVert5(value)	(value) ?(outValue |= (Bits[5] | (Bits[13]))) : (outValue &= ~(Bits[5] | Bits[13]))


#define inpStart				((bus_IO_InputVal[0] & Bits[0]) == Bits[0])

#endif /* BMPLC_H_ */
