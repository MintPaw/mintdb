#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

void setInstructionBreakpoint(unsigned long long instrNum);
void sigchldHandler(int sig);

int main(int argc, char **argv)  {

	pid_t childPid = fork();

	if (childPid == 0) {
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		if (execvp(argv[1], &argv[1]) < 0) perror("execvp");
	}

	int childStatus;
	wait(&childStatus);

	int instrNum = 0;
	while (WIFSTOPPED(childStatus)) {
		struct user_regs_struct regs;
		ptrace(PTRACE_GETREGS, childPid, 0, &regs);
		unsigned instr = ptrace(PTRACE_PEEKTEXT, childPid, regs.rip, 0);

		printf("instrNum = %u.  RIP = 0x%08llx.  instr = 0x%08x\n", instrNum, regs.rip, instr);
		/* printf("Stopped because %s\n", strsignal(WSTOPSIG(childStatus))); */

		if (ptrace(PTRACE_SINGLESTEP, childPid, 0, 0) < 0) perror("ptrace");
		instrNum++;
		wait(&childStatus);
	}

	return 0;
}

void setInstructionBreakpoint(unsigned long long instrNum) {

}

void sigchldHandler(int sig) {
}
