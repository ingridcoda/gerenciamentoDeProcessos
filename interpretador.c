#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char * argv[]){
	int status, *prioridade;
	int segmento1 = shmget(10,  3 * sizeof(char *), IPC_CREAT | S_IRUSR | S_IWUSR);
	int segmento2 = shmget(20,  100 * sizeof(char), IPC_CREAT | S_IRUSR | S_IWUSR);
	int segmento3 = shmget(30,  100 * sizeof(char), IPC_CREAT | S_IRUSR | S_IWUSR);
	int segmento4 = shmget(40,  sizeof(char), IPC_CREAT | S_IRUSR | S_IWUSR);
	int segmento5 = shmget(50,  sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	pid_t pid;
	char **vet;
	char * programa = (char *) shmat(segmento2, 0, 0);
	FILE *arq = fopen("exec.txt", "r");
	vet = (char **) shmat(segmento1, 0, 0);
	vet[0] = (char *) shmat(segmento3, 0, 0);
	vet[1] = (char *) shmat(segmento4, 0, 0);
	prioridade = shmat(segmento5, 0, 0);	

	while(fscanf(arq, "exec %s prioridade=%d\n", programa, prioridade) != EOF){
		sleep(1);
		pid = fork();
		vet[0] = programa;
		sprintf(vet[1], "%d", prioridade);
		printf ("%s", vet[1]);	
		if(pid == 0) {	
			execv("./escalonador.c", vet);
			perror("execv\n");
			exit(EXIT_FAILURE);
		} else if(pid < 0) {
			perror("fork\n");
			exit(EXIT_FAILURE);
		} else {
			wait(&status);	
			if(WIFEXITED(status))
				printf("Interpretador executado com sucesso!\n");
			else
				printf("Interpretador não executado!\n");
		}
	}
	
	fclose(arq);
	return 0;
}