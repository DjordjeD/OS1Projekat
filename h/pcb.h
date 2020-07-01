
#ifndef pcb_h_
#define pcb_h_
#include <dos.h>
#include <iostream.h>
#include "thread.h"
#include "SCHEDULE.h"
#include "usable.h"
#include "Lista.h"

void ispis();

static unsigned staticBlockedFlags[16];


struct SignalStruct {
	int blockflag;
	List<SignalHandler> handlers;
};



class PCB{
public:
	volatile static PCB* running; //nullptr

	unsigned* stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned stack_size;
	State state_pcb;

	int timeSlice;
	int flag_delete_from_allpcb;
	int sem_unblock_flag;
	List<PCB*> wait_list_thread;
	Thread* myThread;

	int specialSignalsFlag;
	ID parent;
	SignalStruct pcbSignals[16];
	List<SignalId> signalsCalled;



	PCB();
	PCB(StackSize stackSize,Time timeSlice, Thread *myThread,State s=NEW_THREAD);


	static ID getRunningId();
	static Thread* getThreadById (ID id);
	static void killThread(PCB* temp);
	void waitToComplete(); // join
	void start(); // pokrece nit
	ID getID(){return idPCB;}

	~PCB(); //waittocomplete ne zaboarvi
	static void wrapper();

	void unblockWaiting();

	static int callSignals();
	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);


	friend ostream& operator<<(ostream& os, PCB* l1)
	{
		if(!(l1->wait_list_thread.list_is_empty()))
		os<<"ID:"<<l1->idPCB<<" TimeSlice:"<<l1->timeSlice<<endl<<"LISTA CEKANJA:"<<endl<<l1->wait_list_thread<<endl;
		else
		os<<"ID:"<<l1->idPCB<<l1->sem_unblock_flag<<endl;
		return os;
	}

private:
	ID idPCB;
	volatile static ID staticID;


};
#endif
