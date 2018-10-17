#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
int novoProcesso = 0;

typedef struct programa {
	char * nome;
	int * prioridade;
	pid_t pid;
} Programa;

typedef struct fila {
	Programa * programa;
	struct fila * prox;
} Fila;

void signal_handler(int sig) {
    switch (sig) {
		case SIGUSR1: //NOVO PROCESSO CHEGOU VIA INTERPRETADOR	
			novoProcesso = 1;
			break;
		default:
			break;
    }
}

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

void escalona(Fila *f, int prioridade, pid_t pid, pid_t pidExecutando){
	f->programa->pid = pid;
	
	//tratador de politicas de escalonamento
	switch(prioridade){
		case 1: //Processos REAL TIME			
			break;

		case 2: //Processos REAL TIME	
			break;

		case 3: //Processos PRIORIDADE SIMPLES
			break;

		case 4: //Processos PRIORIDADE SIMPLES
			break;

		case 5: //Processos PRIORIDADE SIMPLES
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

	/* alguma fila mais prioritaria nao esta vazia, 
	logo nao eh pra executar agora o novo processo */
	for(i = 0; i < prioridade-1; i++)
		if(!filaVazia(filas[i])) return 0; 		

	/*todas as filas mais prioritarias estao vazias */
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
	
	//valor instanciado pelo interpretador
	*segmento[9] = shmget(10, 255 * sizeof (char), IPC_CREAT | S_IRUSR | S_IWUSR);
	p->nome = (char *) shmat(*segmento[9], 0, 0);
	
	//valor instanciado pelo interpretador
	*segmento[10] = shmget(20, sizeof (int), IPC_CREAT | S_IRUSR | S_IWUSR);
	p->prioridade = (int *) shmat(*segmento[10], 0, 0);
	
	//configurando SIGUSR1 para usar handler que trata processo novo recebido
	signal(SIGUSR1, signal_handler);

	while(1){
		//recebeu sinal de novo processo
		if(novoProcesso){
			novoProcesso = 0;
			//se prioridade eh valida
			if(*p->prioridade > 0 && *p->prioridade < 8){
				//insere na fila adequada
				insereFila(filas[*p->prioridade-1], p);	
				//verifica se eh prioritario
				if(testaPrioridade(filas, *p->prioridade)){
					if(filas[*p->prioridade-1]->programa == p){
						pid = fork();
						//erro fork
						if(pid < 0){
							exit(1);
						}
						//filho
						else if(pid == 0){
							execv(*p->nome, NULL);
						}
						//pai
						else {
							//escalona(?) ou apenas espera filho
						}
					}			
					
				} 
				//nao eh prioritario
				else {
					//nao faz nada(?)
				}					
			} 
			//prioridade invalida
			else {
				printf("prioridade não encontrada\n");
				exit(1);
			}
		} 
		//nao tem novo processo
		else {
			//escalona(?) ou nao faz nada (?)
		}
	}
	
	fclose(arq);
	return 0;
}