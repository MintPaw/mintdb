#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

int main(int argc, char **argv)  {
	/* sigset_t mask; */
	/* sigemptyset(&mask); */
	/* sigaddset(&mask, SIGCHLD); */
	/* sigprocmask(SIG_BLOCK, &mask, NULL); */
	/* sigprocmask(SIG_UNBLOCK, &mask, NULL); */

	pid_t childPid = fork();

	if (childPid == 0) {
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		if (execvp(argv[1], &argv[1]) < 0) perror("execvp");
	}

	// listen for sigchld
	// attach
	int childStatus;
	int instruction = 0;
	wait(&childStatus);
	while (WIFSTOPPED(childStatus)) {
		instruction++;
		if (ptrace(PTRACE_SINGLESTEP, childPid, 0, 0) < 0) perror("ptrace");
		wait(&childStatus);
	}
	printf("(%d) %d instruct\n", childPid, instruction);

	return 0;
}
