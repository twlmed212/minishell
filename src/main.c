#include "../include/minishell.h"

int main(int ac, char **av, char **env)
{
    char *input;
    char **args;

    (void) av;
    (void) ac;
    

    while(1){
        input = readline("minishell> ");
        if (!input)
        {
            printf("exit\n");
            break;
        }
        if (*input == '\0')
        {
            free(input);
            continue;
        }
        args = parse_command(input);
        if (!args || !args[0])
        {
            free_array(args);
            continue;
        }
        if (is_builtin(input)){
            execute_builtin(args, env);
        }else{
            execute_command(args, env);
        }
        add_history(input);
        free(input);
    }

    return 0;
}