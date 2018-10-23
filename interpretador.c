#include "semaforos.h"


int main(int argc, char * argv[]){
	int status;
	pid_t pid;
	int segmento1 = shmget(10,  100 * sizeof(char), IPC_CREAT | S_IRUSR | S_IWUSR);
	int segmento2 = shmget(20,  sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);	
	char * programa = (char *) shmat(segmento1, 0, 0);	
	int * prioridade = shmat(segmento2, 0, 0);				
	FILE * arq = fopen("exec.txt", "r");
	int semId = semget (12345, 1, 0666 | IPC_CREAT);
	setSemValue(semId);
	printf ("Interpretador %d inicializado!\n", getpid());

	pid = fork();
	
	/* erro fork */
	if(pid < 0) {
		perror("fork\n");
		exit(EXIT_FAILURE);
	}
	/* se eh filho faz execv do escalonador */
	if(pid == 0){
		execv("escalonador", NULL);
	} 
	/* se eh pai envia sinal pro escalonador */
	else {		
		while(fscanf(arq, "exec %s prioridade=%d\n", programa, prioridade) != EOF){
			semaforoP(semId);
			printf("Programa %s prioridade %d pid %d identificado pelo interpretador.\n", programa, *prioridade, pid);
			semaforoV(semId);
			sleep(1);	
		}

		printf("Terminou de adicionar os programas no escalonador.\n");

		sleep(1);			

		semaforoP(semId);
		*prioridade = -1;
		semaforoV(semId);
				
		/* espera filho */
		wait(&status);	
		printf("Interpretador finalizado!\n");
	}
	
	fclose(arq);
	return 0;
}
