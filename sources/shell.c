#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <err.h>
#include <wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

/* this is color
    for our freetings row
    in function print_greeetings*/
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int complete_exec(char ** argvec, char * pwd);

pid_t gpid;

/*
    IMPLEMENTING DIVIDING ON LEXEMS
*/

/*this function returns words
    in user's entering data
    so, it returns the last symbol in the word*/

char * get_word(char * end){
    char ch = EOF;
    int i = 0;
    char * check = NULL;
    char * word = NULL;
    if(*end == '\n'){
        return NULL;
    }
    do {
        ch = getchar();
        if(!i && ch == '\n'){
            return NULL;
        }
        while (!i && (ch == ' ' || ch == '\t' )) { 
            ch = getchar();
            if (ch == '\n') {
                return NULL;
            }
        }
        if(ch == '<' || ch == '>'){
            check = (char *)realloc(word, (i + 1) * sizeof(char *));
            if(check == NULL){
                err(1, NULL);
            }
            word = check;
            word[i] = ch;
            i++;
            word[i + 1] = '\0';
            *end = ch;
            puts(word);
            return word;
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

//this function can clear our list(char **)
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

//this function dividing entering row
//on lexems(word) for correcting execute
char **get_list(void)
{
    char end = 0, **list = NULL, **ch = NULL;
    int i = 0;
    do{
        ch = (char **)realloc(list, (i + 1) * sizeof(char *));
        if (ch == NULL) {
            free_list(list);
            err(1, NULL);
        }
        list = ch;
        list[i] = get_word(&end);
        if(list[0] == NULL){
            return NULL;
        }
        i++;
    } while (list[i - 1] != NULL);
    return list;
}

/*
    IMPLEMENTING PIPE THROUGH 2 PROGRAMS
*/

/*check existence of 1 pipe
  returns 0 if it is not exist
  else 1*/
int io_pipes(char ** argvec){
    for(int i = 0; argvec[i] != NULL; i++){
        if(strcmp(argvec[i], "|") == 0){
            return 1;
        }
    }
    return 0;
}

//implementing pipe through 2 programs
int exec_pipes(char ** cmd_A, char ** cmd_B){
    int fd[2];
    pipe(fd);
    if(fork() == 0){
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        gpid = getpid();
        execvp(cmd_A[0], cmd_A);
        return 0;
    }
    wait(NULL);
    if(fork() == 0){
        dup2(fd[0], 0);
        close(fd[1]);
        close(fd[0]);
        execvp(cmd_B[0], cmd_B);
        return 0;
    }
    close(fd[1]);
    close(fd[0]);
    wait(NULL);
    return 0;
}

/*dividing row with pipe on two commands
  for correcting execute*/
int pipes(char ** argvec){
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
    exec_pipes(cmd_A, cmd_B);
    return 0;
}

/*
        IMPLEMENTING REDIRECTION
*/

//check existence of "<"
int io_direct_left(char ** argvec){
    for(int i = 0; argvec[i] != NULL; i++){
        if (!strcmp(argvec[i], "<")){
            return 1;
        }
    } 
    return 0;
}
//check existence of ">"
int io_direct_right(char ** argvec){
    for(int i = 0; argvec[i] != NULL; i++){
        if(!strcmp(argvec[i], ">")){
            return 1;
        }
    }
    return 0;
}


//redirect command's output to file
void direct_exec_right(char ** cmd_A, char ** cmd_B){
    int fd = open(cmd_B[0], O_RDONLY | O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fork() == 0){
        gpid = getpid();
        dup2(fd, 1);
        execvp(cmd_A[0], cmd_A);
    }
    close(fd);
}

//divide row on file and command
void direct_to_right(char ** argvec){
    int i = 0;
    char ** cmd_A = NULL;
    char **  cmd_B = NULL;
    for(i = 0; strcmp(argvec[i], ">") != 0; i++){
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
    direct_exec_right(cmd_A, cmd_B);
}

//redirect file's data to command
void direct_exec_left(char ** cmd_A, char ** cmd_B){
    int fd = open(cmd_A[0], O_RDONLY, 0755 );
    if (fd < 0){
        printf("error: %s", strerror(errno));
        exit(1);
    }
    if (fork() == 0){
        dup2(fd, 0);
        gpid = getpid();
        execvp(cmd_B[0], cmd_B);
    }
}

//dividing row on two lexems: command and file
void direct_to_left(char ** argvec){
    int i = 0;
    char ** cmd_A = NULL;
    char **  cmd_B = NULL;
    for(i = 0; strcmp(argvec[i], "<") != 0; i++){
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
    direct_exec_left(cmd_B, cmd_A);
}

//print greetings row with important environment variables
int print_greetings(char * pwd){
    wait(NULL);
    printf(ANSI_COLOR_BLUE  "%s" ANSI_COLOR_RESET,getenv("USER"));
    printf(ANSI_COLOR_CYAN "@" ANSI_COLOR_RESET);
    char * name = (char *)malloc(sizeof(char)  * 9);
    gethostname(name, 9);
    printf(ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET, name);
    printf(ANSI_COLOR_CYAN ":" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET, pwd);
    printf(ANSI_COLOR_RED "$" ANSI_COLOR_RESET);
    return 0;
}

//implement moving through directories
char * cd_func(char ** argvec, char * pwd){
    wait(NULL);
    char * home = getenv("HOME");
    if(argvec[1] == NULL || strcmp(argvec[1], "~") == 0){
        chdir(home);
        strcpy(pwd, getenv("HOME"));
    } else if(strcmp(argvec[1], "..") == 0){
        chdir(strcat(getenv("PWD"), "/.."));
    }
    else{
        if(chdir(argvec[1]) < 0){
            perror("Failed to find a direcctory");
        }
        else{
            strcat(pwd, "/");
            strcat(pwd, argvec[1]);
        }
    }
    return pwd;
}

/*IT'S REALIZE OF "&&" AND "||" CONTAINERS*/

/*  implement "&&" comtainer
    this function dividing row on 
    a lot of programs, that must to execute
    one by one*/
int ampersend(char ** argvec, char * pwd){
    int flag = 0;
    for(int k = 0; argvec[k] != NULL; k++){
       if(strcmp(argvec[k], "&&") == 0){
           flag = 1;
       } 
    }

    if(flag == 0){
        return 1;
    }
    char ** list = NULL;
    int j = 0;
    for(int i = 0; argvec[i] != NULL; i++){
        if(strcmp(argvec[i], "&&") != 0){
            list = (char **)realloc(list, sizeof(char *) * (j + 1));
            list[j] = argvec[i];
            j++;
        }
        if(strcmp(argvec[i], "&&") == 0){
            list[j]  = NULL;
            j = 0;
            if(fork() == 0){
                gpid = getpid();
                if(execvp(list[0], list) < 0){
                    perror("execute error");
                }
                return 0;
            }
            int wstatus;
            wait(&wstatus);
//            free_list(list);
            //printf("%d\n", WEXITSTATUS(wstatus));
        }
    }
    list[j] = NULL;
    j = 0;
    if(fork() == 0){
        gpid = getpid();
        if(execvp(list[0], list) < 0){
            perror("execute error");
        }
    }
    int wstatus;
    wait(&wstatus);
    //printf("%d\n", WEXITSTATUS(wstatus));
    return 0;
}

//realizing "|| " container
//if one program executes another aren't execute
int or_container(char ** argvec, char * pwd){
    int flag = 0;
    for(int k = 0; argvec[k] != NULL; k++){
       if(strcmp(argvec[k], "||") == 0){
           flag = 1;
       } 
    }

    if(flag == 0){
        return 1;
    }
    char ** list = NULL;
    int j = 0;
    for(int i = 0; argvec[i] != NULL; i++){
        if(strcmp(argvec[i], "||") != 0){
            list = (char **)realloc(list, sizeof(char *) * (j + 1));
            list[j] = argvec[i];
            j++;
        }
        if(strcmp(argvec[i], "||") == 0){
            list[j]  = NULL;
            j = 0;
            if (fork() == 0){
                gpid = getpid();
                if(execvp(list[0], list) >= 0){
                    return 0;
                }
            }
            int wstatus;
            wait(&wstatus);
            return 0;
//            free_list(list);
            //printf("%d\n", WEXITSTATUS(wstatus));
        }
    }
    list[j] = NULL;
    j = 0;
    if(fork() == 0){
        gpid = getpid();
        if(execvp(list[0], list) >= 0){
            return 0;
        }
    }
    int wstatus;
    wait(&wstatus);
    //printf("%d\n", WEXITSTATUS(wstatus));
    return 0;
}

/*this function checks on existense
  of containers, redirects, pipes, etc.
  and executes the row that been keep in argvec*/
int complete_exec(char ** argvec, char * pwd){
    if(argvec != NULL){
        char * cmd = argvec[0];
        if (io_pipes(argvec) == 1){
            pipes(argvec);
        }
        if (!ampersend(argvec, pwd)){
            return 0;
        }
        if(!or_container(argvec, pwd)){
            return 0;
        }
         if(io_direct_left(argvec)){
            direct_to_left(argvec);
            return 0;
        }
        if(io_direct_right(argvec)){
            direct_to_right(argvec);
            return 0;
        }
        
        if (strcmp(argvec[0], "cd") == 0){
            pwd = cd_func(argvec, pwd);
            return 0;
        }
       
        if(argvec != NULL && ((strcmp(cmd, "exit") == 0) || 
          (strcmp(cmd, "quit") == 0))){
                free_list(argvec);
                return 1;
        }
        pid_t pid = fork();
        gpid = pid;
        if ((pid == 0) && (io_pipes(argvec) != 1) && (argvec != NULL) && (io_direct_left(argvec) != 1) && (io_direct_right(argvec) != 1)){
            if (execvp(cmd, argvec) < 0){
                perror("No such command");
                return 0;
                if (pid == 0){
                    return 1;
                }
            }
        }
        if (pid == 0){
            return 1;
        }
        free_list(argvec);
    
    }
    return 0;
}


/*this function implements background mode of programs
  and prints his number and pid                       */
int background_mode(char ** argvec, char * pwd, int k){
    int i = 0;
    for(i = 0; argvec[i] != NULL; i++);
    if (strcmp(argvec[i - 1], "&") == 0)
    {
        argvec[i - 1] = NULL;
        if(fork() == 0){
            printf("[%d] %d\n", k, getpid());
            if (execvp(argvec[0], argvec) < 0){
                perror("No such command");
                return k;
            }
            complete_exec(argvec, pwd);
        }
        k++;
        return k;
    }
    return k;
}

/* handler, that processing interrupts*/
void handler(int signo){
    kill(gpid, SIGINT);
    printf("\n");
}

//this function executes commands endlessly and cheks existence of background mode
int infinity(){
    char ** argvec = NULL;
    char * pwd = getenv("PWD");
    int k = 1;
    signal(SIGINT, handler);
    while(1){
        print_greetings(pwd);
        argvec = get_list();
        if(argvec != NULL){
                k = background_mode(argvec, pwd, k);
                if(complete_exec(argvec, pwd) == 1){
                    return 1;
                }
            }
        }        
    }

//this is main:)
int main(int argc, char ** argv){
    infinity();
    return 0;
}