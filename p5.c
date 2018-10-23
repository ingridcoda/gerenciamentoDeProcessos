#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
int main(){
	int i=40;
	printf("executando p5\n");
	while(i<50){
		i++;
		printf("%d ", i);
		sleep(1);
	}
	printf("\n");
}