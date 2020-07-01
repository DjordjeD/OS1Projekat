#include "idlePCB.h"

//friend class PCB;

Idle::Idle():PCB(maxStack,1,nullptr,IDLE){};//mozda i maxStack
// fali destruktor vidi za statik klase
// mozda i ne, vidi dal se unistava izvedena lol

PCB* getIdlePCB()

{
	static Idle idle_thr=Idle();
	return &idle_thr;
}
