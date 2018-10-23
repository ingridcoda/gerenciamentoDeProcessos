#include "semaforos.h"
#include <string.h>
#include <signal.h>

typedef struct no {
	char *nome;
	int prioridade;
	pid_t pid;
	int executando;
	int novo;
	struct no * prox;
} No;

typedef struct fila {
	No * inicio;
	No * fim;
} Fila;

int filaVazia (Fila * f){
	if (f->inicio == NULL) return 1; //fila vazia	

	return 0; //fila ocupada		
}


Fila *insereFila (Fila * fila, char * nome, int prioridade, int executando, int novo, int pid){
	printf ("entrei insere\n");
	No * elem = (No *) malloc (sizeof(No));
	elem->nome = nome;
	elem->prioridade = prioridade;
	elem->pid = pid;
	elem->executando = executando;
	elem->novo = novo;
	elem->prox = NULL;
	if(filaVazia(fila)){
		fila->inicio = elem;
		fila->fim = elem;
	}	
	else {
		fila->fim->prox = elem;
		fila->fim = elem;
	}
	printf("inserido\n");	
	return fila;
}

No * removeFila (Fila * f){
	No * elem = (No *) malloc (sizeof(No));
	elem = f->inicio;
	if(f->inicio == f->fim){
		f->inicio = NULL;
		f->fim = NULL;
	} else f->inicio = f->inicio->prox;
	return f->inicio;
}

No * removeElemFila (No * f, int n){
	int i = 0;
	No * elem = (No *) malloc (sizeof(No));
	elem = f;
	while(i < n){
		elem = elem->prox;
	} 
	elem->prox = elem->prox->prox;
	return f;
}


void escalonaRR(No *f){	
	int i, count = 0;
	pid_t pid;
	No * aux;
	printf("Entrei no escalonaRR\n");
	if(aux == NULL) printf("Erro malloc aux fila");
	for(aux = f; aux->prox != NULL; aux = aux->prox){
		printf("Entrei no for escalonaRR\n");
		if(aux->executando){
			printf("Entrei no for if escalonaRR\n");
			kill(aux->pid, SIGSTOP);
			break;			
		} else count++;		
	}
	printf("Sai do for escalonaRR\n");
	if(aux->novo){
		printf("Entrei no if novo escalonaRR\n");
		pid = fork();
		//erro fork
		if(pid < 0){
			exit(1);
		}
		//filho
		else if(pid == 0){
			execv(aux->nome, NULL);
			aux->executando = 1;
			aux->novo = 0;
			printf ("Executando programa: %s\n",aux->nome);  
		}
		//pai
		else {
			wait(&i);
			f = removeElemFila(f, count+1);
			f = f->prox;
		}
	} else {
		printf("Entrei no else novo escalonaRR\n");
		kill(aux->pid, SIGCONT);
		aux->executando = 1;
		printf ("Executando programa: %s\n",aux->nome);  
	}
}

void escalonaOutros(No * elem){
	int i;
	pid_t pid;
	pid = fork();
	//erro fork
	if(pid < 0){
		perror("Erro fork escalonaOutros");
		exit(1);
	}
	//filho
	else if(pid == 0){
		if(elem->novo){
			execv(elem->nome, NULL);
			elem->executando = 1;
			elem->novo = 0;
			printf ("Executando programa: %s\n",elem->nome);  
		} else {
			kill(elem->pid, SIGCONT);
			elem->executando = 1;
		}
	}
	//pai
	else {
		wait(&i);
		printf("Programa %s terminou - cod %d\n", elem->nome, i);
	}	
}

