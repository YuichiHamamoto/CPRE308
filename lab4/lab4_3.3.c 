#include <signal.h>
#include <stdio.h>

void exception();

int main()
{
	signal(SIGFPE, exception);
	int a = 4;
    int b  = 0;
	a = a/b;

	return 0;
}

void exception()
{
	printf("SIGFPE\n");
}