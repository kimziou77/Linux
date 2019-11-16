// gcc thread.c -o thread.out -lpthread
#include <pthread.h>
#include <stdio.h>
void *myThread(void *arg);
int main(int argc, char const *argv[]) {
    pthread_t tid = 0;
    int count = 5;
    int *status;
    printf("Hello. I'm main thread: %ld\n", pthread_self());

    if (pthread_create(&tid, NULL, myThread, (void *)&count)) {
        perror("pthread_create() error!");
        return -1;
    }

    pthread_join(tid, (void **)&status);
    printf("Thread %ld exit\n", tid);

    return 0;
}

void *myThread(void *arg) {
    int i = 0;
    int count = *(int *)arg;
    int status = 0;
    printf("Hello. I\'m new thread: %ld\n", pthread_self());

    for (i = count; i > 0; --i) {
        printf("%d ...\n", i);
    }

    pthread_exit((void *)&status);
}
