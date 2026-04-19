/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 17:20:01 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/19 18:17:12 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_the_split(char **data)
{
	int	i;

	i = 0;
	if (!data)
		return ;
	while (data[i])
	{
		free(data[i]);
		i++;
	}
	free(data);
}
