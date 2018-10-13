#include <stdio.h>
#include <stdib.h>

int main(int argc, char * argv[]){
	char * programa = (char *) malloc (255 * sizeof(char));
	int prioridade;
	char * arg[];
	FILE * arq = fopen("saida.txt", "w");
	
	programa = argv[0];
	atoi(argv[1], prioridade);
	
	while(1){
		switch(prioridade){
			case 1 || case 2: //Processos REAL TIME
				break;
			case 6: //Processos ROUND-ROBIN CPU-BOUND
				break;
			case 7: //Processos ROUND-ROBIN I/O-BOUND
				break;
			default: //Outros processos (???)
				break;
		}
	}
}