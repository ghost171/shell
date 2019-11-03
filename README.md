##What is a shell.
**A Unix shell is a command-line interpreter or shell
that provides a command line user interface
for Unix-like operating systems.
The shell is both an interactive command language
and a scripting language, and is used by the operating system
to control the execution of the system using shell scripts.**

    Users typically interact with a Unix shell using
    a terminal emulator; however, direct operation via serial
    hardware connections or Secure Shell are common for server systems.
    All Unix shells provide filename wildcarding, piping, here documents,
    command substitution, variables and control structures for
    condition-testing and iteration.

##How you can use it?
    To execute program you can enter "./shell" command.
    To exit from this shell you can enter "exit" or "quit".
    1)You can execute any count of pipes with "|" symbol.
    2)You can execute redirection to left or right with command "<" or ">".
    3)You can move through directories with command cd.
    4)You can interrupt lust executing program with Ctrl+C.
    5)You can execute && or || containers.

##What is the pipes technology?
In Unix-like computer operating systems, a pipeline is a mechanism for inter-process communication using message passing. 
**A pipeline is a set of processes chained together by their standard streams, so that the output text of each process (stdout) is passed directly as input (stdin) to the next one. **
    The first process is not completed before the second is started, but they are executed concurrently. 
    The concept of pipelines was championed by Douglas McIlroy at Unix's ancestral home of Bell Labs, during the development of Unix, shaping its toolbox philosophy. 
    It is named by analogy to a physical pipeline. 
    A key feature of these pipelines is their "hiding of internals" (Ritchie & Thompson, 1974). 
    This in turn allows for more clarity and simplicity in the system.
##What are the redirects?
    In computing, redirection is a form of interprocess communication, and is a function common to most command-line interpreters, 
    including the various Unix shells that can redirect standard streams to user-specified locations.
    Examples:
    grep .c < ls
    ls -la > 1.txt
In Unix-like operating systems, programs do redirection with the dup2(2) system call, or its less-flexible but higher-level stdio analogues.

##How move through directories?
    With command cd. You can write cd and the direcoty you want to move.
    Command cd .. Move you to the parent directory.
    Command cd or cd ~.Move you to the home directory.

##How to use && and || container.
    && conatainer can executes some kind of programs.
    Example:
    pwd && ls -la && date && whoami
    || container helps you with detecting errors in your programs.
    Examples:
    pwd || ls

##version 0.1
    It's the first version of my shell,
    I divided input strings on lexems(get_list(), get_word()) and
    implement execution of simple programs(infinity()).
    Also in pipes(), I realizing pipes through 2 programs.

##version 0.2
    In this version I made my i/o system through 2 programs worked.
    Also, I implemented cd command and greetings row.
    And in the final I realized background processes.

##version 0.5
    version 0.5 I made Interrupts.
    Implemented && and || containers.
    I have got worked redirect system.
    Improved my lexems dividing for input redirects(<, >) without indents.

##version 1.0
In my final version of shell I add n pipes technology.
The next is debugging.

