/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 17:59:46 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/17 01:11:52 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*colon_edge_case(char *env)
{
	int i;
	int len;
	char	*joined;

	i = 0;
	len = ft_strlen(env);
	joined = NULL;
	while (env[i])
	{
		if (env[0] == ':' || env[len -1] == ':' || 
			(env[i + 1] && ft_strncmp(&env[i], "::", 2 == 0)))
		{
			joined = ft_strjoin("./", env);
			if (!joined)
				return (ft_putstr_fd("join err for curr dir case\n", 2), NULL);
			if (!access(joined, F_OK))
			{
				if (!access(joined, X_OK))
					return(joined);
			}
		}
		i++;
	}
	if(joined)
		free(joined);
	return (NULL);
}

static char	*find_exacutable(char *cmd, char *env)
{
	char 	*command;
	char 	**splitted;
	int		i;

	command = colon_edge_case(env);
	if(command)
		return(command);
	splitted = ft_split(env, ':');
	if(!splitted)
		return (ft_putstr_fd("split alloc fail: find_exacutable\n", 2), NULL);
	i = 0;
	while (splitted[i])
	{
		command = ft_strjoin(splitted[i], "/");
		if(!command)
			return(ft_putstr_fd("cmd alloc fail: find_exacutable\n", 2), NULL);
		command = ft_strjoin(command, cmd);
		if(!command)
			return(ft_putstr_fd("cmd alloc fail: find_exacutable\n", 2), NULL);
		if (!access(command, F_OK))
		{
			if (!access(command, X_OK))
				return(command);
		}
		free(command);
		i++;
	}
	return (ft_putstr_fd("no executable found\n", 2), NULL);
}

static char	*check_if_path(char *cmd)
{
	if(ft_strchr(cmd, '/'))
	{
		if(!access(cmd, F_OK))
		{
			if(!access(cmd, X_OK))
				return(cmd);
			else
				exit((perror("access"), 126)); // TODO: change this for when i have process
		}
	}
	return(NULL);
}

char *get_path(char	*cmd, char **envp)
{
	int i;
	char *path;
	if(!envp || !*envp)
		return(perror("Envp not found"), NULL);
	i = 0;
	if(check_if_path(cmd))
		return(cmd);
	while(envp[i])
	{
		if(ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path =find_exacutable(cmd, envp[i] + 5);
			if(!path)
				return(NULL);
			return(path);
		}
		i++;
	}
	return(NULL);
}
