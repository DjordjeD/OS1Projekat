/*
 * kersem.h
 *
 *  Created on: May 5, 2020
 *      Author: OS1
 */

#ifndef H_KERSEM_H_
#define H_KERSEM_H_
#include "Lista.h"
#include "usable.h"
#include "pcb.h"
#include "listSem.h"

 void semTimeDecrease();

class KernelSem{
public:

	volatile int semVal;
	List<PCB*> infinetly_blocked_on_sem;
	listSem blocked_on_sem;
	static List<KernelSem*> semaphores;


	KernelSem(int init);

	int valKernel() const;
	int waitKernel(Time maxTimeToWait);
	int signalKernel(int n);
	void increaseVal(int inc);


	~KernelSem();
};




#endif /* H_KERSEM_H_ */
