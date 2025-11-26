/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:07 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/25 22:08:06 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void free_token_structs(t_tokens *head)
{
    t_tokens *tmp;

	while (head)
    {
        tmp = head->next;
        if (head->value)
            free(head->value);
        free(head);
        head = tmp;
    }
}

char **tokens_to_array(t_tokens *tokens, int size)
{
    char **args;
    int i = 0;
    t_tokens *current = tokens;
    
    args = malloc(sizeof(char *) * (size + 1));
    if (!args)
        return NULL;
    
    while (current)
    {
        args[i] = ft_strdup(current->value);
        if (!args[i])
        {
            while (--i >= 0)
                free(args[i]);
            free(args);
            return NULL;
        }
        i++;
        current = current->next;
    }
    args[i] = NULL;
    return args;
}

void execute_command(char *command, t_env_and_exit *shell)
{
    pid_t pid;
    char **args;
    char *cmd_path;
    t_cmd *cmd;
    int *saved_fds;
    int status;
    int size = 0;
    
    
    t_tokens *tokens = tokenize(command, &size);
    
    if (!tokens)
        return;
    
    if (check_simple_command(tokens) == 0)
    {
        free_token_structs(tokens);
        return;
    }
    
    args = tokens_to_array(tokens, size);
    
    free_token_structs(tokens);
    
    if (!args || !args[0])
    {
        free_array(args);
        return;
    }

    if (has_pipe(args, shell))
    {
        free_array(args);
        return;
    }

    cmd = parse_cmd_with_redir(args);
    if (!cmd)
    {
        free_array(args);
        return;
    }

    if (is_builtin(cmd->args[0]))
    {
        if (ft_strcmp(cmd->args[0], "exit") == 0)
        {
            free_cmd(cmd);
            builtin_exit(args, shell);
        }
        
        saved_fds = save_std_fds();

        if (cmd->redirs)
        {
            if (execute_redirections(cmd->redirs) == -1)
            {
                restore_std_fds(saved_fds);
                free_cmd(cmd);
                free_array(args);
                return;
            }
        }

        run_builtin(cmd->args, shell);

        restore_std_fds(saved_fds);
        free_cmd(cmd);
        free_array(args);
        return;
    }

    cmd_path = find_command_path(cmd->args[0], shell);
    if (!cmd_path)
    {
        printf("minishell: %s: command not found\n", cmd->args[0]);
        free_cmd(cmd);
        free_array(args);
        return;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(cmd_path);
        free_cmd(cmd);
        free_array(args);
        return;
    }

    if (pid == 0)
    {
        reset_signals();
        if (cmd->redirs)
        {
            if (execute_redirections(cmd->redirs) == -1)
                exit(1);
        }
        if (execve(cmd_path, cmd->args, shell->env) == -1)
        {
            perror("minishell");
            exit(126);
        }
    }
    else
    {
        pid_t result;
        
        // Switch to "child execution" signal mode
        // Parent will ignore SIGINT while child runs
        init_signals_child_exec();
        
        result = waitpid(pid, &status, 0);
        
        // Restore interactive signal handling
        init_signals();
        
        if (result > 0)
        {
            if (WIFSIGNALED(status))
            {
                int sig = WTERMSIG(status);
                if (sig == SIGINT)
                {
                    // Child was interrupted - just print newline
                    write(1, "\n", 1);
                }
            }
        }
    }

    free(cmd_path);
    free_cmd(cmd);
    free_array(args);
}