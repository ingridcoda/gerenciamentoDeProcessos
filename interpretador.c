#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char * argv[]){
	int status, prioridade;
	pid_t pid;
	char * programa;
	FILE *arq = fopen("exec.txt", "r");

	while(fscanf(arq, "exec %s prioridade=%d", programa, &prioridade) != EOF){
		sleep(1);
		pid = fork();
		argv[0] = programa;
		itoa(prioridade, argv[1], 10);
		argc = 2;
		if(pid == 0) {
			execv("./escalonador", argv);
			perror("execv\n");
			exit(EXIT_FAILURE);
		} else if(pid < 0) {
			perror("fork\n");
			exit(EXIT_FAILURE);
		} else {
			wait(&status);			
			if(WIFEXITED(status))
				printf("Escalonador executado com sucesso!\n");
			else
				printf("Escalonador não executado!\n");
		}
	}
	return 0;
}