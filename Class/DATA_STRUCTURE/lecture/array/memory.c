#include <stdio.h>
#include <stdlib.h>

#define SAFE_FREE(p) if (p) {free(p); p=NULL;}
#define MAX_ITEM_CNT 5

int main(void){
	int i=0;

	int *buf1 =(int *)malloc(MAX_ITEM_CNT * sizeof(int));
	int *buf2 =(int *)calloc(MAX_ITEM_CNT, sizeof(int));

	printf("Address of buf 1 : %p\n",buf1);
	for(i=0;i<MAX_ITEM_CNT;i++){printf("%d\n", *(buf1+i));}


	printf("Address of buf 2 : %p\n",buf2);
	for(i=0;i<MAX_ITEM_CNT;i++){printf("%d\n", *(buf2+i));}


	SAFE_FREE(buf1);
	SAFE_FREE(buf2);

	return 0;

}
