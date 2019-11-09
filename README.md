What is a shell.
----------------
----------------
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

How you can use it?
-------------------
to compile program you can use:

    make -B shell
To execute program you can enter "./shell" command.

    ./shell
To exit from this shell you can enter "exit" or "quit".
1)You can execute any count of pipes with "|" symbol.
2)You can execute redirection to left or right with command "<" or ">".
3)You can move through directories with command cd.
4)You can interrupt lust executing program with Ctrl+C.
5)You can execute && or || containers.

What are the pipes?
------------------------------
In Unix-like computer operating systems, a pipeline is a mechanism for inter-process communication using message passing.   
A pipeline is a set of processes chained together by their standard streams, 
so that the output text of each process (stdout) is passed directly as input (stdin) to the next one. **
The first process is not completed before the second is started, but they are executed concurrently. 
The concept of pipelines was championed by Douglas McIlroy at Unix's ancestral home of Bell Labs, 
during the development of Unix, shaping its toolbox philosophy. 
It is named by analogy to a physical pipeline. .

Examples:
    
    ls | grep .c | sort
    
This program chooses rows with ".c" in result of program ls and sorts their.
As a result we take sorting list of files with type ".c".

What are the redirects?
-----------------------
In computing, redirection is a form of interprocess communication, and is a function common to most command-line          
interpreters, including the various Unix shells that can redirect standard streams to user-specified locations.

Examples:
    
    grep .c < 1.txt
    ls -la > 1.txt
    
First command chooses rows in file "1.txt" with text, ".c" and output it in the standart output thread.
Second command writes the result of ls -la command to the file 1.txt.
In Unix-like operating systems, programs do redirection with the dup2(2) system call, or its less-flexible but higher-level     
stdio analogues.

How move through directories?
-----------------------------
With command cd. You can write cd and the direcoty you want to move.
Command cd .. Move you to the parent directory.
Command cd or cd ~.Move you to the home directory.
 
    cd ..
    cd
    cd ~
    cd Documents
    
How to use && and || container.
-------------------------------
Used to build AND lists, it allows you to run one command only if another exited successfully.
&& conatainer can executes some kind of programs.
    
 Examples:
    
    ls && pwd && date
    
The first command outputs: What in the current directory, where yo are and today's date.
    
|| conatainer can find errors in one of the programs.
    
Exmaples:
    
    asdfsadf || pwd
This command executes pwd, because the first input program  returns error.

Interrupts
----------
In digital computers, an interrupt is an input signal to the processor indicating an event that needs immediate attention. An 
interrupt signal alerts the processor and serves as a request for the processor to interrupt the currently executing code, so 
that the event can be processed in a timely manner. If the request is accepted, the processor responds by suspending its 
current activities, saving its state, and executing a function called an interrupt handler 
(or an interrupt service routine, ISR) 
to deal with the event. This interruption is temporary, and, unless the interrupt indicates a fatal error, the processor 
resumes normal activities after the interrupt handler finishes.

Interrupts are commonly used by hardware devices to indicate electronic or physical state changes that require attention. 
Interrupts are also commonly used to implement computer multitasking, especially in real-time computing. Systems that use 
interrupts in these ways are said to be interrupt-driven.
    
    You can use interrupts with buttons combination Ctrl+C.

How to compile programs:
------------------------
You can compile program with command gcc:
Example:
    
    gcc (Name_of_program).c -o (Name_of_program) -Wall -Werror
How to execute programs:
-----------------------
You can execute program with  "./" prefix:
    ./shell

Version 0.1
------------
It's the first version of shell,
Raw divided on input strings - lexems(get_list(), get_word()) and
implement execution of simple programs(infinity()).
Also in pipes(), I realized pipes through 2 programs.

Version 0.2
------------
In this version i/o system through 2 programs became worked.
Also, was mplemented cd command and greetings row.
And in the final develper realized background processes.

Version 0.5
-------------
In version 0.5 was contibuted Interrupts.
Implemented && and || containers.
This version have got worked redirect system.
Improved lexems dividing for input redirects(<, >) without indents.

Version 1.0
-----------
Final version of shell has got n pipes.

