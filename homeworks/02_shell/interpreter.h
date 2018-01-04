//------------------------------------------------------------------------
// FILE NAME: interpreter.h
// FILE PURPOSE:
// Interpreter of given shell commands / executes lines from stdin
//------------------------------------------------------------------------

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#define READ_END 0
#define WRITE_END 1

#define BUFF_SIZE 4096

//------------------------------------------------------------------------
// FUNCTION: run_command
// The main functionality of the program
// Runs a given command array of strings ( e.g. {"ls", "-la"} ) by:
// -> creating a process ( fork() ),
// -> executing the already parsed command in child process ( exec*() )
// PARAMETERS:
// char** command - The command line to be executed
//------------------------------------------------------------------------
void run_command(char** command);
//------------------------------------------------------------------------
// FUNCTION: close_pipe
// Closes completely a given pipe
// PARAMETERS:
// int pipefd[2] - The pipe file R/W descriptors
//------------------------------------------------------------------------
void close_pipe(int pipefd[2]);
//------------------------------------------------------------------------
// FUNCTION: run_piped_commands
// Runs piped commands
// PARAMETERS:
// char** current - The current command line to be executed
// char** next    - The next command line to be executed after the current one
//------------------------------------------------------------------------
void run_piped_commands(char** current, char** next);
//------------------------------------------------------------------------
// FUNCTION: execute_shell
// Reads commands from stdin and executes them
// with the already implemented functions
// PARAMETERS:
// const char* prefix - The string to be printed in front of every read prompt
//------------------------------------------------------------------------
void execute_shell(const char* prefix);

#endif /* end of include guard: INTERPRETER_H_ */
