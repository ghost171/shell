#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>
#include <fcntl.h>
#include <err.h>

char *get_word(char *end)
    {
    if (*end == '\n') { 
        return NULL;
    }
    int i = 0;
    char ch, *word = NULL, *check = NULL;
    do {
        ch = getchar();
        while (!i && (ch == ' ' || ch == '\t')) { 
            ch = getchar();
            if (ch == '\n') {
                return NULL;
            }
        }
        check = (char *)realloc(word, (i + 1) * sizeof(char));
        if (check == NULL) {
            err(1, NULL);
        }
        word = check;
        word[i] = ch;
        i++;
    } while (ch != ' ' && ch != '\t' && ch != '\n');
    word[i - 1] = '\0'; 
    *end = ch;
    return word;
}

char ** free_list(char ** list){
    if(list == NULL){
        return list;
    }
    for(int i = 0; list[i] != NULL; i++){
        free(list[i]);
    }
    free(list);
    return list;

}

char **get_list(void)
{
    char end = 0, **list = NULL, **ch = NULL;
    int i = 0;
    do {
        ch = (char **)realloc(list, (i + 1) * sizeof(char *));
        if (ch == NULL) {
            free_list(list);
            err(1, NULL);
        }
        list = ch;
        list[i] = get_word(&end);
        i++;
    } while (list[i - 1] != NULL);
    return list;
}

int io_pipes(char ** argvec){
    for(int i = 0; argvec[i] != NULL; i++){
        if(strcmp(argvec[i], "|") == 0){
            return 1;
        }
    }
    return 0;
}

void pipes(char ** argvec){
    int i = 0;
    char ** cmd_A = NULL;
    char **  cmd_B = NULL;
    for(i = 0; strcmp(argvec[i], "|") != 0; i++){
        cmd_A = (char **)realloc(cmd_A, sizeof(char *) * (i + 1));
        cmd_A[i] = argvec[i];
    }
    cmd_A = (char **)realloc(cmd_A, sizeof(char *) * (i + 2));
    cmd_A[i + 1] = NULL;
    int j = 0;
    for(j = 0; argvec[i + j + 1] != NULL; j++){
        cmd_B = (char **)realloc(cmd_B, sizeof(char *) * (j + 1));
        cmd_B[j] = argvec[i + j + 1];
    }
    cmd_B = (char **)realloc(cmd_B, sizeof(char *) * (j + 2));
    cmd_B[j + 1] = NULL; 
    int fd[2];
    pipe(fd);
    if(fork() == 0){
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        execvp(cmd_A[0], cmd_A);
        return;
    }
    wait(NULL);
    if(fork() == 0){
        dup2(fd[0], 0);
        close(fd[1]);
        close(fd[0]);
        execvp(cmd_B[0], cmd_B);
        return;
    }
    wait(NULL);
}

int infinity(){
    char ** argvec = NULL;
    char * cmd = NULL;
    while(1){
        argvec = get_list();
        cmd = argvec[0];
        if (io_pipes(argvec) == 1){
            pipes(argvec);
        }
        if(argvec != NULL && ((strcmp(cmd, "exit") == 0) || 
            (strcmp(cmd, "quit") == 0))){
                free_list(argvec);
                return 0;
            }
        pid_t pid = fork();
        if ((pid == 0) && (io_pipes(argvec) != 1)){
            if (execvp(cmd, argvec) < 0){
                perror("No such command");
            }
        }
        if (pid == 0){
            return 0;
        }
        free_list(argvec);
    }
    return 0;
}

int infinity1(){
    char * a = NULL;
    char end = '1';
    while(1){
        a = get_word(&end);
        if (strcmp(a, "exit") == 0 || strcmp(a, "quit") == 0){
           free(a);
           exit(1); 
        }
        puts(a);
        pid_t pid = fork();
        if(pid == 0){
            break;
        }
        free(a);
    }
    return 0;
}

int main(int argc, char ** argv){
    infinity();
    return 0;
}