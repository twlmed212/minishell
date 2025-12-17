/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:59:24 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/17 16:45:46 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd			*new_cmd(void);
void			add_redir(t_redir **head, t_redir *new);
int				count_args(t_token *tokens);

static t_token	*process_word_token(t_cmd *cmd, t_token *tokens, int *i)
{
	tokens->value = expand_exit_code(tokens->value);
	cmd->args[(*i)++] = remove_quotes(tokens->value);
	return (tokens->next);
}

static t_token	*process_redir_token(t_cmd *cmd, t_token *tokens, int *err)
{
	t_redir	*redir;

	if (!check_syntax(tokens))
	{
		*err = 1;
		return (NULL);
	}
	redir = process_redir(tokens);
	if (!redir)
	{
		*err = 1;
		return (NULL);
	}
	add_redir(&cmd->redirs, redir);
	return (tokens->next->next);
}

static t_token	*fill_cmd(t_cmd *cmd, t_token *tokens, int *err)
{
	int		i;
	int		argc;

	*err = 0;
	argc = count_args(tokens);
	cmd->args = ft_malloc(sizeof(char *) * (argc + 1));
	if (!cmd->args)
		return (NULL);
	i = 0;
	while (tokens && tokens->type != T_PIPE)
	{
		if (tokens->type == T_WORD)
			tokens = process_word_token(cmd, tokens, &i);
		else if (tokens->type >= T_REDIR_IN)
		{
			tokens = process_redir_token(cmd, tokens, err);
			if (*err)
				return (cmd->args[i] = NULL, NULL);
		}
		else
			tokens = tokens->next;
	}
	return (cmd->args[i] = NULL, tokens);
}

static t_cmd	*process_single_cmd(t_token **tokens, t_cmd *cmds,
		t_cmd **current)
{
	t_cmd	*new;
	int		err;

	new = new_cmd();
	if (!new)
		return (free_cmds(cmds), NULL);
	*tokens = fill_cmd(new, *tokens, &err);
	if (err)
		return (handle_parse_error(cmds, new));
	if (!cmds)
		cmds = new;
	else
		(*current)->next = new;
	*current = new;
	return (cmds);
}

t_cmd	*parser(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*current;

	cmds = NULL;
	current = NULL;
	if (!handle_pipe_token(&tokens, 1))
		return (NULL);
	while (tokens)
	{
		cmds = process_single_cmd(&tokens, cmds, &current);
		if (!cmds)
			return (NULL);
		if (tokens && tokens->type == T_PIPE)
		{
			if (!handle_pipe_token(&tokens, 0))
				return (free_cmds(cmds), NULL);
		}
	}
	return (cmds);
}
