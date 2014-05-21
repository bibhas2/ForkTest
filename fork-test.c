#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <stdlib.h>

#define NUM_CHILD 4

void
_info(const char* fmt, ...) {
	va_list ap;

	va_start(ap, fmt);

	printf("INFO: ");
	vprintf(fmt, ap);
	printf("\n");
}

int
main() {
	int i = 0, numChildren = 0;

	for (i = 0; i < NUM_CHILD; ++i) {
		int pid = fork();
		if (pid == 0) {
			//This block executes only for children
			_info("Child started working....");
			sleep(20);
			_info("Child finished working.");
			/*
			 * Child must call exit after finishing work. Else
			 * it will continue with the code execution below that
			 * is meant only for parents.
			 */
			exit(0);
		} else if (pid < 0) {
			_info("Failed to start child process.");
		} else {
			//This block executes only for parent
			_info("Child spawned successfully. PID: %d", pid);
			++numChildren;
		}
	}
	while (numChildren > 0) {
		int status;

		int pid = wait(&status);
		if (pid > 0) {
			_info("Child terminated. PID: %d", pid);
			--numChildren;
		}
	}
}
