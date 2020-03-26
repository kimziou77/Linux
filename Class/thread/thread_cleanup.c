#include <pthread.h>
#include <stdio.h>
void *myThread(void *arg);
void cleanupHandler(void *arg);

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

    pthread_cleanup_push(cleanupHandler, "Handler 1");
    // 스레드의 cleanup 핸들러를 스택에 저장함
    // 자원을 해제하거나, 뮤텍스를 해제할 때 주로 사용됨
    pthread_cleanup_push(cleanupHandler, "Handler 2");
    // 1: cleanup핸들러 함수 포인터
    // 2: 함수에 전달할 인자

    for (i = count; i > 0; --i) {
        printf("%d ...\n", i);
    }

    pthread_cleanup_pop(1);
    // 핸들러를 스택에서 가져와 실행한다.
    pthread_cleanup_pop(1);
    //0이면 실행안하고 삭제

    pthread_exit((void *)&status);
}

void cleanupHandler(void *arg) {

    printf("Cleanup: %s\n", (char *)arg);
}
