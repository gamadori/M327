/*
 * BoardCycle.h
 *
 *  Created on: Nov 17, 2016
 *      Author: gianluca
 */

#ifndef BOARDCYCLE_H_
#define BOARDCYCLE_H_

enum BoardStatus
{
	boardInit,
	boardSettings,
	boardRun
};

#define BoardInRun	(boardState == boardRun)

void BoardInit();

void BoardServer();





#endif /* BOARDCYCLE_H_ */
