
/*
 * listSem.cpp
 *
 *  Created on: May 5, 2020
 *      Author: OS1
 */

#include "listSem.h"
#include "SCHEDULE.h"
#include "PCB.h"

void listSem::add(PCB* pcb, Time time) {
	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	TimePCB* t1=new TimePCB(pcb,time);
	listElem.addEnd(t1);
	//cout<<"ubacio sam "<<t1<<endl;
	unlockHard;
	#endif
}

int listSem::reduceTime()
{
	int number_of_unblocked=0;

	//syncPrintf("poceo jedan otkucaj \n");
	for(listElem.onFirst();listElem.currentExists();listElem.onNext())
	{
			if(listElem.getCurrentNode()->timePCB>1)
				{listElem.getCurrentNode()->timePCB--;//lock
				//syncPrintf("smanjeno vreme reduce time \n", listElem.getCurrentNode());

				//cout<<"smanjeno vreme reduce time"<<listElem.getCurrentNode()<<endl;
				}
			else //if(listElem.getCurrentNode()->timePCB==0)
			{
				//lock oko svega
				listElem.getCurrentNode()->timePCB--;

				//syncPrintf("vreme je nula %d \n",listElem.getCurrentNode()->pcb->getID());
				listElem.getCurrentNode()->pcb->state_pcb=READY;
				listElem.getCurrentNode()->pcb->sem_unblock_flag=1;
				//cout<<listElem;
				//ispis();
				//listElem.getCurrentNode()->timePCB=-9999;
				//cout<<listElem.getCurrentNode();
				//flaguj ga da je skinut;
				Scheduler::put((PCB*)listElem.getCurrentNode()->pcb);
				listElem.deleteCurrNode();
	//				cout<<endl<<"posle"<<endl;
	//				cout<<listElem;


				number_of_unblocked++;

			}

	}


//	syncPrintf("u listi ima %d \n",listElem.list_size());
//	if(number_of_unblocked==0) syncPrintf("nisam nijednog oslobodio\n\n");
//	else syncPrintf("oslobodio %d \n\n", number_of_unblocked);
	return number_of_unblocked;

}


int listSem::list_is_empty() {
	return listElem.list_is_empty();
}

int listSem::list_size() {
	return listElem.list_size();
}

TimePCB* listSem::take() {
	if(!list_is_empty())
		{
		return listElem.onFirst().getCurrentNode();
		listElem.deleteCurrNode();
		}
}

listSem::~listSem() {
	//verovatno nije dobro :D
	listElem.delete_list();
}
