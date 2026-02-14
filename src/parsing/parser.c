/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 15:26:27 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/14 16:44:36 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*alloc_cmd(void)
{
	t_cmd	*cmd;

	cmd = ft_malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

static void	add_cmd(t_cmd **cmd_list, t_cmd *new_cmd)
{
	t_cmd	*tmp;

	if (!*cmd_list)
	{
		*cmd_list = new_cmd;
		return ;
	}
	tmp = *cmd_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_cmd;
}

static int	add_arg(t_cmd *cmd, char *arg)
{
	char	**new_args;
	int		count;
	int		i;

	count = 0;
	while (cmd->args && cmd->args[count])
		count++;
	new_args = ft_malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (0);
	i = 0;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = arg;
	new_args[count + 1] = NULL;
	cmd->args = new_args;
	return (1);
}

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
	t_shell	*shell;


	shell = get_and_set_value(NULL, -1);
	i = 0;
	while (str[i])
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
			if (!word || (word && word[0] == '\0'))
			{
				ft_perror("Command not found\n");
				shell->exit_code = 127;
				continue;;
			}
			if (word && !add_arg(*current, word))
				return (NULL);
		}
	}
	return (add_cmd(cmd_list, *current), *cmd_list);
}
