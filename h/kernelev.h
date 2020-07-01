/*
 * kernelev.h
 *
 *  Created on: May 11, 2020
 *      Author: OS1
 */

#ifndef H_KERNELEV_H_
#define H_KERNELEV_H_
class PCB;
class Event;
class KernelEv{

public:
	KernelEv(unsigned ivtNo);
	~KernelEv();

	void wait();
	void signal();

	unsigned myIVTnum;
		PCB* myPCB;
	volatile int value;
		//Event* myEvent;

};




#endif /* H_KERNELEV_H_ */
