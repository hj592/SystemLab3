#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#define PASSWORDSIZE 12
int main(void)
{
    int fd;
    int nread, cnt =0, errcnt =0;
    char ch, text[] ="Hi~ My name is heejin";
    //char ch, text[] ="The magic thing is that you can change it.";
    struct termios init_attr, new_attr;
    time_t  start, end;
    fd = open(ttyname(fileno(stdin)), O_RDWR);
    tcgetattr(fd, &init_attr);
    new_attr = init_attr;
    new_attr.c_lflag &= ~ICANON;
    new_attr.c_lflag &= ~ECHO; /* ~(ECHO | ECHOE | ECHOK | ECHONL); */
    new_attr.c_cc[VMIN] =1;
    new_attr.c_cc[VTIME] =0;
    if (tcsetattr(fd, TCSANOW, &new_attr) !=0) {
        fprintf(stderr, "터미널 속성을 설정할 수 없음.\n");
    }
    printf("다음 문장을 따라 입력하세요~\n%s\n", text);
    time(&start);
    while ((nread =read(fd, &ch, 1)) >0 && ch !='\n') {
        if (ch == text[cnt ++])
            write(fd, &ch, 1);
        else {
            write(fd, "*", 1);
            errcnt++;
        }
    }
    time(&end);
    printf("%f",((float)(end -start)));
    printf("\n타이핑 오류의 횟수는 %d\n", errcnt);
    printf("분당 타자수는 : %.2f\n", (float)(sizeof(text)/sizeof(text[0]))/((float)(end -start))*60.0);
    tcsetattr(fd, TCSANOW, &init_attr);
    close(fd);
}
