#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
int main(){
	int i=30;
	printf("executando p4\n");
	while(i<40){
		i++;
		printf("%d ", i);
		sleep(1);
	}
	printf("\n");
}