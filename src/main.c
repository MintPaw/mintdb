#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#define TRAP_CODE 0xCC

struct Breakpoint {
	unsigned long long instrAddr;
	long origInstr;
};

pid_t childPid;
struct Breakpoint **breakpoints;
int breakpointNum = 0;

void setInstructionBreakpoint(unsigned long long instrNum);

int main(int argc, char **argv)  {
	breakpoints = (struct Breakpoint **)malloc(sizeof(struct Breakpoint *) * 100);
	childPid = fork();

	if (childPid == 0) {
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		if (execvp(argv[1], &argv[1]) < 0) perror("execvp");
	}

	int childStatus;
	wait(&childStatus);

	setInstructionBreakpoint((unsigned long long)0x00000000004000da);

	char firstTimeContinue = 1;
	int instrNum = 0;
	while (WIFSTOPPED(childStatus)) {
		if (firstTimeContinue) {
			firstTimeContinue = 0;
			if (ptrace(PTRACE_CONT, childPid, 0, 0) < 0) perror("ptrace");
			wait(&childStatus);
			continue;
		}

		struct user_regs_struct regs;
		ptrace(PTRACE_GETREGS, childPid, 0, &regs);
		long instr = ptrace(PTRACE_PEEKTEXT, childPid, regs.rip, 0);

		printf("Stopped. instrNum = %u.  RIP = 0x%llx.  instr = 0x%lx\n", instrNum, regs.rip, instr);

		int i;
		for (i = 0; i < breakpointNum; i++) {
			if (breakpoints[i]->instrAddr == regs.rip-1) {
				printf("Stopped for breakpoint, fixing...\n");
				ptrace(PTRACE_POKETEXT, childPid, breakpoints[i]->instrAddr, breakpoints[i]->origInstr);
				regs.rip--;
				ptrace(PTRACE_SETREGS, childPid, 0, &regs);
			}
		}

		if (ptrace(PTRACE_CONT, childPid, 0, 0) < 0) perror("ptrace");
		instrNum++;
		wait(&childStatus);
	}

	return 0;
}

void setInstructionBreakpoint(unsigned long long instrAddr) {
	long instr = ptrace(PTRACE_PEEKTEXT, childPid, (void*)instrAddr, 0);
	printf("Adding breakpoint at instruction 0x%08llx\n", instrAddr);

	struct Breakpoint *b = (struct Breakpoint *)malloc(sizeof(struct Breakpoint));
	b->instrAddr = instrAddr;
	b->origInstr = instr;
	breakpoints[breakpointNum++] = b;

	long trapInstr = (instr & 0xFFFFFFFFFFFFFF00) | 0xCC;
	ptrace(PTRACE_POKETEXT, childPid, (void*)instrAddr, trapInstr);
}
