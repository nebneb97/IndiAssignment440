#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_CHILDREN 4

int main( void )
{
   void signhandler(int sig);
    pid_t pid;

    int fds[2];
    char some[50];
   
    if (signal(SIGINT,signhandler) == SIG_ERR){
        perror("signal");
        exit(1);
    }
    for(int i = 0; i < MAX_CHILDREN; i++)
    {
        printf("Enter something : ");
        scanf("%s", some);

        if(pipe(fds) == -1)
        {
            perror( "pipe Failed" );
            continue;
        }

        pid = fork();

        if(pid < 0)
        {
            perror("fork failed");
            exit(1);
        }
        
        //child process
        if(pid == 0)
        { 
            printf("What you entered: %s\n", some);

            char buffer[50];
            printf("Child %i (pid = %i)\n", i , getpid());
            close(fds[1]);

            if ( read( fds[0], buffer, sizeof(buffer)) <= 0) //read from pipe
            {
                perror( "read failed" );
                exit( EXIT_FAILURE );
            }

            printf("Input read by child = %s\n", buffer);
            exit(0);
        }
        
        //parent process
        else
        {
            printf("I am the parent %i\n",getpid());
            close(fds[0]);
            write(fds[1], some,strlen(some)+1);
            wait(NULL);
            printf("Parent sends:  %s\n", some);
            
        }
    }

    return 0;
}
 
 //interrupt message
 
void signhandler(int sig)
{
    printf("\n\nProgram Terminated");
    exit(1);
}
