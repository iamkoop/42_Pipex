/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_processes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 19:44:05 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/23 17:34:09 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	second_child(t_pipex *pip)
{
	close(pip->fds[1]);
	if (!pip->argv[3])
		malloc_fail_handler("command not found", NULL, 1);
	pip->dupe1 = dup2(pip->fds[0], STDIN_FILENO);
	if (pip->dupe1 == -1)
		child_processes_handler(pip, "first dup2 in child 2", 2);
	close(pip->fds[0]);
	pip->outfile = open(pip->argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pip->outfile == -1)
		child_processes_handler(pip, pip->argv[4], 2);
	pip->dupe2 = dup2(pip->outfile, STDOUT_FILENO);
	if (pip->dupe2 == -1)
		child_processes_handler(pip, "second dup2 in child 2", 2);
	pip->cmd_and_args = ft_split(pip->argv[3], ' ');
	if (!pip->cmd_and_args)
		child_processes_handler(pip, "split failed", 2);
	pip->path = get_path(pip, pip->cmd_and_args[0]);
	if (!pip->path || !*pip->path)
		child_processes_handler(pip, "", 2);
	close(pip->outfile);
	execve(pip->path, pip->cmd_and_args, pip->envp);
	free(pip->path);
	perror(pip->cmd_and_args[0]);
	free_the_split(pip->cmd_and_args);
	exit(1);
}

static void	first_child(t_pipex *pip)
{
	close(pip->fds[0]);
	if (!pip->argv[2])
		malloc_fail_handler("command not found", NULL, 1);
	pip->infile = open(pip->argv[1], O_RDONLY);
	if (pip->infile == -1)
		child_processes_handler(pip, pip->argv[1], 1);
	pip->dupe1 = dup2(pip->infile, STDIN_FILENO);
	if (pip->dupe1 == -1)
		child_processes_handler(pip, "first dup2 in child 1", 1);
	pip->dupe2 = dup2(pip->fds[1], STDOUT_FILENO);
	if (pip->dupe2 == -1)
		child_processes_handler(pip, "second dup2 in child 1", 1);
	close(pip->fds[1]);
	pip->cmd_and_args = ft_split(pip->argv[2], ' ');
	if (!pip->cmd_and_args)
		child_processes_handler(pip, "split failed", 1);
	pip->path = get_path(pip, pip->cmd_and_args[0]);
	if (!pip->path)
		child_processes_handler(pip, "", 1);
	close(pip->infile);
	execve(pip->path, pip->cmd_and_args, pip->envp);
	free(pip->path);
	perror(pip->cmd_and_args[0]);
	free_the_split(pip->cmd_and_args);
	exit(1);
}

int	parent(t_pipex *pip)
{
	int		status;

	status = 0;
	if (pipe(pip->fds) == -1)
		exit(3);
	pip->child1 = fork();
	if (pip->child1 == -1)
		exit(1);
	if (pip->child1 == 0)
		first_child(pip);
	pip->child2 = fork();
	if (pip->child2 == -1)
		exit(2);
	if (pip->child2 == 0)
		second_child(pip);
	close(pip->fds[0]);
	close(pip->fds[1]);
	waitpid(pip->child1, NULL, 0);
	waitpid(pip->child2, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
}
