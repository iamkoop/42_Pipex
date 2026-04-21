/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 17:14:44 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/04/21 12:14:22 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char	**argv, char **envp)
{
	t_pipex	pip;

	pip = (t_pipex){0};
	if (argc != 5)
		return (perror("Wrong amount of args"), 1);
	pip.argv = argv;
	pip.envp = envp;
	return (parent(&pip));
}