int main(int argc, char * argv[]){
	pid_t * pid;
	int i, j, status, **segmento, fd, *prioridadeNovo, *prioridadeAtual, *novo, *executando, fim = 0;
	char *nomeAtual, *nomeNovo;
	Fila ** filas;
	//nomeNovo
	int segmento1 = shmget(10, 100 * sizeof (char), IPC_CREAT | S_IRUSR | S_IWUSR);
	//prioridadeNovo
	int segmento2 = shmget(20, sizeof (int), IPC_CREAT | S_IRUSR | S_IWUSR);
	//executando
	int segmento3 = shmget(30, sizeof (int), IPC_CREAT | S_IRUSR | S_IWUSR);
	//novo
	int segmento4 = shmget(40, sizeof (int), IPC_CREAT | S_IRUSR | S_IWUSR);
	//pid
	int segmento5 = shmget(50, sizeof (pid_t), IPC_CREAT | S_IRUSR | S_IWUSR);
	//nomeAtual
	int segmento6 = shmget(60, 100 * sizeof (char), IPC_CREAT | S_IRUSR | S_IWUSR);
	//prioridadeAtual
	int segmento7 = shmget(70, sizeof (int), IPC_CREAT | S_IRUSR | S_IWUSR);
	//semaforo
	int semId = semget (12345, 1, 0666 | IPC_CREAT);

	setSemValue(semId);
	
	filas = (Fila **) malloc (7 * sizeof(Fila*));
	for(i = 0; i < 7; i++){
		filas[i] = (Fila *) malloc (sizeof(Fila));
	}

	printf ("Pid escalonador: %d\n", getpid());
	nomeNovo = shmat(segmento1, 0, 0);
	prioridadeNovo = (int *) shmat(segmento2, 0, 0);
	executando = (int *) shmat(segmento3, 0, 0);
	*executando = 0;
	novo = (int *) shmat(segmento4, 0, 0);
	*novo = 1;
	pid = (pid_t *) shmat(segmento5, 0, 0);
	*pid = 0;
	nomeAtual = shmat(segmento6, 0, 0);
	prioridadeAtual = (int *) shmat(segmento7, 0, 0);
	
	if ((fd=open("saida.txt", O_RDWR|O_TRUNC,0666)) == -1){
		printf("Error open() ");
		return -1;
	}
	close(1);
	if(dup2(fd, 1) == -1){
		printf("Error dup2");
		return -1;
	}

	if(filas[0] == NULL) {
		printf("Fila null");
	}
	
	j=1;
	for(;;){
                semaforoP(semId);
		memcpy((void*)nomeAtual, (void*)nomeNovo, sizeof(nomeNovo));
		if(*prioridadeNovo != -1) memcpy((void*)prioridadeAtual, (void*)prioridadeNovo, sizeof(prioridadeNovo));
		else fim = 1;
		//semaforoV(semId);
		if(!fim){
			//se prioridade eh valida
			printf("Valor prioridade vez %d: %d\n", j, *prioridadeAtual);
			printf("Valor nome programa vez %d: %s\n", j, nomeAtual);	
			if(*prioridadeAtual > 0 && *prioridadeAtual < 8){
				printf("entrei prioridade ok\n");
				//insere na fila adequada
				printf("Antes insere\n");
			
				filas[*prioridadeAtual-1] = insereFila(filas[*prioridadeAtual-1], nomeAtual, *prioridadeAtual, *executando, *novo, *pid);
				printf("Depois insere\n");
			
			} 
			//prioridade invalida
			else {
				printf("prioridade não encontrada\n");
				exit(1);
			}
		
			for(i = 0; i < 7; i++){
				printf("Fila %d vazia? %s\n", i, (filaVazia(filas[i])? "sim": "nao"));
				if(!filaVazia(filas[i])){
					printf("if fila %d\n", i);	
					if(i < 4){
						//Realtime (1 e 2) e Prioridade (3 a 5)	
						escalonaOutros(filas[i]->inicio);
						filas[i]->inicio = removeFila(filas[i]);
					} else {
						//RoundRobin (6 e 7)
						sleep(1);
						printf("RR %d\n", i);
						escalonaRR(filas[i]->inicio);
					}
					printf("Fila %d vazia? %s\n", i, (filaVazia(filas[i])? "sim": "nao"));
				}
			}
		} else exit(0);
				
		semaforoV(semId);
		j++;	
		
	}
	
	return 0;
}
