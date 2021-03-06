#include "thread.h"
#include "usable.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include "timer.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	myPCB=new PCB(stackSize,timeSlice,this);
	unlockHard;
	#endif

}

void Thread::start() {
	if(myPCB!=nullptr)
	myPCB->start();

}

void Thread::waitToComplete() {
	if(myPCB!=nullptr)
	myPCB->waitToComplete();
}

Thread::~Thread() {

		//cout<<"thread desturkotr pozvan"<<endl;
		waitToComplete();
		//cout<<"thread deleted";
		#ifndef BCC_BLOCK_IGNORE
		lockHard;
		if (myPCB!=nullptr)
			delete myPCB;
		unlockHard;
		#endif

		myPCB = nullptr;
}

ID Thread::getId() {
	if(myPCB!=nullptr)
	return myPCB->getID();
	else return -1;
}

ID Thread::getRunningId() {

	return PCB::getRunningId();
}

Thread* Thread::getThreadById(ID id) {

	return PCB::getThreadById(id);
}



void dispatch() {

	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	explicitCall=1;
	timer();
	unlockHard;
	#endif

}

void Thread::signal(SignalId signal) {
	if (myPCB==nullptr) return;
		myPCB->signal(signal);
}

void Thread::registerHandler(SignalId signal, SignalHandler handler) {
	if (myPCB==nullptr) return;
		myPCB->registerHandler(signal,handler);
}

void Thread::unregisterAllHandlers(SignalId id) {
	if (myPCB==nullptr) return;
		myPCB->unregisterAllHandlers(id);
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	if (myPCB==nullptr) return;
		myPCB->swap(id, hand1, hand2);
}

void Thread::blockSignal(SignalId signal) {
	if (myPCB==nullptr) return;
		myPCB->blockSignal(signal);
}

void Thread::blockSignalGlobally(SignalId signal) {
	 PCB::blockSignalGlobally(signal);
}

void Thread::unblockSignal(SignalId signal) {
	if (myPCB==nullptr) return;
		myPCB->unblockSignal(signal);
}

void Thread::unblockSignalGlobally(SignalId signal) {
	PCB::unblockSignalGlobally(signal);
}
