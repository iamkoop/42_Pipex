*This project has been created as part of the 42 curriculum by nildruon*

# Description

This is a project meant to be an introduction into the world of processes and pipes.


In this project we recreate the way a pipe works.

The funcs used:

pipe(): For inter-process communication.

fork(): To create child processes.

dup2(): To redirect standard input/output.

execve(): To execute the commands.

# Instructions

to compile use make.


to run the programm					./pipex cmd1 cmd2 outfile


in bash you would run it like this: < infile cmd1 | cmd2 > outfile

# Rescources:

gemini and to understand the functions that are needed for this project