/*
 * CANOpen.h
 *
 *  Created on: Jan 2, 2016
 *      Author: gianluca
 */

#ifndef CANOPEN_H_
#define CANOPEN_H_

#define SUCCESSFUL 0x12

#define START_REMOTE_NODE 				0x01

#define ENTER_PRE_OPERATIONAL_STATE 	0x80

#define RESET_REMOTE_NODE 				0x81

#define RESET_REMOTE_COMMUNICATION 		0x82

#define OPERATIONAL						0x05

#define STOPPED 						0x04

#define PRE_OPERATIONAL 				0x7F

#define NODE_INITIALIZATION				0x00

#define COMMUNICATION_INITIALIZATION	0x10

extern byte stateMachine;

extern word canIDBoards[];

void CANOPENServer();

#endif /* CANOPEN_H_ */
