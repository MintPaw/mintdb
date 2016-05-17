#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ptrace.h>

int main(int argc, char **argv)  {
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &mask, NULL);

	pid_t pid = fork();

	if (pid != 0) {
		// listen for sigchld
		sigprocmask(SIG_UNBLOCK, &mask, NULL);
		// attach
	} else {
		sigprocmask(SIG_UNBLOCK, &mask, NULL);
		execvp(argv[1], &argv[1]);
	}
	return 0;
}
