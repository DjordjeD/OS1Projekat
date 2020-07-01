#ifndef USERTHR_H
#define USERTHR_H
#include "Thread.h"
#include "Lista.h"

class UserThread : public Thread {
	int argc;
	char **argv;
	int retCode;

public:
	UserThread(int argc1, char** argv1);
	int returnCode();
	~UserThread();
protected:
	void run();
};
#endif
