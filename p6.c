#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
int main(){
	int i=50;
	printf("executando p6\n");
	while(i<60){
		i++;
		printf("%d ", i);
		sleep(1);
	}
	printf("\n");
}