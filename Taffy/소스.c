#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_QUEUE_SIZE 480

typedef struct {
    int id;
	int arriveTime;
	int startTime;
	int waitTime;
	int serviceTime;
}element;

typedef struct {
	int front;
	int rear;
	element data[MAX_QUEUE_SIZE];
}QueueType;

void error(char* message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}

void init_queue(QueueType* q)
{
    q->front = q->rear = 0;
}
//비어있는지 확인
int is_empty(QueueType* q)
{
    return (q->front == q->rear);
}
//가득차있는 확인
int is_full(QueueType* q)
{
    return ((q->rear + 1) % MAX_QUEUE_SIZE == q->front);
}
//삽입 큐
void enqueue(QueueType* q, element item)
{
    if (is_full(q))
    {
        error("포화상태입니다.\n");
    }
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->data[q->rear] = item;
}
//삭제 큐
element dequeue(QueueType* q)
{
    if (is_empty(q))
    {
        error("공백상태입니다.\n");
    }
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    return q->data[q->front];
}

int main(void)
{
    int minutes = 480; //60분을 담기 위한 변수
    int total_wait = 0; //총 대기시간을 담기 위한 변수
    int total_customers = 1; //총 고객번호를 담기 위한 변수
    int service_time = 0; //손님 서비스 시간을 담기 위한 변수
    int service_customer; //손님 고객번호를 담기 위한 변수
    bool Counter = true; //서비스 여부

    QueueType q;
    init_queue(&q);
    srand(time(NULL)); //rand()를 초기화해주는 역할

    for (int clock = 0; clock < minutes; clock++)
    {
        printf("\n====================================현재시각=%d분======================================\n", clock);
        if (Counter == 0) printf("태피는 현재 작업 중입니다.\n");
        else printf("태피는 현재 휴식 중입니다.\n");
        if ((rand() % 4+1) == 4)
        {
            element customer;
            customer.id = total_customers++;
            customer.arriveTime = clock;
            customer.serviceTime = rand() % 10 + 1;
            enqueue(&q, customer);
            printf("고객 %d이 %d분에 들어옵니다. 고객 업무처리시간=%d\n", customer.id, customer.arriveTime, customer.serviceTime);
        }

        if (service_time > 0) //a창구 고객의 서비스 시간이 0보다 클 경우
        {
            printf("고객 %d이 업무처리중입니다.\n", service_customer);
            service_time--;

            if (service_time == 0)
            {
                printf("(태피가 %d분에 현재 업무를 마무리합니다.)\n", clock + 1);
                Counter = true;
            }
        }
        else if (Counter) //a창구 서비스가 열려있을 경우
        {
            if (!is_empty(&q))
            {
                element customer = dequeue(&q);
                service_customer = customer.id;
                service_time = customer.serviceTime;

                printf("고객 %d이 %d분에 A창구에서 업무를 시작합니다. 대기시간은 %d분이었습니다.\n현재 대기중인 인원 : %d\n", customer.id, clock, clock - customer.arriveTime, total_customers - service_customer-1);
                Counter = false;
                total_wait += clock - customer.arriveTime;
            }
        }

    }

    printf("\n\n\n====================================현재시각 = 480분======================================\n");
    printf("전체 대기 시간 = %d시간 %d분(%d분) \n작업을 끝내지 못한 인원 : %d\n", total_wait/60, total_wait%60, total_wait, total_customers - service_customer);
    return 0;
}