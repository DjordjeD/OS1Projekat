#include "pcb.h"
#include "thread.h"
#include "idlePCB.h"
#include <dos.h>

volatile ID PCB::staticID = 0;
volatile PCB* PCB::running = nullptr;

volatile List<PCB*> all_PCB;

	void ispis()
	{
		#ifndef BCC_BLOCK_IGNORE
		lockHard;
		cout<<all_PCB;
		unlockHard;
		#endif
	}

void idle_fun() {
	cout << "IDLE\n";
	while (1) {
	};

}

PCB::PCB() {
	//kernel main

	stack = nullptr;
	ss = 0;
	sp = 0;
	bp = 0;
	//lockCount = 0;
	specialSignalsFlag=1;
	timeSlice = defaultTimeSlice;
	myThread = nullptr;
	sem_unblock_flag = 0;
	parent = 0;
	state_pcb = RUNNING;
	//flag_delete_from_allpcb=1;
#ifndef BCC_BLOCK_IGNORE
	lockHard;
	idPCB = ++staticID;
	all_PCB.addEnd(this);
	unlockHard;
#endif

}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* myThread, State s) {//fali za idle kako se pravi

	if (stackSize < minStack)
		stackSize = minStack;

	if (stackSize > maxStack)
		stackSize = maxStack;

	unsigned long stackCount = stackSize / sizeof(unsigned);
#ifndef BCC_BLOCK_IGNORE
	lockHard;
	stack = new unsigned[stackCount];
	unlockHard;
#endif

	stack[stackCount - 1] = 0x200; // interrupt 1

	if (s != IDLE) {
#ifndef BCC_BLOCK_IGNORE
		stack[stackCount-2] = FP_SEG(wrapper);
		stack[stackCount-3] = FP_OFF(wrapper);
#endif
	} else {

#ifndef BCC_BLOCK_IGNORE
		stack[stackCount-2] = FP_SEG(idle_fun);
		stack[stackCount-3] = FP_OFF(idle_fun);
#endif

	}
#ifndef BCC_BLOCK_IGNORE
	sp = FP_OFF(stack+stackCount-12);
	ss = FP_SEG(stack+stackCount-12);
#endif
	bp = sp;
	state_pcb = s;
	sem_unblock_flag = 0;
	parent = PCB::running->idPCB;

	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	if(s==IDLE)
	{
	idPCB=0;
	}
	else{
	idPCB=++staticID;
	for (int i=0;i<16;i++) {pcbSignals[i].handlers.copy( PCB::running->pcbSignals[i].handlers);}
	}
	all_PCB.addEnd(this);
	unlockHard;
	#endif


	if (timeSlice == 0)
		this->timeSlice = -1;
	else
		this->timeSlice = timeSlice;

	this->myThread = myThread;

}

void PCB::wrapper() {
	//syncPrintf("pozvan wraper \n");

	PCB::running->myThread->run();
	//syncPrintf("zavrsena run metoda\n");

	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	PCB::running->unblockWaiting();
	//cout<<"odblokirani"<<endl;

	Thread *parentThr = Thread::getThreadById(running->parent);

	if (parentThr!=nullptr && parentThr->myPCB!=nullptr)
		{

			parentThr->myPCB->signal(1);
		}
	PCB::running->signal(2);
	//pozvoi za dvojku
	PCB::running->callSignals();


	running->state_pcb=FINISHED;

	dispatch();
	unlockHard;
	#endif


}

ID PCB::getRunningId() {
	return running->idPCB;
}

Thread* PCB::getThreadById(ID id) {
	Thread* thread = nullptr;
#ifndef BCC_BLOCK_IGNORE
	lockHard;
	for(all_PCB.onFirst();all_PCB.currentExists();all_PCB.onNext())
	{

		PCB* temp=all_PCB.getCurrentNode();
		if(temp->idPCB==id)
		{
			thread=(temp->myThread);
			break;
		}
	}
	unlockHard;
#endif
	return thread;
}

void PCB::waitToComplete() {
	//syncPrintf("pozvan wait to complete\n");
#ifndef BCC_BLOCK_IGNORE
	lockHard;

	if (PCB::running != this && state_pcb != NEW_THREAD && state_pcb != FINISHED && state_pcb != IDLE)
	{
#ifndef BCC_BLOCK_IGNORE
		//lockHard;
		PCB::running->state_pcb=BLOCKED;
		//unlockHard;
#endif
		wait_list_thread.addEnd((PCB*)PCB::running);		//cast running u pcb
		dispatch();
		//cout<<"gotov wait to complete"<<endl;
	}
	unlockHard;
#endif

}

void PCB::start() {

#ifndef BCC_BLOCK_IGNORE
	lockHard;
	if (state_pcb == NEW_THREAD ) {	//running.state??
		state_pcb = READY;
		Scheduler::put(this);
	}
	unlockHard;
#endif

}

