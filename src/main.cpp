#include <iostream.h>
#include "pcb.h"
#include "idlePCB.h"
#include "Thread.h"
#include "userthr.h"
#include "usable.h"
#include "timer.h"
#include "idlePCB.h"
#include "kersem.h"
#include "listSem.h"




int main(int argc, char* argv[]) {
	//proveri;
    PCB main_pcb= PCB();
	PCB::running=&main_pcb;

	//proveri;

	inic();

	UserThread* user_thr=new UserThread(argc, argv); //zasto je proradilo kad sam stavio new
	user_thr->start();
	user_thr->waitToComplete();
	int ret = user_thr->returnCode();


	#ifndef BCC_BLOCK_IGNORE
	lockHard;
	cout << "MAIN ZAVRSIO: " << ret << endl;
	unlockHard;
	#endif


   restore();
   return 0;
}
