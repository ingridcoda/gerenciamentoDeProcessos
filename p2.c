#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
int main(){
	int i=10;
	printf("executando p2\n");
	while(i<20){
		i++;
		printf("%d ", i);
		sleep(1);
	}
	printf("\n");
}
