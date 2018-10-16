#include <stdio.h>
#include <stdlib.h>

typedef struct programa {
	char *nomeprograma;
	int prioridade;
} Programa;

typedef struct fila {
	Programa programa;
	Programa *prox;
} Fila;



Fila *testaPrioridade (Programa atual, Programa novo, Fila fila){
	Fila p = fila;
	if (atual.prioridade > novo.prioridade){

		//SIGSTOP atual;
		signal(SIGSTOP);
		while (p.prox != NULL){
			p= p->prox;
		}
		p->prox = novo;
		p->prox->prox = NULL;
		
		//sort fila
		
		//SIGCONT 1o da fila
		if (fila.programa.nomeprograma == novo.nomeprograma)
			signal(SIGSTART);
		else 
			signal (SIGCONT);
	}
	return fila;
}


int main(int argc, char * argv[]){
	char * programa = (char *) malloc (255 * sizeof(char));
	int prioridade;
	char * arg[2];
	FILE * arq = fopen("saida.txt", "w");
	
	programa = argv[0];
	prioridade = atoi(argv[1]);
	
	while(1){
		switch(prioridade){
			case 1 || 2: //Processos REAL TIME
																																										
				break;
			case 6: //Processos ROUND-ROBIN CPU-BOUND
				break;
			case 7: //Processos ROUND-ROBIN I/O-BOUND
				break;
			default: //Outros processos (???)
				break;
		}
	}
	
	fclose(arq);
	return 0;
}
