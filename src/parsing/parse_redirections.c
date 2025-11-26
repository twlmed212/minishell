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
t_cmd *parse_cmd_with_redir(char **args)
{
    t_cmd *cmd;
    t_redir *redir;
    t_redir *last_redir;
    int i;
    int j;
    int cmd_arg_count;

    cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    
    cmd->redirs = NULL;
    last_redir = NULL;
    
    // Count non-redirection arguments
    cmd_arg_count = 0;
    i = 0;
    while (args[i])
    {
        if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0 ||
            ft_strcmp(args[i], "<") == 0 || ft_strcmp(args[i], "<<") == 0)
        {
            i += 2;  // Skip operator and filename
        }
        else
        {
            cmd_arg_count++;
            i++;
        }
    }
    
    // ALWAYS allocate a NEW array for cmd->args
    cmd->args = malloc(sizeof(char *) * (cmd_arg_count + 1));
    if (!cmd->args)
    {
        free(cmd);
        return (NULL);
    }
    
    // Parse and copy
    i = 0;
    j = 0;
    while (args[i])
    {
        if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0 ||
            ft_strcmp(args[i], "<") == 0 || ft_strcmp(args[i], "<<") == 0)
        {
            // Create redirection
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
            // Copy pointer (NOT duplicate string!)
            cmd->args[j] = args[i];
            j++;
            i++;
        }
    }
    cmd->args[j] = NULL;
    
    return (cmd);
}
void free_cmd(t_cmd *cmd)
{
    t_redir *redir;
    t_redir *next;

    if (!cmd)
        return;
    
    // Free ONLY the args array, NOT the strings inside!
    // The strings are owned by the original args array
    if (cmd->args)
        free(cmd->args);
    // Free redirections (these ARE duplicated)
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