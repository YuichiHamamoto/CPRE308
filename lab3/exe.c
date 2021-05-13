#include <stdio.h>
#include <pthread.h>

void * thread1();
void * thread2();

int main(){
	pthread_t t1;
	pthread_t t2;

    // sleep(5);

	/*creation of threads 1 and 2*/
	pthread_create(&t1, NULL, thread1, NULL);
	pthread_create(&t2, NULL, thread2, NULL);

	/*wait for threads 1 and 2 to complete*/
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("Hello from the main thread\n");

	return 0;
}

void * thread1()
{
	printf("Hello from thread 1\n");
}

void * thread2()
{
	printf("Hello from thread 2\n");
}
