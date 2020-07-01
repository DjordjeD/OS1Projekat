/*
 * prepEnt.h
 *
 *  Created on: May 11, 2020
 *      Author: OS1
 */

#ifndef H_PREPENT_H_
#define H_PREPENT_H_

//mzoda mora da se ukljuci ovde kerevent.h

#define PREPAREENTRY(number, callOldRoutine)\
	void interrupt newRoutine##number(...);\
	IVTEntry ivtEntry##number(number,&newRoutine##number);\
	void interrupt newRoutine##number(...){\
		IVTEntry::IVTable[number]->myKernelEvent->signal();\
		if (callOldRoutine == 1)IVTEntry::IVTable[number]->myOldInterruptRoutine();\
}\

//

//if(IVTEntry::IVTable[number]->myKernelEvent)

#endif /* H_PREPENT_H_ */
