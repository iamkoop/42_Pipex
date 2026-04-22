/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 16:27:48 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/21 20:45:29 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*malloc_fail_handler(char	*error_msg, int	*exit_status, int exite)
{
	ft_putstr_fd(error_msg, 2);
	if (exit_status)
		*exit_status = 1;
	if (exite)
		exit(1);
	return (NULL);
}

static	void	child2(t_pipex	*pip, char	*error_msg)
{
	if (pip->outfile == -1)
		close(pip->fds[0]);
	else if (pip->dupe1 == -1)
		close(pip->fds[0]);
	else if (pip->dupe2 == -1)
		close(pip->outfile);
	else if (!pip->cmd_and_args)
	{
		malloc_fail_handler("split failed\n", &pip->exit_status, 0);
		close(pip->outfile);
		exit(pip->exit_status);
	}
	else if (!pip->path)
	{
		close(pip->outfile);
		free_the_split(pip->cmd_and_args);
		exit(pip->exit_status);
	}
	perror(error_msg);
	exit(1);
}

static	void	child1(t_pipex	*pip, char	*error_msg)
{
	if (pip->infile == -1)
		close(pip->fds[1]);
	else if (pip->dupe1 == -1)
	{
		close(pip->infile);
		close(pip->fds[1]);
	}
	else if (pip->dupe2 == -1)
		close(pip->fds[1]);
	else if (!pip->cmd_and_args)
	{
		malloc_fail_handler("split failed\n", &pip->exit_status, 0);
		exit(pip->exit_status);
	}
	else if (!pip->path)
	{
		free_the_split(pip->cmd_and_args);
		exit(pip->exit_status);
	}
	perror(error_msg);
	exit(1);
}

void	child_processes_handler(t_pipex	*pip, char	*error_msg, int child)
{
	if (child == 1)
		child1(pip, error_msg);
	if (child == 2)
		child2(pip, error_msg);
}
