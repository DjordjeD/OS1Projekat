#ifndef timer_h_
#define timer_h_

#include "usable.h"
#include "pcb.h"
#include "usable.h"
#include <dos.h>
#include "idlePCB.h"


extern volatile int explicitCall;

void interrupt timer(...);
void inic();
void restore();
#endif
