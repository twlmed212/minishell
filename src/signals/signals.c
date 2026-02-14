/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:24 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/14 14:26:02 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

void	restore_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(2, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
void	handle_sigquit(int sig)
{
	(void)sig;
	g_signal = SIGQUIT;
	rl_redisplay();
}
void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

void	disable_parent_signals(void)
{
	struct sigaction	sa_sign;

	sa_sign.sa_handler = SIG_IGN;
	sigemptyset(&sa_sign.sa_mask);
	sa_sign.sa_flags = 0;
	sigaction(SIGINT, &sa_sign, NULL);
	sigaction(SIGQUIT, &sa_sign, NULL);
}
