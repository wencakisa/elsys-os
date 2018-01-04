//------------------------------------------------------------------------
// NAME: Vencislav Tashev
// CLASS: XIa
// NUMBER: 4
// PROBLEM: #2
// FILE NAME: main.c
// FILE PURPOSE:
// A Unix-like shell command interpreter
// written in C using processes and exec* functions
//------------------------------------------------------------------------

#include "interpreter.h"

#define LINE_PREFIX "$ "

int main() {
    execute_shell(LINE_PREFIX);
}
