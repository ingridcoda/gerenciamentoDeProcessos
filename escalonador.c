#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

typedef struct programa {
	char * nome;
	int * prioridade;
	pid_t pid;
} Programa;

typedef struct fila {
	Programa * programa;
	struct fila * prox;
} Fila;

void insereFila (Fila * fila, Programa * p){
	Fila * f = (Fila *) malloc (sizeof(Fila));
	f = fila;
	while (f->prox != NULL){
		f= f->prox;
	}
	f->programa = p;
	f->prox = NULL;
}

Programa * removeFila (Fila * f){
	Programa *p = (Programa *) malloc (sizeof(Programa));
	f->programa = f->prox->programa;
	p = f->programa;
	f->prox = NULL;
	f->prox= f->prox->prox;
	return p;
}

int filaVazia (Fila * f){
	if (f->prox == NULL && f->programa == NULL) return 1; //fila vazia	
	return 0; //fila ocupada		
}

void executa(Fila *f, int prioridade, pid_t pid, pid_t pidExecutando){
	f->programa->pid = pid;
	
	//tratador de politicas de escalonamento
	switch(prioridade){
		case 1: //Processos REAL TIME			
			break;

		case 2: //Processos REAL TIME	
			break;

		case 3:
			break;

		case 4:
			break;

		case 5:
			break;

		case 6: //Processos ROUND-ROBIN CPU-BOUND
			break;

		case 7: //Processos ROUND-ROBIN I/O-BOUND
			break;

		default: //Outros processos (???)
			printf("prioridade não encontrada\n");
			break;
			exit(1);		
	}
	pidExecutando = pid;
}



int testaPrioridade(Fila * filas[], int prioridade){
	int i;	

	//alguma fila mais prioritaria nao esta vazia, logo nao eh pra executar agora o novo processo
	for(i = 0; i < prioridade-1; i++)
		if(!filaVazia(filas[i])) return 0; 		

	/*todas as filas mais prioritarias estao vazias, porem processo novo nao eh o primeiro da sua fila, 
	  logo nao eh pra executar agora o novo processo*/
	if(!filaVazia(filas[prioridade-1])) return 0;	

	/*todas as filas mais prioritarias estao vazias e processo novo eh o primeiro da sua fila, 
	  logo eh pra executar agora o novo processo*/
	return 1;
}

int main(int argc, char * argv[]){
	pid_t pid, executando;
	int i, seg, status, **segmento;
	FILE * arq = fopen("saida.txt", "w");
	Fila ** filas;
	Programa * p;	

	//adicionando todas as variaveis que usaremos na memoria compartilhada, para serem acessiveis pelos processos filhos.
	seg = shmget(100, (11 * sizeof (int*)), IPC_CREAT | S_IRUSR | S_IWUSR);
	segmento = (int **) shmat(seg, 0, 0);	

	*segmento[0] = shmget(200, (7 * sizeof (Fila *)), IPC_CREAT | S_IRUSR | S_IWUSR);
	filas = (Fila **) shmat(*segmento[0], 0, 0);	

	*segmento[1] = shmget(300, sizeof (Programa), IPC_CREAT | S_IRUSR | S_IWUSR);
	p = shmat(*segmento[1], 0 , 0);	

	for(i = 2; i < 9; i++){
		*segmento[i] = shmget(100*(i+2), sizeof (Fila *), IPC_CREAT | S_IRUSR | S_IWUSR);
		filas[i] = (Fila *) shmat(*segmento[i], 0, 0);
	}		

	*segmento[9] = shmget(1100, 255 * sizeof (char), IPC_CREAT | S_IRUSR | S_IWUSR);
	p->nome = (char *) shmat(*segmento[9], 0, 0);

	*segmento[10] = shmget(1200, sizeof (int), IPC_CREAT | S_IRUSR | S_IWUSR);
	p->prioridade = (int *) shmat(*segmento[10], 0, 0);

	while(1){
		p->nome = argv[0];
		*p->prioridade = atoi(argv[1]);
		if(*p->prioridade > 0 && *p->prioridade < 8){
			pid = fork();
			if (pid < 0) {
				perror("fork\n");
				exit(EXIT_FAILURE);
			} else if (pid == 0) { //é filho
				//verifica se eh prioritario
				if(testaPrioridade(filas, *p->prioridade)){
					insereFila(filas[*p->prioridade], p);
					executa(filas[*p->prioridade], *p->prioridade, pid, executando);
				} else insereFila(filas[*p->prioridade], p);
			} else {
				wait(&status);
				if(WIFEXITED(status)) {
					printf("Escalonador executado com sucesso!\n");
					exit(0);
				} else {
					printf("Escalonador não executado!\n");
					exit(1);
				}
			}
		} else {
			printf("prioridade não encontrada\n");
			exit(1);
		}
	}
	
	fclose(arq);
	return 0;
}