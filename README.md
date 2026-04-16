*This project has been created as part of the 42 curriculum by nildruon*

# Description

Pipex

Funcs:
open, close, read, write,
malloc, free, perror,
strerror, access, dup, dup2,
execve, exit, fork, pipe,
unlink, wait, waitpid


// f[0] read
// f[1] write
/* t_pipex	pip;
	int input = open("input", O_RDONLY | O_CREAT | O_TRUNC);
	int output = open("output", O_WRONLY | O_CREAT | O_TRUNC);
	pipe(pip.fds);
	pip.child1 = fork();
	printf("child 1 pipe read: %d\n", pip.fds[0]);
		printf("child 1 pipe write: %d\n", pip.fds[1]);
	if(pip.child1 == 0)
	{
		close(pip.fds[0]);
		dup2(pip.fds[1], STDOUT_FILENO);
		execve("/bin/ls", &argv[1], envp);
		close(pip.fds[1]);
		return (0);
	}
	pip.child2 = fork();
	if(pip.child2 == 0)
	{
		close(pip.fds[1]);
		dup2(pip.fds[0], STDIN_FILENO);
		execve("/bin/ls", &argv[1], envp);
		close(pip.fds[0]);
		return (0);
	}
	close(pip.fds[0]);
	close(pip.fds[1]);
	printf("Parent: \n");
	return (0); */

126 Permission Denied (Found but not executable)
127 Command Not Found
# Instructions



# Rescources:

