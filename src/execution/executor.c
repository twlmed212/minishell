/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:07 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/29 14:44:48 by mtawil           ###   ########.fr       */
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
static void expand_exit_code(char **args, int exit_code){
    int i = 0;
    while(args[i]){
        if (ft_strncmp(args[i], "$?", 2) == 0)
        {
           char *tmp = ft_itoa(exit_code);// 127
           char *res = ft_strjoin(tmp, args[i]+2);
           free(args[i]);
           args[i] = res;
           free(tmp);
        }
        i++;
    }
}
char **tokens_to_array(t_tokens *tokens, int size, int exit_code)
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
    expand_exit_code(args, exit_code);
    return args;
}

// CRITICAL FIX: Preprocess heredocs in args array for non-pipeline commands
static int preprocess_heredocs(char **args)
{
	int i;
	char *temp_file;
	
	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "<<") == 0 && args[i + 1])
		{
			// Read the heredoc NOW, before any execution
			temp_file = read_heredoc(args[i + 1]);
			if (!temp_file)
			{
				// Heredoc was interrupted (Ctrl+D)
				return (-1);
			}
			
			// Replace the delimiter with the temp filename
			// Note: args[i + 1] points to a malloc'd string from tokens_to_array
			free(args[i + 1]);
			args[i + 1] = temp_file;
		}
		i++;
	}
	return (0);
}

static void	handle_redir_only(t_cmd *cmd, char **args)
{
	int	*saved_fds;

	saved_fds = save_std_fds();
	if (cmd->redirs)
		execute_redirections(cmd->redirs);
	restore_std_fds(saved_fds);
	free_cmd(cmd);
	free_array(args);
}


// check the input looking for folder, 
static int look_for_directories(char *args)
{
    struct stat metadata;
    if (args[0] == '/')
       return (-1);
	
	if (stat(args, &metadata) == 0) {
		if (S_ISDIR(metadata.st_mode)) {
			return (1);
		} else if (S_ISREG(metadata.st_mode)) {
			return (2);
		}
	}
    if (args[ft_strlen(args) - 1 ] == '/')
    {
        return (-1);
    }
	return (0);
}
// /x - x/
static int parse_cmd_dirs(char **args, t_env_and_exit *shell)
{
    if (access(args[0], X_OK) == 0)
        return (0);
    if (args[0][0] != '/' && args[0][ft_strlen(args[0]) - 1] != '/')
        return (0);
    
    int stat = look_for_directories(args[0]);
    if (stat == 0)
        return (0);
    ft_perror("minishell: ");
    ft_perror(args[0]);
    if ((ft_strcmp(args[0], "/") == 0) || (ft_strcmp(args[0], "//") == 0) || (ft_strcmp(args[0], "/.") == 0))
        stat = 1;
    if (stat == -1)
    {
        shell->last_exit = 127;
        return (ft_perror(": No such file or directory\n"), -1); 
    }
    else if (stat == 1)
        return (ft_perror(": Is a directory\n"), -1);
    else if (stat == 2)
        return (ft_perror(": Not a directory\n"), -1);
    
    return (0);
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
    args = tokens_to_array(tokens, size, shell->last_exit);
    
    free_token_structs(tokens);
    if (!args || !args[0])
    {
        free_array(args);
        return;
    }
    
    if (parse_cmd_dirs(args, shell) == -1)
    {
        free_array(args);
        return;
    }
    if (has_pipe(args, shell))
    {
        free_array(args);
        return;
    }

	// CRITICAL FIX: Preprocess ALL heredocs BEFORE parsing command
	if (preprocess_heredocs(args) == -1)
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

    if (!cmd->args[0])
	{
		handle_redir_only(cmd, args);
		return ;
	}
    // Handling Arrow in begining, with either file name or Derictory
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
                shell->last_exit = 1;
            }
        }

        shell->last_exit = run_builtin(cmd->args, shell);

        restore_std_fds(saved_fds);
        free_cmd(cmd);
        free_array(args);
        return;
    }
    cmd_path = find_command_path(cmd->args[0], shell);
    if (!cmd_path)
    {
        ft_perror("minishell: ");
        if (!get_env_value("PATH", shell)){
            ft_perror(cmd->args[0]);
            ft_perror(": No such file or directory\n");
        }else if (ft_strcmp(cmd->args[0], "sudo") == 0)
            ft_perror("/usr/bin/sudo: Permission denied\n");
        else{
            ft_perror(cmd->args[0]);
            ft_perror(": command not found\n");
        }
        free_cmd(cmd);
        free_array(args);
        shell->last_exit = 127;
        return;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(cmd_path);
        free_cmd(cmd);
        free_array(args);
        shell->last_exit = 1;
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
        
        if (WIFEXITED(status))
			shell->last_exit = WEXITSTATUS(status);
		else
			shell->last_exit = 1;
            
        if (result > 0)
        {
            if (WIFSIGNALED(status))
            {
                int sig = WTERMSIG(status);
                if (sig == SIGINT)
                {
                    write(1, "\n", 1);
                }
            }
        }
    }

    free(cmd_path);
    free_cmd(cmd);
    free_array(args);
}
