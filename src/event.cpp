#include "usable.h"
#include "event.h"
#include "kernelev.h"


Event::Event(IVTNo ivtNo) {
	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	//provera za vrednost ivntno
		myImpl = new KernelEv(ivtNo);
	unlockHard;
	#endif

}

Event::~Event() {
	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	if(myImpl) delete myImpl;
	unlockHard;
	#endif
}

void Event::wait() {
	 myImpl->wait();
}

void Event::signal() {
	 myImpl->signal();
}
/*
 * event.cpp
 *
 *  Created on: May 11, 2020
 *      Author: OS1
 */

