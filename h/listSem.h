/*
 * listSem.h
 *
 *  Created on: May 5, 2020
 *      Author: OS1
 */

#ifndef H_LISTSEM_H_
#define H_LISTSEM_H_
#include "pcb.h"
#include "thread.h"
#include "usable.h"
#include "Lista.h"
struct TimePCB {
	//public :
		PCB* pcb;
		Time timePCB;
		TimePCB(PCB *pcb, Time time) : pcb(pcb), timePCB(time) {}
		friend ostream& operator<<(ostream& os, TimePCB* l1)
			{
				os<<"ID:"<<l1->pcb->getID()<<"timePCB::"<<l1->timePCB<<endl;
				return os;
			}
//		~TimePCB(){
//			pcb=0;
//
//		}
};


class listSem{
public:
//	struct TimePCB {
//			PCB* pcb;
//			Time timePCB;
//			TimePCB(PCB *pcb, Time time) : pcb(pcb), timePCB(time) {}
//		};

	listSem() {}//currentTime(0)
	void add(PCB *pcb, Time time);
	TimePCB* take();
	int reduceTime();

	int list_is_empty();
	int list_size();
	//fali uzimanje;
	~listSem();


		List<TimePCB*> listElem;

};



#endif /* H_LISTSEM_H_ */
