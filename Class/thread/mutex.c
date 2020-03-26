//뮤텍스를 이용한 스레드 동기화 예시
#include <pthread.h>
#include <stdio.h>
void *myThread1(void *arg);
void *myThread2(void *arg);
void cleanupHandler(void *arg);

pthread_mutex_t mutex;
int sharedNum = 0;

int main(int argc, char const *argv[]) {

    pthread_t tid1 = 0;
    pthread_t tid2 = 0;
    int *status;

    pthread_mutex_init(&mutex, NULL);
    // 뮤텍스를 초기화한다.
    // 1: 뮤텍스를 저장한 메모리공간 포인터, 2: 보통 NULL


    if (pthread_create(&tid1, NULL, myThread1, NULL)) {
        perror("pthread_create() error!");
        goto END;
    }

    if (pthread_create(&tid2, NULL, myThread2, NULL)) {
        perror("pthread_create() error!");
        goto END;
    }

    pthread_join(tid1, (void **)&status);
    pthread_join(tid2, (void **)&status);

    END:
    pthread_mutex_destroy(&mutex);
    return 0;
}
void *myThread1(void *arg) {
    int i = 0;
    int status = 0;

    pthread_cleanup_push(cleanupHandler, "Thread 1");
    // Tread 1 이라는 인자를 넘기면서 cleanupHandler를 실행하는게아니라
    // 실행하지않고 실행할걸 저장한다.
    pthread_mutex_lock(&mutex);
    // 실행이 끝나면 열린 mutex를 다시 잠근다.
    puts("Thread 1");

    for (i = 0; i < 5; ++i) {
        sharedNum += i;
        printf("%d ", sharedNum);
    }

    puts("");
    pthread_cleanup_pop(1);
    // 아까 cleanupHandler 에 저장했던 함수를 실행한다.

    pthread_exit((void *)&status);
    // 함수의 마지막은 항상 스레드 종료!
}

void *myThread2(void *arg) {
    int i = 0;
    int status = 0;

    pthread_cleanup_push(cleanupHandler, "Thread 2");
    pthread_mutex_lock(&mutex);
    puts("Thread 2");

    for (i = 0; i < 5; ++i) {
        sharedNum -= i;
        // 전역인 shared Num은 스레드를 실행하면서 동시에 접근가능했지만
        // mutex로 잠긴 상태에서 thread 1이 먼저 쓰고 있었기 때문에 동시접근을 막고
        // 스레드 1이 10까지 올려놓고 반환한 뮤텍스(접근할 수 있는 키)를 받아 수를 다시 줄인다.
        printf("%d ", sharedNum);
    }

    puts("");
    pthread_cleanup_pop(1);
    pthread_exit((void *)&status);
}

void cleanupHandler(void *arg) {
    pthread_mutex_unlock(&mutex);
    // 열쇠와 같은 개념으로 뮤텍스를 받으면 unlock 해주고 다음을 실행한다.
    printf("Cleanup: %s\n", (char *)arg);
}
