// gcc thread.c -o thread.out -lpthread
#include <pthread.h>
#include <stdio.h>
void *myThread(void *arg);

int main(int argc, char const *argv[]) {
    pthread_t tid = 0;
    int count = 5;
    int *status;

    printf("Hello. I'm main thread: %ld\n", pthread_self());
    //현재 스레드의 tid 반환

    if (pthread_create(&tid, NULL, myThread, (void *)&count)) {
        // 스레드를 생성한다
        // 1: tid포인터, 2: 대부분은 NULL, 3: 스레드 실행할 함수이름, 4:함수에 전달할 인자
        perror("pthread_create() error!");
        return -1;
    }

    pthread_join(tid, (void **)&status);
    // 스레드가 종료할때까지 대기한다.
    //1: 대기하고자 하는 스레드의 tid, 2: 대기스레드의 종료값을 저장할 포인터
    printf("Thread %ld exit\n", tid);

    return 0;
}

void *myThread(void *arg) {
    // 스레드를 실행할 함수
    int i = 0;
    int count = *(int *)arg;
    int status = 0;
    printf("Hello. I\'m new thread: %ld\n", pthread_self());

    for (i = count; i > 0; --i) {
        printf("%d ...\n", i);
    }

    pthread_exit((void *)&status);
    //현재 스레드를 종료한다. pthread join에 전달할 값을 저장한 주소
}
