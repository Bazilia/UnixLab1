
#ifndef labHeader
#define labHeader
#include <signal.h>
siginfo_t info;
int gotSignal;

void doStd();
void doChild();
void doPosix(int count);
void doKill(int sig, int pid);
void doPipe();

#endif
