#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
int main(){
	int i=20;
	printf("executando p3\n");
	while(i<30){
		i++;
		printf("%d ", i);
		sleep(1);
	}
	printf("\n");
}