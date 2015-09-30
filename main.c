#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <memory.h>
//#include <stdbool.h>

void someFunc(int file,char *chars){
    if (file != -1) {
                        while(*chars!='\0'){
                            write(file,chars,sizeof(char));
                            chars=chars+1;
                        }
                    }
    close(file);
}

int main()
{
    pid_t child_pid;
    char *str1 = (char *)malloc(70);
    char *str2 = (char *)malloc(70);
switch(child_pid = fork()){
        case -1:
            perror("fork");
            break;
        case 0:
            printf("Сhild process!\n");
            sprintf(str1,"./file_%d.txt",getpid());
            int fdest = open (str1,  O_WRONLY/*write*/|O_CREAT/*create if not found */, S_IRUSR|S_IWUSR/*user can read and write*/);
            char *a = (char *)malloc(30);
            sprintf(a,"parent's pid = %d",getppid());
            someFunc(fdest,a);
            printf("end of child \n");
            break;
        default:
            printf("Parent process!\n");
            printf("%d \n",getpid());

            sprintf(str2,"./file_%d.txt",getpid());
            int sdest = open (str2,  O_WRONLY/*write*/|O_CREAT/*create if not found */, S_IRUSR|S_IWUSR/*user can read and write*/);
            char *b = (char *)malloc(30);
            sprintf(b,"childs's pid = %d, my pid = %d",child_pid, getpid());
            someFunc(sdest,b);
            wait(NULL);

            printf("end of app \n");
            break;
}

}
