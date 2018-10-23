#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
int main(){
	int i=0;
	printf("executando p1\n");
	while(i<10){
		i++;
		printf("%d ", i);
		sleep(1);
	}
	printf("\n");
}
