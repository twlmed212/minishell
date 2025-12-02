/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 23:20:48 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/02 18:30:22 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	find_redir(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0)
			return (i);
		if (ft_strcmp(args[i], ">>") == 0)
			return (i);
		if (ft_strcmp(args[i], "<") == 0)
			return (i);
		if (ft_strcmp(args[i], "<<") == 0)
			return (i);
		i++;
	}
	return (-1);
}

t_redir_type	get_redir_type(char *op)
{
	if (ft_strcmp(op, ">") == 0)
		return (REDIR_OUT);
	if (ft_strcmp(op, ">>") == 0)
		return (REDIR_APPEND);
	if (ft_strcmp(op, "<") == 0)
		return (REDIR_IN);
	if (ft_strcmp(op, "<<") == 0)
		return (REDIR_HEREDOC);
	return (REDIR_OUT);
}

int	pars_args_without_redi(char **args, t_cmd *cmd)
{
	int	i;
	int	cmd_arg_count;

	cmd_arg_count = 0;
	i = 0;
	cmd->redirs = NULL;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0
			|| ft_strcmp(args[i], "<") == 0 || ft_strcmp(args[i], "<<") == 0)
			i += 2;
		else
		{
			cmd_arg_count++;
			i++;
		}
	}
	cmd->args = malloc(sizeof(char *) * (cmd_arg_count + 1));
	if (!cmd->args)
	{
		free(cmd);
		return (-1);
	}
	return (0);
}

int	extract_redirection(char **args, int *i, t_cmd *cmd)
{
	t_redir			*redir;
	static t_redir	*last_redir;

	if (ft_strcmp(args[*i], ">") == 0 || ft_strcmp(args[*i], ">>") == 0
		|| ft_strcmp(args[*i], "<") == 0 || ft_strcmp(args[*i], "<<") == 0)
	{
		redir = malloc(sizeof(t_redir));
		if (!redir)
		{
			free_cmd(cmd);
			return (-1);
		}
		redir->type = get_redir_type(args[*i]);
		redir->file = ft_strdup(args[*i + 1]);
		redir->next = NULL;
		if (!cmd->redirs)
			cmd->redirs = redir;
		else
			last_redir->next = redir;
		last_redir = redir;
		*i += 2;
		return (1);
	}
	return (0);
}

t_cmd	*parse_cmd_with_redir(char **args)
{
	t_cmd	*cmd;
	int		i;
	int		j;
	int		status;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	if (pars_args_without_redi(args, cmd) == -1)
		return (NULL);
	i = 0;
	j = 0;
	while (args[i])
	{
		status = extract_redirection(args, &i, cmd);
		if (status == -1)
			return (NULL);
		else if (status == 1)
			continue ;
		else
			cmd->args[j++] = args[i++];
	}
	cmd->args[j] = NULL;
	return (cmd);
}
