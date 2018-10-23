#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
int main(){
	int i=60;
	printf("executando p7\n");
	while(i<70){
		i++;
		printf("%d ", i);
		sleep(1);
	}
	printf("\n");
}