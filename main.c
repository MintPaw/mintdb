#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/user.h>
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
	int iNum = 0;
	wait(&childStatus);
	while (WIFSTOPPED(childStatus)) {

		struct user_regs_struct regs;
		ptrace(PTRACE_GETREGS, childPid, 0, &regs);
		unsigned instr = ptrace(PTRACE_PEEKTEXT, childPid, regs.rip, 0);
		printf("icounter = %u.  EIP = 0x%08x.  instr = 0x%08x\n", iNum, regs.rip, instr);

		if (ptrace(PTRACE_SINGLESTEP, childPid, 0, 0) < 0) perror("ptrace");
		iNum++;
		printf("(%d) %d instruct\n", childPid, iNum);
		wait(&childStatus);
	}

	return 0;
}
