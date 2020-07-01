/*
 * usable.h
 *
 *  Created on: Apr 28, 2020
 *      Author: OS1
 */

#ifndef USABLE_H_
#define USABLE_H_

//#include "Lista.h"
#include <iostream.h>
//defining all usefull stuff
#define nullptr 0
#define true 1
#define false 0
#define lockHard asm {pushf;cli;}
#define unlockHard asm popf;

#define proveri syncPrintf("provera\n")
#define prov_space do { lockHard; cout<<endl<<"Pozvano iz:"<<(__FILE__)<<endl; unlockHard;} while(0);


void dispatch();
enum State {RUNNING,BLOCKED,NEW_THREAD,IDLE,READY,FINISHED};


int syncPrintf(const char *format, ...);


#endif /* USABLE_H_ */
