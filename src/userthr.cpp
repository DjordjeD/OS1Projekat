#include "thread.h"
#include "userthr.h"
#include "usable.h"

int userMain(int argc, char* argv[]);

UserThread::UserThread(int argc1, char** argv1):Thread() {
	argc=argc1;
	argv=argv1;
	retCode=0;
}

int UserThread::returnCode() {
	return retCode;
}

UserThread::~UserThread() {
	//syncPrintf("destruktor userthread");
	waitToComplete();
}

void UserThread::run() {
	retCode = userMain(argc,argv);
}
