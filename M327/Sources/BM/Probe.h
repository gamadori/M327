/*
 * Probe.h
 *
 *  Created on: Jun 18, 2017
 *      Author: info
 */

#ifndef PROBE_H_
#define PROBE_H_

extern short probeHomMaxErrBatt;

extern word probeTimeOut;

extern long probePos0;

extern word probeStatus;

void InitProbe();

void ResetProbe();

void ProbeHoming();

void CmdProbeHome();

void ProbeMeasure();

void CmdProbeMeasure();

void CmdProbeSearchMin();

#endif /* PROBE_H_ */
