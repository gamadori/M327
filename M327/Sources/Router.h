/*
 * Router.h
 *
 *  Created on: Jan 13, 2017
 *      Author: info
 */

#ifndef ROUTER_H_
#define ROUTER_H_

enum TypeCmd
{
	cmdError,
	cmdAlfacol,
	cmdSDO	
};
enum TypeOp
{
	opGet,
	opSet
};
byte RouterVariable(byte numStation, char *buff, word* frame, byte cmdType);

byte RouterFunction(byte numStation, char *buff, word* frame);

#endif /* ROUTER_H_ */
