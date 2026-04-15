/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 17:14:44 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/04/15 21:50:02 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// f[0] read
// f[1] write

int	main()
{
	t_pipex	pip;
	int input = open("input", O_RDONLY | O_CREAT | O_TRUNC);
	int output = open("output", O_WRONLY | O_CREAT | O_TRUNC);
	pipe(pip.fds);
	pip.child1 = fork();
	printf("child 1 pipe read: %d\n", pip.fds[0]);
		printf("child 1 pipe write: %d\n", pip.fds[1]);
	if(pip.child1 == 0)
	{
		close(pip.fds[0]);
		dup2(input, pip.fds[1]);
		execve("/bin/ls", "ls", envp);
		close(pip.fds[1]);
		return (0);
	}
	pip.child2 = fork();
	if(pip.child2 == 0)
	{
		close(pip.fds[1]);
		dup2(pip.fds[0], output);
		execve(argv[2],);
		close(pip.fds[0]);
		return (0);
	}
	close(pip.fds[0]);
	close(pip.fds[1]);
	printf("Parent: \n");
	return (0);
}
