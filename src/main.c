/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 02:45:27 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/25 22:09:22 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void rl_clear_history(void);
int main(int ac, char **av, char **env)
{
    char *input;
    t_env_and_exit shell;

    (void)ac;
    (void)av;
    
    shell.env = copy_env(env);
    if (!shell.env)
    {
        printf("Error: Failed to copy environment\n");
        return (1);
    }
    
    init_signals();

    while (1)
    {
        g_signal = 0;
        
        input = readline("minishell> ");
        
        if (!input)
        {
            printf("exit");
            break;
        }
        
        if (g_signal == SIGINT)
        {
            g_signal = 0;
            free(input);
            continue;
        }
        
        if (!input[0])
        {
            free(input);
            continue;
        }
        
        if (check_unclosed_quotes(input) == -1)
        {
            printf("error: unclosed quotes\n");
            free(input);
            continue;
        }
        
        execute_command(input, &shell);
        
        add_history(input);
        free(input);
    }
    
    free_array(shell.env);
    rl_clear_history();
    return (0);
}
