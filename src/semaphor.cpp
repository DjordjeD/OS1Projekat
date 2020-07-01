
/*
 * smph.cpp
 *
 *  Created on: May 5, 2020
 *      Author: OS1
 */
#include "../h/semaphor.h"
#include "usable.h"
#include "kersem.h"
Semaphore::Semaphore(int init) {
	myImpl=new KernelSem(init);
}

Semaphore::~Semaphore() {
	if(myImpl!=nullptr)
	{
		delete myImpl;//lock unlock
		myImpl=nullptr;
	}

}

int Semaphore::wait(Time maxTimeToWait) {
	return myImpl->waitKernel(maxTimeToWait);
}

int Semaphore::signal(int n) {
	return myImpl->signalKernel(n);
}

int Semaphore::val() const {
	return myImpl->valKernel();
}

