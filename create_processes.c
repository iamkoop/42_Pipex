/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_processes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 19:44:05 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/20 01:33:21 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void second_child(t_pipex *pip)
{
	char	**cmd_and_args;
	int		outfile;
	char	*path;

	close(pip->fds[1]);
	if(dup2(pip->fds[0], STDIN_FILENO) == -1)
	{
		perror("first dup2 in child 2");
		exit(1);
	}
	close(pip->fds[0]);
	outfile = open(pip->argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(outfile == -1)
	{
		perror("outfile");
		exit(1);
	}
	if(dup2(outfile, STDOUT_FILENO) == -1)
	{
		close(outfile);
		perror("second dup2 in child 2");
		exit(1);
	}
	cmd_and_args = ft_split(pip->argv[3], ' ');
	if(!cmd_and_args)
	{
		//pip->exit status check it
		malloc_fail_handler("split failed", &pip->exit_status);
		close(outfile);
		exit(pip->exit_status);
	}
	path = get_path(pip, cmd_and_args[0]);
	if(!path)
	{
		close(outfile);
		free_the_split(cmd_and_args);
		exit(pip->exit_status);
	}
	if(execve(path, cmd_and_args, pip->envp) == -1)
	{
		free(path);
		close(outfile);
		perror("execve child 2");
		free_the_split(cmd_and_args);
		exit(1);
	}
}

static void first_child(t_pipex *pip)
{
	char	**cmd_and_args;
	int		infile;
	char	*path;

	close(pip->fds[0]);
	infile = open(pip->argv[1], O_RDONLY);
	if(infile == -1)
	{
		close(pip->fds[1]);
		perror("infile");
		exit(1);
	}
	if(dup2(infile, STDIN_FILENO) == -1)
	{
		close(infile);
		close(pip->fds[1]);
		perror(" first dup2 in child 1");
		exit(1);
	}
	close(infile);
	if(dup2(pip->fds[1], STDOUT_FILENO) == -1)
	{
		perror("second dup2 in child 1");
		close(pip->fds[1]);
		exit(1);
	}
	close(pip->fds[1]);
	cmd_and_args = ft_split(pip->argv[2], ' ');
	if(!cmd_and_args)
	{
		//pip->exit status check it
		malloc_fail_handler("split failed", &pip->exit_status);
		exit(pip->exit_status);
	}
	path = get_path(pip, cmd_and_args[0]);
	if(!path)
	{
		free_the_split(cmd_and_args);
		exit(pip->exit_status);
	}
	if(execve(path, cmd_and_args, pip->envp) == -1)
	{
		free(path);
		perror("execve child 1");
		free_the_split(cmd_and_args);
		exit(1);
	}
	exit(0);
}

int parent(t_pipex *pip)
{
	int		status;

	status = 0;
	if(pipe(pip->fds) == -1)
		exit(3);
	pip->child1 = fork();
	if(pip->child1 == -1)
		exit(1);
	if(pip->child1 == 0)
		first_child(pip);
	pip->child2 = fork();
	if(pip->child2 == -1)
		exit(2);
	if(pip->child2 == 0)
		second_child(pip);
	close(pip->fds[0]);
    close(pip->fds[1]);
	waitpid(pip->child1, NULL, 0);
	waitpid(pip->child2, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return(status);
}
