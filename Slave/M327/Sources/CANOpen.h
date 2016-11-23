/*
 * CANOpen.h
 *
 *  Created on: Jan 2, 2016
 *      Author: gianluca
 */

#ifndef CANOPEN_H_
#define CANOPEN_H_

#define NO_SUCH_INDEX 0x11

/** Return value that indicates a successful operation */
#define SUCCESSFUL 0x12


// predefined error-codes. neccessary for sending an emergency-message
 /** internal device error: no error
  */
  #define NO_ERROR 0x0000

/** internal device error: generic error
 */
#define GENERIC_ERROR 0x1000

/** internal device error: current error
 */
#define CURRENT_ERROR 0x2000

/** internal device error: voltage error
 */
#define VOLTAGE_ERROR 0x3000

/** internal device error: hardware error 
 */
#define HARDWARE_ERROR 0x5000
// ... (the user can define more errors here) 

extern byte canDeviceNodeId;

extern byte stateMachine;

void CANOPENServer();

void CanOpenSetID(byte id);

#endif /* CANOPEN_H_ */
