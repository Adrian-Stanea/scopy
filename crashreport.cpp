#include "crashreport.h"
#include <QDebug>
#include <QStandardPaths>
//#include <sigaction.h>
#include <unistd.h>
#include <sys/wait.h>
#ifndef _WIN32
#include <execinfo.h>
#endif

#define STACKTRACE_DEPTH 50

static int fd[2];


struct crash_report {
	pid_t pid;
	int signum;
	int err_no;
	time_t time;
};

void handleStacktrace()
{
	int nbytes;
	char readbuffer[8192];
	int total_bytes = 0;

	/* Get everything the parent process sends */
	close(fd[1]);

	memset(readbuffer, 0, sizeof(readbuffer));
	while (1) {
		nbytes = read(fd[0], readbuffer + total_bytes,
				sizeof(readbuffer) - total_bytes);

		if (nbytes < 0) {
			fprintf(stderr, "read error\n");
			break;
		}

		if (nbytes == 0) {
			break;
		}

		total_bytes += nbytes;
	}
	/* Osc ran normally and no bytes were sent by the parent proc */
	if (total_bytes == 0)
		return;

	/* First received data contains a crash_report struct */
	char *creport_buf;
	struct crash_report *creport;
	FILE *fp;
	char *creport_filename;

	creport_buf = (char*)malloc(sizeof(struct crash_report));
	if (!creport_buf) {
		fprintf(stderr, "Memory allocation failed. File: %s "
			"Func: %s Line: %d\n",
			__FILE__, __func__, __LINE__);
		return;
	}
	creport = (struct crash_report *)creport_buf;
	memcpy(creport_buf, readbuffer, sizeof(struct crash_report));

	/* Store the crash report in a file */
	// (char*)QString(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
	creport_filename = ".scopy_crash_report";
	fp = fopen(creport_filename, "w+");
	if (!fp) {
		fprintf(stderr, "Failed to open file %s for writing. "
			"File: %s Func: %s Line: %d\n",
			creport_filename,
			__FILE__, __func__, __LINE__);

		free(creport_buf);
		free(creport_filename);
		return;
	}
	fprintf(fp, "\n"
		"Scopy Crash Info\n"
		"\n"
		"PID: %d\n"
		"Signal No: %d\n"
		"Signal String: %s\n"
		"Error No: %d\n"
		"Error String: %s\n"
		"Time Stamp: %s",
		creport->pid,
		creport->signum,
		strsignal(creport->signum),
		creport->err_no,
		strerror(creport->err_no),
		ctime(&creport->time)
	);
	fprintf(fp, "\nScopy Backtrace\n");
	fprintf(fp, "%s", readbuffer + sizeof(struct crash_report));
	fprintf(stderr, "A crash report file has been created at: %s\n",
			creport_filename);

	/* Free resources */
	fclose(fp);
	free(creport_buf);
//	free(creport_filename);
}

void stacktraceCallback(int signum, siginfo_t *siginfo, void *uctx)
{
	void *backtrace_buffer[STACKTRACE_DEPTH];
	size_t size;
	struct crash_report creport;
	struct timespec timestamp;
	ssize_t ret;

	/* Get a minimal status of the program */
	clock_gettime(CLOCK_REALTIME, &timestamp);
	creport.pid = getpid();
	creport.signum = signum;
	creport.err_no = errno; //tbd handle this
	creport.time = timestamp.tv_sec;

	/* Get the backtrace */
	size = backtrace(backtrace_buffer, sizeof(backtrace_buffer));

	/* Send everything to the child process */
	close(fd[0]);
	ret = write(fd[1], (const void *)&creport,
			sizeof(struct crash_report));
	if (ret != sizeof(struct crash_report)) {
		goto end;
	}
	backtrace_symbols_fd(backtrace_buffer, size, fd[1]);
end:
	close(fd[1]);
	/* Wait child process to finish the crash report */
	wait(NULL);
	abort();
}

void initSignalHandler(char *progName)
{
#ifndef _WIN32
	/* Struct describes the action to be taken when the signal arrives */
	struct sigaction signalHandler;
	int ret;

	/* Prepare a child process to receive crash data the handler sends */
	pid_t stackParserPid;

	ret = pipe(fd);
	if (ret != 0) {
		qInfo() << "Failed to create pipe. Error: "
			<< strerror(errno)
			<< " File: " << __FILE__
			<< " Func: " << __func__
			<< " Line: " << __LINE__;
		return;
	}
	stackParserPid = fork();

	if (stackParserPid == 0) {
		handleStacktrace();
//		::raise(SIGTERM);
		// send back to main to print scopy finished unsuccessfully
		_exit(EXIT_SUCCESS);
	}

	/* Send a crash report when a crash occurs */
	memset(&signalHandler, 0, sizeof(signalHandler));
	signalHandler.sa_sigaction = stacktraceCallback;

	// Which signals should we track?
	sigaction(SIGILL, &signalHandler, NULL);
	sigaction(SIGSEGV, &signalHandler, NULL);
	sigaction(SIGABRT, &signalHandler, NULL);
	sigaction(SIGFPE, &signalHandler, NULL);
#endif
}
