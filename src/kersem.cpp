#include "kersem.h"

List<KernelSem*> KernelSem::semaphores;

int KernelSem::waitKernel(Time maxTimeToWait) {
    int ret = 1;


	//lock oko svega

	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	if (--semVal < 0)
	{
		PCB *temp = (PCB*)PCB::running;

		#ifndef BCC_BLOCK_IGNORE
		lockHard;
		temp->state_pcb=BLOCKED;
		unlockHard;
		#endif

		//block temp

		if (maxTimeToWait==0)
			{infinetly_blocked_on_sem.addEnd(temp);}// cout<<"ubacen u obcinu listu";}
		else
			blocked_on_sem.add(temp, maxTimeToWait);

		dispatch();

	// razresi flag u waitu
		if (temp->sem_unblock_flag==1) {
			temp->sem_unblock_flag=0;
			ret = 0;
		}


	}
	unlockHard;
	#endif



 return ret;
}

int KernelSem::signalKernel(int n) {

	if (n<0) return n;
	int ret = 0;

	//lock

	int toUnblock;
	if(n==0) toUnblock=1;
	else toUnblock=n;

	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	this->semVal+=toUnblock;
	//cout<<"pozvansignal"<<endl;
	if(!infinetly_blocked_on_sem.list_is_empty() && toUnblock>0)
	{
		for(infinetly_blocked_on_sem.onFirst();infinetly_blocked_on_sem.currentExists();infinetly_blocked_on_sem.onNext())
		{
			infinetly_blocked_on_sem.getCurrentNode()->state_pcb=READY;
			Scheduler::put((PCB*)infinetly_blocked_on_sem.getCurrentNode());
			infinetly_blocked_on_sem.deleteCurrNode();
			//mozda treba da se razveze
			toUnblock--;
			ret++;
			if(toUnblock==0) break;
		}
	}
	if(toUnblock>0 && !blocked_on_sem.list_is_empty() )
	{

		for(blocked_on_sem.listElem.onFirst();blocked_on_sem.listElem.currentExists();blocked_on_sem.listElem.onNext())
		{
			blocked_on_sem.listElem.getCurrentNode()->pcb->state_pcb=READY;
						Scheduler::put((PCB*)blocked_on_sem.listElem.getCurrentNode()->pcb);
						toUnblock--;
						blocked_on_sem.listElem.deleteCurrNode();
						//cout<<"signal obrisao"<<blocked_on_sem.listElem.getCurrentNode()<<endl;
						//mozda treba da se razveze
						ret++;
						if(toUnblock==0) break;
		}

	}
	if (n==0) ret=0;
	unlockHard;
	#endif

	//unlock
	return ret;

}

int KernelSem::valKernel() const {
	return semVal;
}

void KernelSem::increaseVal(int inc) {
	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	this->semVal=this->semVal+inc;
	unlockHard;
	#endif
}

void semTimeDecrease() {
	//lock treba obavezno

	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	for(KernelSem::semaphores.onFirst();KernelSem::semaphores.currentExists();KernelSem::semaphores.onNext())
	{	//lock

		int ret = KernelSem::semaphores.getCurrentNode()->blocked_on_sem.reduceTime();

		if(ret!=0)
			{
				KernelSem::semaphores.getCurrentNode()->semVal+=ret;
				//syncPrintf("dodat value, trenutna vrednost sem je:  %d \n",KernelSem::semaphores.getCurrentNode()->semVal);

			}
	}
	unlockHard;
	#endif


}

KernelSem::KernelSem(int init) {
	 if (init<0) init=0;
	    semVal = init;
	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	//lock
	 semaphores.addEnd(this);
	unlockHard;
	#endif

}

KernelSem::~KernelSem() {

	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	for(KernelSem::semaphores.onFirst();KernelSem::semaphores.currentExists();KernelSem::semaphores.onNext())
	{
		if(KernelSem::semaphores.getCurrentNode()==this)
		{
			KernelSem::semaphores.deleteCurrNode();
		}
	}
	blocked_on_sem.~listSem();
	infinetly_blocked_on_sem.delete_list();
	unlockHard;
	#endif

}
/*
 * kersem.cpp
 *
 *  Created on: May 5, 2020
 *      Author: OS1
 */




