
#include "kernelev.h"
#include "IVTentry.h"
#include "pcb.h"
#include "usable.h"
#include "SCHEDULE.H"

KernelEv::KernelEv(unsigned ivtNo) {


	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	myIVTnum=ivtNo;
	if(PCB::running->state_pcb!=IDLE)
	myPCB=(PCB*)PCB::running;
	value=1;//proveri
	IVTEntry::IVTable[ivtNo]->setMyKernelEvent(this);
	unlockHard;
	#endif


}


KernelEv::~KernelEv() {

	myPCB=nullptr;
	//mozda treba da se obrisu eventovi?
}

void KernelEv::wait() {


	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	if(value == 1 && myPCB == PCB::running)
	{
		value = 0;

		PCB::running->state_pcb=BLOCKED;

		dispatch();
	}
	unlockHard;
	#endif



}

void KernelEv::signal() {


	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	//cout<<value<<endl;
	//cout<<"povan signal"<<endl;
	if(value == 0){
			value = 1;
			myPCB->state_pcb=READY;
			Scheduler::put(myPCB);
			//syncPrintf("signal VALUE: %d \n",value);
	}
	//potencijalno greska
	unlockHard;
	#endif

}

	//dispathc?


/*
 * kernelev.cpp
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */




