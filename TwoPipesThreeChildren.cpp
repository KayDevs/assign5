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
#include <sys/wait.h>
int main(int argc, char **argv){
	//Added (char*) cast bc "Deprecated conversion from string constant to char*", bc strings are write-only
	//Making it a char *const made execvp no longer work, so casting will have to do for now.
	//(we're not writing to these strings so it -should- be okay?)
	char *ls_args[] = {(char*)"ls", (char*)"-ltr", NULL};
	char *grep_args[] = {(char*)"grep", (char*)"3376", NULL};
	char *grep2_args[] = {(char*)"grep", (char*)"hi", NULL};
	char *wc_args[] = {(char*)"wc", (char*)"-l", NULL};
	char **cmds[] = {ls_args, grep_args, grep2_args, wc_args, NULL};	
	
	int cur_proc = 0;
	int pipes[2];
	int childpid;
	int cur_fd = 0;
	while(cmds[cur_proc] != NULL) {
		pipe(pipes);
		if((childpid = fork()) == -1) {
			perror("Could not create child process.");
			exit(1);
		}
		if(childpid == 0) {
			dup2(cur_fd, 0);
			if(cmds[cur_proc + 1] != NULL) {
				dup2(pipes[1], 1);
			}
			close(pipes[0]);
			execvp(cmds[cur_proc][0], cmds[cur_proc]);
			exit(0);
		} else {
			wait(NULL);
			close(pipes[1]);
			cur_fd = pipes[0];
			cur_proc++;
		}
	}

	return(0);

}
