/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 18:33:38 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/23 18:26:11 by nildruon         ###   ########.fr       */
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
# include <errno.h>

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
char	*malloc_fail_handler(char	*error_msg, int	*exit_status, int exite);
void	child_processes_handler(t_pipex	*pip, char	*error_msg, int child);
void	get_path_err_handler(t_pipex	*pip, char	*cmd, int print);

int		parent(t_pipex *pip);

#endif