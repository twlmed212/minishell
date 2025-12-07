/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_descriptor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:48:26 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/07 16:48:27 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

int	*save_std_fds(void)
{
	int	*saved;

	saved = malloc(sizeof(int) * 2);
	if (!saved)
		return (NULL);
	saved[0] = dup(STDIN_FILENO);
	saved[1] = dup(STDOUT_FILENO);
	return (saved);
}

void	restore_std_fds(int *saved)
{
	if (!saved)
		return ;
	dup2(saved[0], STDIN_FILENO);
	dup2(saved[1], STDOUT_FILENO);
	close(saved[0]);
	close(saved[1]);
	free(saved);
}