/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:59:24 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/07 16:47:05 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd	*new_cmd(void);
t_redir	*new_redir(t_token_type type, char *file);
void	add_redir(t_redir **head, t_redir *new);
int		count_args(t_token *tokens);

char	*expand_exit_code(char *cmds)
{
	char	*tmp;
	char	*res;
	t_shell	*shell;

	shell = get_and_set_value(NULL, -1);
	if (ft_strncmp(cmds, "$?", 2) == 0)
	{
		tmp = ft_itoa(shell->exit_code);
		res = ft_strjoin(tmp, cmds + 2);
		free(cmds);
		cmds = res;
		free(tmp);
	}
	return (cmds);
}

static int	check_syntax(t_token *token)
{
	if ((!token->next || token->next->type != T_WORD))
	{
		printf("minishell: syntax error near unexpected token `%s'\n",
			token->next ? token->next->value : "newline");
		return (0);
	}
	return (1);
}

static t_redir	*process_redir(t_token *token)
{
	char	*file;
	t_redir	*redir;

	if (token->type == T_HEREDOC)
	{
		file = handle_heredoc(token->next->value);
		if (!file)
			return (NULL);
		redir = new_redir(token->type, file);
		free(file);
		return (redir);
	}
	return (new_redir(token->type, token->next->value));
}

static t_token	*fill_cmd(t_cmd *cmd, t_token *tokens, int *err)
{
	int		i;
	int		argc;
	t_redir	*redir;

	*err = 0;
	argc = count_args(tokens);
	cmd->args = malloc(sizeof(char *) * (argc + 1));
	if (!cmd->args)
		return (NULL);
	i = 0;
	while (tokens && tokens->type != T_PIPE)
	{
		if (tokens->type == T_WORD)
		{
			tokens->value = expand_exit_code(tokens->value);
			cmd->args[i++] = remove_quotes(tokens->value);
		}
		else if (tokens->type >= T_REDIR_IN)
		{
			if (!check_syntax(tokens))
				return (cmd->args[i] = NULL, *err = 1, NULL);
			redir = process_redir(tokens);
			if (!redir)
				return (cmd->args[i] = NULL, *err = 1, NULL);
			add_redir(&cmd->redirs, redir);
			tokens = tokens->next;
		}
		
		tokens = tokens->next;
	}
	cmd->args[i] = NULL;
	return (tokens);
}

t_cmd	*parser(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*current;
	t_cmd	*new;
	int		err;

	cmds = NULL;
	current = NULL;
	while (tokens)
	{
		new = new_cmd();
		if (!new)
			return (free_cmds(cmds), NULL);
		tokens = fill_cmd(new, tokens, &err);
		if (err)
		{
			free_array(new->args);
			free(new);
			return (free_cmds(cmds), NULL);
		}
		if (!cmds)
			cmds = new;
		else
			current->next = new;
		current = new;
		if (tokens && tokens->type == T_PIPE)
		{
			if (!tokens->next)
			{
				printf("minishell: syntax error near unexpected token `|'\n");
				return (free_cmds(cmds), NULL);
			}
			tokens = tokens->next;
		}
	}
	return (cmds);
}
