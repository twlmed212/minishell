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
t_cmd	*parse_cmd_with_redir(char **args)
{
	t_cmd	*cmd;
	t_redir	*redir;
	t_redir	*last_redir;
	int		i;
	int		j;
	int		cmd_arg_count;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->redirs = NULL;
	last_redir = NULL;
	cmd_arg_count = 0;
	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0
			|| ft_strcmp(args[i], "<") == 0 || ft_strcmp(args[i], "<<") == 0)
		{
			i += 2;
		}
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
		return (NULL);
	}
	i = 0;
	j = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0
			|| ft_strcmp(args[i], "<") == 0 || ft_strcmp(args[i], "<<") == 0)
		{
			redir = malloc(sizeof(t_redir));
			if (!redir)
			{
				free_cmd(cmd);
				return (NULL);
			}
			redir->type = get_redir_type(args[i]);
			redir->file = ft_strdup(args[i + 1]);
			redir->next = NULL;
			if (!cmd->redirs)
				cmd->redirs = redir;
			else
				last_redir->next = redir;
			last_redir = redir;
			i += 2;
		}
		else
		{
			cmd->args[j] = args[i];
			j++;
			i++;
		}
	}
	cmd->args[j] = NULL;
	return (cmd);
}

void	free_cmd(t_cmd *cmd)
{
	t_redir *redir;
	t_redir *next;

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