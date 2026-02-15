/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 15:26:27 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/15 17:10:33 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*create_redir(t_redir_type type, char *file)
{
	t_redir	*redir;

	redir = ft_malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = file;
	redir->next = NULL;
	return (redir);
}

static void	add_redir(t_cmd *cmd, t_redir *new_redir)
{
	t_redir	*tmp;

	if (!cmd->redirs)
	{
		cmd->redirs = new_redir;
		return ;
	}
	tmp = cmd->redirs;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_redir;
}

static int	handle_redir(char *str, int *i, t_cmd *cmd)
{
	t_redir_type	type;
	char			*filename;
	char			*delimiter;
	t_redir			*redir;

	type = get_redir_type(str, i);
	skip_spaces(str, i);
	filename = extract_word(str, i);
	if (!filename)
		return (1);
	if (type == T_HEREDOC)
	{
		delimiter = filename;
		filename = handle_heredoc(delimiter);
		if (!filename)
			return (1);
	}
	redir = create_redir(type, filename);
	if (!redir)
		return (1);
	add_redir(cmd, redir);
	return (0);
}

static int	handle_pipe(t_cmd **cmd_list, t_cmd **current, int *i)
{
	add_cmd(cmd_list, *current);
	*current = alloc_cmd();
	if (!*current)
		return (1);
	(*i)++;
	return (0);
}

t_cmd	*parser(char *str, t_cmd **cmd_list, t_cmd **current)
{
	char	*word;
	int		i;
	// t_shell	*shell;

	// shell = get_and_set_value(NULL, -1);
	i = 0;
	while (str[i])//echo "hello $USER" | cat -e > out.txt
	{
		skip_spaces(str, &i);
		if (!str[i])
			break ;
		if (str[i] == '|')
		{
			if (handle_pipe(cmd_list, current, &i))
				return (NULL);
		}
		else if (str[i] == '<' || str[i] == '>')
		{
			if (handle_redir(str, &i, *current))
				return (NULL);
		}
		else
		{
			word = extract_word(str, &i);
			//TODO if (!word || (word && word[0] == '\0'))
			// {
			// 	ft_perror("Command not found\n");
			// 	shell->exit_code = 127;
			// 	continue ;
			// }
			if (word && !add_arg(*current, word))
				return (NULL);
		}
	}
	return (add_cmd(cmd_list, *current), *cmd_list);
}
