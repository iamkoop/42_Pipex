/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 18:33:38 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/19 18:33:41 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include "libft_lib/libft.h"

typedef struct s_pipex
{
	int	fds[2];
	int	child1;
	int	child2;
}	t_pipex;
//MAIN
int		main(int argc, char	**argv, char **envp);

//ACCESS
char	*get_path(char	*cmd, char **envp, int	*exit_status);

//free
void	free_the_split(char **data);

//error handler
char	*malloc_fail_handler(char	*error_msg, int	*exit_status);

#endif