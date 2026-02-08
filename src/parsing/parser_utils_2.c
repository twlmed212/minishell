/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:59:24 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/18 03:48:25 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redir			*new_redir(t_token_type type, char *file);

t_redir	*process_redir(t_token *token)
{
	char	*file;
	t_redir	*redir;

	if (token->type == T_HEREDOC)
	{
		file = handle_heredoc(token->next->value);
		if (!file)
		{
			return (NULL);
		}
		redir = new_redir(token->type, file);
		return (redir);
	}
	if (token->type == T_REDIR_IN && token->next)
	{
		if (access(token->next->value, F_OK) != 0)
		{
			get_and_set_value(NULL, 1);
		}
	}
	return (new_redir(token->type, token->next->value));
}

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
		cmds = res;
	}
	return (cmds);
}

int	handle_pipe_token(t_token **tokens, int flag)
{
	if ((!(*tokens)->next && !flag) || ((*tokens)->type == T_PIPE && flag))
	{
		ft_perror("minishell: syntax error near unexpected token");
		if ((*tokens)->next && (*tokens)->next->type == T_PIPE)
			ft_perror(" `||'\n");
		else
			ft_perror(" `|'\n");
		return (0);
	}
	if (!flag)
		*tokens = (*tokens)->next;
	return (1);
}

int	check_syntax(t_token *token)
{
	if ((!token->next || token->next->type != T_WORD))
	{
		ft_perror("minishell: syntax error near unexpected token `");
		if (token->next)
			ft_perror(token->next->value);
		else
			ft_perror("newline");
		ft_perror("`\n");
		return (0);
	}
	return (1);
}
