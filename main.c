/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 17:14:44 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/04/20 01:03:27 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int	main(int argc, char	**argv,  char **envp)
{
	t_pipex pip;
	int		exit_status;

	pip = (t_pipex){0};
	if(argc != 5)
		return(perror("Wrong amount of args"), 1);
	exit_status = 0;
	pip.argv = argv;
	pip.envp = envp;
	pip.exit_status = exit_status;
	return  (parent(&pip));
}
