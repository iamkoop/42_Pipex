/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 18:33:38 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/20 16:23:38 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "libft_lib/libft.h"

typedef struct s_pipex
{
	int		fds[2];
	int		child1;
	int		child2;
	char	**argv;
	char	**envp;
	char	**cmd_and_args;
	char	*path;
	int		dupe1;
	int		dupe2;
	int		infile;
	int		outfile;
	int		exit_status;
}	t_pipex;
//MAIN
int		main(int argc, char	**argv, char **envp);

//ACCESS
char	*get_path(t_pipex *pip, char *cmd);

//free
void	free_the_split(char **data);

//error handler
char	*malloc_fail_handler(char	*error_msg, int	*exit_status);
void	child_processes_handler(t_pipex	*pip, char	*error_msg, int child);

int		parent(t_pipex *pip);

#endif