PCB::~PCB() {
	//dal treba lock ovde
	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	//if(flag_delete_from_allpcb)
	for( all_PCB.onFirst();all_PCB.currentExists();all_PCB.onNext())
	{

		PCB* temp=all_PCB.getCurrentNode();
		if(temp->idPCB==this->idPCB)
		{
			all_PCB.deleteCurrNode();
			break;
		}
	}

	signalsCalled.delete_list();
	if(stack!=nullptr)
	{   //zbog kernela
		delete [] stack;
		stack=nullptr;
	}
	unlockHard;
#endif

}

void PCB::killThread(PCB* temp) {

#ifndef BCC_BLOCK_IGNORE
lockHard;
//cout<<"pozvan kill thread"<<endl;
	temp->unblockWaiting();
	temp->myThread->myPCB=nullptr;

	delete temp;
	unlockHard;
	#endif


}


void PCB::unblockWaiting() {


	#ifndef BCC_BLOCK_IGNORE
	lockHard;
//	if(wait_list_thread.list_is_empty())
//	{
//		cout<<"niko ne ceka"<<endl;
//	}
	for( wait_list_thread.onFirst();wait_list_thread.currentExists();wait_list_thread.onNext())
	{
		PCB* temp= wait_list_thread.getCurrentNode();
		//cout<<temp->getID()<<endl;
		temp->state_pcb=READY;
		Scheduler::put(temp);
		wait_list_thread.deleteCurrNode();
	}
	unlockHard;
	#endif



}


int PCB::callSignals(){


	PCB *tempRunning = (PCB*)running;
	if(tempRunning->state_pcb==IDLE) return 1;
	//cout<<"usao u call singals";
	for(tempRunning->signalsCalled.onFirst();tempRunning->signalsCalled.currentExists();tempRunning->signalsCalled.onNext())
	{
		SignalId temp=tempRunning->signalsCalled.getCurrentNode();
		//cout<<tempRunning->signalsCalled;
		//cout<<"signal id je :"<<temp<<endl;
		if(tempRunning->pcbSignals[temp].blockflag!=1 && staticBlockedFlags[temp]!=1)
		{

			if(temp==0)
		{//cout<<"vratio nulu brise se "<<endl;
		return 0;}
			//if(temp==2) cout<<"dvojkica prosla"<<endl;
			if(tempRunning->specialSignalsFlag!=1 && temp==2) continue;
			if(tempRunning->specialSignalsFlag!=1 && temp==1) continue;

			for(tempRunning->pcbSignals[temp].handlers.onFirst();tempRunning->pcbSignals[temp].handlers.currentExists();tempRunning->pcbSignals[temp].handlers.onNext())
			{
				SignalHandler call=tempRunning->pcbSignals[temp].handlers.getCurrentNode();
				//cout<<"pozvao sam nesto"<<endl;
				(*call)();
				//zove

			}
			tempRunning->signalsCalled.deleteCurrNode();
		}

	}
	return 1;

}

void PCB::signal(SignalId signal) {
	if (signal < 16)
	{
	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	signalsCalled.addEnd(signal);
	unlockHard;
	#endif
	}
	//nesto
}

void PCB::registerHandler(SignalId signal, SignalHandler handler) {
	//lock
	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	if (signal < 16)
		pcbSignals[signal].handlers.addEnd(handler);
	unlockHard;
	#endif
}

void PCB::unregisterAllHandlers(SignalId id) {
	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	if (id < 16)
		pcbSignals[id].handlers.delete_list();
	unlockHard;
	#endif
}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {

	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	SignalHandler* pointer=0;
	int flag=0;


	if (id < 16) {
		for (pcbSignals[id].handlers.onFirst();
						pcbSignals[id].handlers.currentExists();
						pcbSignals[id].handlers.onNext()) {
			if(pcbSignals[id].handlers.getCurrentNode()==hand1)
			{pointer=&pcbSignals[id].handlers.current->data;flag=1; break;}

		}

		for (pcbSignals[id].handlers.onFirst();
							pcbSignals[id].handlers.currentExists();
							pcbSignals[id].handlers.onNext()) {
				if(pcbSignals[id].handlers.getCurrentNode()==hand2 && flag==1)
				{
					*pointer=pcbSignals[id].handlers.getCurrentNode();
					pcbSignals[id].handlers.getCurrentNode()=hand1;
					break;

				}

			}
	}
	unlockHard;
	#endif

}

void PCB::blockSignal(SignalId signal) {
	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	pcbSignals[signal].blockflag = 1;
	unlockHard;
	#endif
}

void PCB::blockSignalGlobally(SignalId signal) {
	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	staticBlockedFlags[signal] = 1;
	unlockHard;
	#endif
}

void PCB::unblockSignal(SignalId signal) {
	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	pcbSignals[signal].blockflag = 0;
	unlockHard;
	#endif
}

void PCB::unblockSignalGlobally(SignalId signal) {
	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	staticBlockedFlags[signal] = 0;
	unlockHard;
	#endif
}


