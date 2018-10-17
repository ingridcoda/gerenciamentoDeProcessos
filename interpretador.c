#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char * argv[]){
	int status;
	pid_t pid;
	int segmento1 = shmget(10,  100 * sizeof(char), IPC_CREAT | S_IRUSR | S_IWUSR);
	int segmento2 = shmget(20,  sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);	
	char * programa = (char *) shmat(segmento1, 0, 0);	
	int * prioridade = shmat(segmento2, 0, 0);				
	FILE * arq = fopen("exec.txt", "r");

	pid = fork();
	
	/* erro fork */
	if(pid < 0) {
		perror("fork\n");
		exit(EXIT_FAILURE);
	}
	/* se eh filho faz execv do escalonador */
	if(pid == 0){
		execv("./escalonador", NULL);
	} 
	/* se eh pai envia sinal pro escalonador */
	else {
		while(fscanf(arq, "exec %s prioridade=%d\n", programa, prioridade) != EOF){
			sleep(1);
			
			/* envia sinal dizendo que recebeu novo processo */
			kill(pid, SIGUSR1);			
		}
		
		/* espera filho */
		wait(&status);	
		
		/* verifica resultado da execucao do filho */
		if(WIFEXITED(status))
			printf("Interpretador executado com sucesso!\n");
		else
			printf("Interpretador não executado!\n");
	}
	
	fclose(arq);
	return 0;
}