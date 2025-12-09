/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 12:20:33 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/09 12:49:18 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_sigint_heredoc(int sig)
{
	t_shell	*shell;

	(void)sig;
	shell = get_and_set_value(NULL, -1);
	write(1, "\n", 1);
	free_array(shell->env);
	exit(130);
}
