/*
 * I2C.h
 *
 *  Created on: Aug 8, 2016
 *      Author: info
 */

#ifndef I2C_H_
#define I2C_H_

void I2CInitial(void);

void I2CCmd();

byte I2CRead(byte device, byte address, byte *data, byte count);

byte I2CWrite(byte device, byte address, byte data);


#endif /* I2C_H_ */
