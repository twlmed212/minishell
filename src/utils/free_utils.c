/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:25:21 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/02 18:25:26 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_cmd(t_cmd *cmd)
{
	t_redir	*redir;
	t_redir	*next;

	if (!cmd)
		return ;
	if (cmd->args)
		free(cmd->args);
	redir = cmd->redirs;
	while (redir)
	{
		next = redir->next;
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = next;
	}
	free(cmd);
}
