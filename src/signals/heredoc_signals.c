/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 12:20:33 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/12 15:53:44 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint_heredoc(int sig)
{
	int	fd;

	(void)sig;
	fd = get_fd_file(-1);
	write(1, "\n", 1);
	free_grabage();
	close(fd);
	exit(130);
}
