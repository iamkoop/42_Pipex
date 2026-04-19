/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 16:27:48 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/19 18:17:06 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*malloc_fail_handler(char	*error_msg, int	*exit_status)
{
	ft_putstr_fd(error_msg, 2);
	*exit_status = 1;
	return (NULL);
}
