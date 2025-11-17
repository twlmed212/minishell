#include "../../include/minishell.h"

int find_redir(char **args)
{
    int i;
    
    i = 0;
    int len_arg;
    while (args[i])
    {
        len_arg = ft_strlen(args[i]);
        if (ft_strncmp(args[i], ">", len_arg) == 0)
            return (i);
        if (ft_strncmp(args[i], ">>", len_arg) == 0)
            return (i);
        if (ft_strncmp(args[i], "<", len_arg) == 0)
            return (i);
        if (ft_strncmp(args[i], "<<", len_arg) == 0)
            return (i);
        i++;
    }
    return (-1);
}

t_redir_type get_redir_type(char *op)
{
    int len_op = ft_strlen(op);
    if (ft_strncmp(op, ">", len_op) == 0)
        return (REDIR_OUT);
    if (ft_strncmp(op, ">>", len_op) == 0)
        return (REDIR_APPEND);
    if (ft_strncmp(op, "<", len_op) == 0)
        return (REDIR_IN);
    if (ft_strncmp(op, "<<", len_op) == 0)
        return (REDIR_HEREDOC);
    return (REDIR_OUT);
}

t_cmd *parse_cmd_with_redir(char **args)
{
    t_cmd   *cmd;
    t_redir *redir;
    int     redir_pos;
    int     i;
    
    cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    
    redir_pos = find_redir(args);
    
    if (redir_pos == -1)
    {
        cmd->args = args;
        cmd->redirs = NULL;
        return (cmd);
    }
    
    redir = malloc(sizeof(t_redir));
    if (!redir)
    {
        free(cmd);
        return (NULL);
    }
    
    redir->type = get_redir_type(args[redir_pos]);
    redir->file = ft_strdup(args[redir_pos + 1]);
    redir->next = NULL;
    
    cmd->redirs = redir;
    
    cmd->args = malloc(sizeof(char *) * (redir_pos + 1));
    if (!cmd->args)
    {
        free(redir->file);
        free(redir);
        free(cmd);
        return (NULL);
    }
    
    i = 0;
    while (i < redir_pos)
    {
        cmd->args[i] = args[i];
        i++;
    }
    cmd->args[i] = NULL;
    
    return (cmd);
}

void free_cmd(t_cmd *cmd)
{
    t_redir *redir;
    t_redir *next;
    
    if (!cmd)
        return;
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