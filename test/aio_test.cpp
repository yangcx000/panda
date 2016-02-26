#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<libaio.h>
#include<errno.h>
#include<unistd.h>

#define MAX_EVENTS 10

int main(void) {
    int output_fd;
    char* test_content="hello world!";
    const char *output_file="./hello.txt";
    struct iocb io, *p=&io;
    struct io_event e;
    struct timespec timeout;

    io_context_t ctx;
    memset(&ctx,0,sizeof(ctx));
    if (io_setup(MAX_EVENTS, &ctx)!=0) {//init
        printf("io_setup error\n");
        return -1;
    }
    if ((output_fd=open(output_file, O_CREAT|O_WRONLY, 0644)) < 0) {
        perror("open error");
        io_destroy(ctx);
        return -1;
    }
    io_prep_pwrite(&io, output_fd, test_content, strlen(test_content), 10);
    io.data=test_content;
    if(io_submit(ctx,1,&p)!=1) {
        io_destroy(ctx);
        printf("io_submit error\n");
        return -1;
    }
    while(1) {
        timeout.tv_sec=0;
        timeout.tv_nsec=500000000;//0.5s
        if (io_getevents(ctx,0,1,&e,&timeout)==1){
            close(output_fd);
            break;
        }
        printf("haven't done\n");
        sleep(1);
    }
    io_destroy(ctx);
    return 0;
}
