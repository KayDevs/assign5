// file: onepipe.cpp
// author: M. Amine Belkoura
// date: 03/04/2015
// purpose: CS3376
// description:
// this program executes "ls -ltr | grep 3376", by dividing the two command
// among the child and parent process
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(int argc, char **argv){
	int status;
	int childpid;
	//Added (char*) cast bc "Deprecated conversion from string constant to char*", bc strings are write-only
	//Making it a char *const made execvp no longer work, so casting will have to do for now.
	//(we're not writing to these strings so it -should- be okay?)
	char *cat_args[] = {(char*)"ls", (char*)"-ltr", NULL};
	char *grep_args[] = {(char*)"grep", (char*)"3376", NULL};
	char *wc_args[] = {(char*)"wc", (char*)"-l", NULL};
	// create one pipe to send the output of "ls" process to "grep" process
	// and another to send the output of "grep" to "wc"
	int pipes[2];
	int secondpipes[2];
	pipe(pipes);
	pipe(secondpipes);
	// fork the first child (to execute cat)
	if((childpid = fork()) == -1){
		perror("Error creating a child process");
		exit(1);
	}
	if (childpid == 0) {
		// replace cat's stdout with write part of 1st pipe
		dup2(pipes[1], 1);
		// close all pipes (very important!); end we're using was safely copied
		close(pipes[0]);
		close(pipes[1]);
		execvp(*cat_args, cat_args);
		exit(0);
	}
	else {
		if((childpid = fork()) == -1) {
			perror("Error creating a child process");
			exit(1);
		}
		if(childpid == 0) {
			// replace grep's stdin with read end of 1st pipe
			dup2(pipes[0], 0);
			close(pipes[0]);
			close(pipes[1]);
			// replace grep's stdout with write of 2nd pipe
			dup2(secondpipes[1], 1);
			close(secondpipes[0]);
			close(secondpipes[1]);
			execvp(*grep_args, grep_args);
			exit(0);
		} else {
			//these are still open so close them
			close(pipes[0]);
			close(pipes[1]);
			printf("Third process.\n");
			//route read end of pipe to stdin
			dup2(secondpipes[0], 0);
			close(secondpipes[0]);
			close(secondpipes[1]);
			execvp(*wc_args, wc_args);
		}
	}
	return(0);

}
