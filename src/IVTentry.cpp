/*
 * IVTentry.cpp
 *
 *  Created on: May 11, 2020
 *      Author: OS1
 */
#include "IVTentry.h"
#include "usable.h"
#include <dos.h>

IVTEntry* IVTEntry::IVTable[256] = {0};

IVTEntry::IVTEntry(unsigned num, InterruptRoutine newRoutine):myIVTnumber(num),myNewInterruptRoutine(newRoutine),myKernelEvent(nullptr) {
IVTable[num]=this;
}

IVTEntry::~IVTEntry() {

	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	setvect(myIVTnumber,myOldInterruptRoutine);
	unlockHard;
	#endif
			myKernelEvent = nullptr;
			myOldInterruptRoutine = 0;
			myNewInterruptRoutine = 0;
			IVTable[myIVTnumber] = nullptr;

}

void IVTEntry::setMyKernelEvent(KernelEv* myKernelEvent) {
	this->myKernelEvent=myKernelEvent;

	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	myOldInterruptRoutine = getvect(myIVTnumber);
	setvect(myIVTnumber,myNewInterruptRoutine);
	unlockHard;
	#endif

}

