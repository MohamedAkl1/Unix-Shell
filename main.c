#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>




char *argv[100];


int pass_to_token(char *input, char **argv);

void execute(char **argv, int checker);

int cd(char **args)
;

int main(void)
{
    char command[512];
    while(1)
    {
        int checker = 0;
        printf("shell> ");
        gets(command);
        checker = pass_to_token(command, argv); //checker to determine whether it's foreground or background
        if(checker == 111){
            perror("too long command\n\n");
            continue;
        }
        if(!strcmp(argv[0],"exit"))
        {
            return 0;
        }else if(strcmp(argv[0],"cd") == 0){ // cd command
            cd(argv);
        }
        else
        {
            execute(argv, checker);//execute command
        }
    }
}


int pass_to_token(char *input, char **argv)
{
    int count=0;
    int checker = 0;
    while (*input != '\0')
    {
        while (*input == ' ' || *input == '\n') {
            *input++ = '\0';
            count++;
        }
        if(strcmp("&", input) == 0){
            checker = 1;
        }else{
            *argv++ = input;
        }
        while (*input != '\0' && *input != ' ' &&
               *input != '\t' && *input != '\n'){
            input++;
            count++;
        }
    }
    if(count >= 512){
        return 111;
    }
    *argv = '\0';
    return checker;
}



void execute(char **argv, int checker)
{
    pid_t pid=fork();
    int status;
    if (pid < 0)
    {
        fprintf(stderr,"command not found\n");
        exit(1);
    }
    else if (pid == 0)
    {
        if (execvp(*argv, argv) < 0) //error creating process
        {
            fprintf(stderr,"command not found\n");
            exit(1);
        }
    }
    else
    {
        if(checker == 1){
            printf("BACKGROUND PROCESS\n\n");
        }else{
            //waitpid(-1,&status,0);
            do{
                waitpid( pid , &status , WUNTRACED);
            }
            while( !WIFEXITED(status) && !WIFSIGNALED(status));
        }


    }
}

int cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("argument error");
        }
    }
    return 1;
}