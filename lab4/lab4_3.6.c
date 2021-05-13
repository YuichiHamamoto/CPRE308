#include <stdio.h>
#include <stdlib.h>
#define MSGSIZE 16
4
int main() {
    char *msg = "How are you?";
    char inbuff[MSGSIZE];
    int p[2];
    int ret;
    pipe(p);
    ret = fork( );
    if (ret > 0) {
        write(p[1], msg, MSGSIZE);
    } else {
        sleep(1);
        read(p[0], inbuff, MSGSIZE);
        printf("%s\n", inbuff);
}
exit(0); }