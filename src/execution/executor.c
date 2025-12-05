/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:07 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/05 12:12:55 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_child_process(t_cmd *cmd, char *cmd_path,
		t_env_and_exit *shell)
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

static void	handle_parent_process(pid_t pid, int *status, t_env_and_exit *shell)
{
	pid_t	result;

	init_signals_child_exec();
	result = waitpid(pid, status, 0);
	init_signals();
	if (WIFEXITED(*status))
		shell->last_exit = WEXITSTATUS(*status);
	else
		shell->last_exit = 1;
	if (result > 0 && WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGINT)
			write(1, "\n", 1);
	}
}

static void	execute_external_cmd(t_cmd *cmd, char **args, char *cmd_path,
		t_env_and_exit *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		free_cmd(cmd);
		free_array(args);
		shell->last_exit = 1;
		return ;
	}
	if (pid == 0)
		handle_child_process(cmd, cmd_path, shell);
	else
		handle_parent_process(pid, &status, shell);
	free(cmd_path);
	free_cmd(cmd);
	free_array(args);
}

void	execute_command(char *command, t_env_and_exit *shell)
{
	char	**args;
	char	*cmd_path;
	t_cmd	*cmd;
	int		size;

	size = 0;
	args = parse_and_validate(command, shell, &size);
	if (!args)
		return ;
	if (preprocess_heredocs(args) == -1)
		return ((void)handle_heredoc_failure(args));
	cmd = parse_cmd_with_redir(args);
	if (!cmd)
		return (free_array(args));
	if (!cmd->args[0])
		return (handle_redir_only(cmd, args));
	if (is_builtin(cmd->args[0]))
		return ((void)execute_builtin_cmd(cmd, args, shell));
	cmd_path = find_command_path(cmd->args[0], shell);
	if (!cmd_path)
	{
		print_cmd_error(cmd->args[0], shell, 127);
		return (free_cmd(cmd), free_array(args));
	}
	execute_external_cmd(cmd, args, cmd_path, shell);
}
