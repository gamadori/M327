/*
 * Route.h
 *
 *  Created on: Dec 14, 2016
 *      Author: gianluca
 */

#ifndef ROUTE_H_
#define ROUTE_H_

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

#endif /* ROUTE_H_ */
