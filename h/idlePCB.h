#ifndef _idlePCB_h_
#define _idlePCB_h_
#include "Lista.h"

#include "usable.h"
#include "pcb.h"
#include "thread.h"


PCB* getIdlePCB();
class Idle:public PCB{
public:
Idle();
};


#endif
