#include "timer.h"
#include "kersem.h"

//typedef void interrupt (*pInterrupt) (...);



volatile int explicitCall = 0;
unsigned tsp;
unsigned tss;
unsigned tbp;
unsigned oldTimerOFF, oldTimerSEG;
volatile int timeLeftTimer;


void tick();

PCB* killTarget;
void interrupt timer(...)
{


//bio je ovaj uslov dole i tick ej bio u !explicitCall&&timeLeftTimer>0
	if(!explicitCall)
				{

					#ifndef BCC_BLOCK_IGNORE

					asm {
					int 60h; //za staru
					}
					#endif
					tick();
					semTimeDecrease();
				}

	if (!explicitCall&&timeLeftTimer>0)
	{
		timeLeftTimer--;


	}

	if (timeLeftTimer==0  || explicitCall)
			{

					#ifndef BCC_BLOCK_IGNORE
					asm {
						// cuva sp
						mov tsp, sp
						mov tss, ss
						mov tbp, bp
					}
					#endif


					PCB::running->sp = tsp;
					PCB::running->ss = tss;
					PCB::running->bp = tbp;

					if(PCB::running->state_pcb==RUNNING )
						//proveri jos uslova && PCB::running->getID()!=1 && PCB::running->state_pcb!=IDLE
						{
							PCB::running->state_pcb=READY;
							//mozda ne mora lock
							Scheduler::put((PCB*)PCB::running);
						}
					// scheduler
					int flag=0;
					killTarget=0;
					while(1)
					{
						PCB::running = Scheduler::get();
						if(PCB::running==nullptr) {PCB::running=getIdlePCB();}
						else//zasad
						PCB::running->state_pcb=RUNNING;

						//if(Thread::getRunningId()==2) cout<<endl<<"MAIN NIT"<<endl;
						tsp = PCB::running->sp;
						tss = PCB::running->ss;
						tbp = PCB::running->bp;
						timeLeftTimer = PCB::running->timeSlice;

						#ifndef BCC_BLOCK_IGNORE
						asm {
							// restaurira sp
							mov sp, tsp
							mov ss, tss
							mov bp, tbp
						}

						#endif

					#ifndef BCC_BLOCK_IGNORE
						lockHard;//da izoluje slucaj kad ga prekine neko dok brises vlajda



						if (killTarget != 0)
						{
							//cout<<"ubijeno nesto"<<endl;
							  PCB::killThread(killTarget);


							 killTarget = 0;

						 }
						else if (PCB::running->state_pcb==IDLE) break;

						if (!PCB::callSignals())
						{
							killTarget = (PCB*)PCB::running;
							//cout<<endl<<Thread::getRunningId()<<endl;
						}

						if(killTarget==0) {break;}
						unlockHard;
						//cout<<"while doso do kraja"<<endl;
						#endif

					}
						//cout<<(PCB*)PCB::running->myThread->getId();
					//cout<<"izasao iz while"<<endl;
					//cout<<endl<<Thread::getRunningId()<<endl;
				}


			explicitCall=0;


}



void inic() {

	syncPrintf("pozvan inic\n");
		#ifndef BCC_BLOCK_IGNORE
		asm{
			cli
			push es
			push ax

			mov ax,0
			mov es,ax // es = 0

			// pamti staru rutinu
			mov ax, word ptr es:0022h
			mov word ptr oldTimerSEG, ax
			mov ax, word ptr es:0020h
			mov word ptr oldTimerOFF, ax

			// postavlja novu rutinu
			mov word ptr es:0022h, seg timer
			mov word ptr es:0020h, offset timer

			// postavlja staru rutinu na int 60h
			mov ax, oldTimerSEG
			mov word ptr es:0182h, ax
			mov ax, oldTimerOFF
			mov word ptr es:0180h, ax

			pop ax
			pop es
			sti
		}
		#endif

}

void restore() {
	syncPrintf("pozvan restore\n");

	#ifndef BCC_BLOCK_IGNORE
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
	#endif
}

