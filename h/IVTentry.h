/*
 * IVTentry.h
 *
 *  Created on: May 11, 2020
 *      Author: OS1
 */

#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_

#include "prepEnt.h"


typedef void interrupt (*InterruptRoutine)(...);
class KernelEv;
class IVTEntry{
public:
	static IVTEntry* IVTable[256];

	IVTEntry(unsigned num, InterruptRoutine newRoutine);
	~IVTEntry();

	// ovo je ili ovde ili u kernel EV;
	void setMyKernelEvent(KernelEv* myKernelEvent);

	//unsigned int callOldInterruptRoutine();
	//void signal();
	InterruptRoutine myOldInterruptRoutine, myNewInterruptRoutine;
	unsigned myIVTnumber;
	KernelEv* myKernelEvent;

};



#endif /* H_IVTENTRY_H_ */
