// file: onepipe.cpp
// author: M. Amine Belkoura
// date: 03/04/2015
// purpose: CS3376
// description:
// this program executes "ls -ltr | grep 3376", by dividing the two command
// among the child and parent process
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int count_spaces(const char *str) {
	int count = 0;
	for(; *str; ++str) {
		if(*str == ' ') {
			count++;
		}
	}
	return count;
}

int main(int argc, char **argv){
	if(argc > 6) {
		perror("Too many arguments!");
		exit(1);
	}
	if(argc < 3) {
		perror("Too few arguments!");
		exit(1);
	}

	char ***cmds = new char**[argc]; //1 extra at end for NULL
	for(int i = 0; i < argc-1; ++i) {
		printf("Parsing.. %s\n", argv[i+1]);
		cmds[i] = new char*[count_spaces(argv[i+1])+1];
		int j = 0;
		cmds[i][j] = strtok(argv[i+1], " ");
		printf("current str: %s\n", cmds[i][j]);
		while(cmds[i][j] != NULL) {
			j++;
			cmds[i][j] = strtok(NULL, " ");
			printf("current str: %s\n", cmds[i][j]);
		}
		//should be null every time
		printf("last str: %s\n", cmds[i][j]);
	}
	cmds[argc] = NULL;

	for(int i = 0; i < argc-1; ++i) {
		printf("command entered: %s\n", cmds[i][0]);
	}

	
	int cur_proc = 0;
	int pipes[2];
	int childpid;
	int cur_fd = 0;
	while(cmds[cur_proc] != NULL) {
		printf("executing process...%d\n", cur_proc);
		printf("current command: %s\n", cmds[cur_proc][0]);
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
