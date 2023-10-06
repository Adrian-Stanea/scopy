#ifndef CRASHREPORT_H
#define CRASHREPORT_H
#include <signal.h>

void initSignalHandler(char *progName);
void stacktraceCallback(int signum, siginfo_t *siginfo, void *uctx);
void handleStacktrace();


#endif // CRASHREPORT_H
