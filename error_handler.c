/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 16:27:48 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/20 16:28:43 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*malloc_fail_handler(char	*error_msg, int	*exit_status)
{
	printf("hello from malloc fail handler\n");
	ft_putstr_fd(error_msg, 2);
	*exit_status = 1;
	return (NULL);
}

static	void child2(t_pipex	*pip, char	*error_msg)
{
	printf("Hello from child_processes_handler child 2\n");
	if(pip->dupe1 == -1)	
		close(pip->fds[0]);
	if(pip->dupe2 == -1)
		close(pip->outfile);
	if(!pip->cmd_and_args)
	{
		//pip->exit status check it
		malloc_fail_handler("split failed", &pip->exit_status);
		close(pip->outfile);
		exit(pip->exit_status);
	}
	if(!pip->path)
	{
		close(pip->outfile);
		free_the_split(pip->cmd_and_args);
		exit(pip->exit_status);
	}
	perror(error_msg);
	exit(1);
}

static	void child1(t_pipex	*pip, char	*error_msg)
{
	printf("Hello from child_processes_handler child 1\n");
	if (pip->infile == -1)
			close(pip->fds[1]);
	if (pip->dupe1 == -1)
	{
		close(pip->infile);
		close(pip->fds[1]);
	}
	if (pip->dupe2 == -1)
		close(pip->fds[1]);
	if (!pip->cmd_and_args)
	{
		//pip->exit status check it
		malloc_fail_handler("split failed", &pip->exit_status);
		exit(pip->exit_status);
	}
	if (!pip->path)
	{
		free_the_split(pip->cmd_and_args);
		exit(pip->exit_status);
	}
	perror(error_msg);
	exit(1);
}

void	child_processes_handler(t_pipex	*pip, char	*error_msg, int child)
{
	printf("Hello from child_processes_handler\n");
	if(child == 1)
		child1(pip, error_msg);
	if(child == 2)
		child2(pip, error_msg);
}
