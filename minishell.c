#include "minishell.h"

int main(int argc, char **argv,char **env) {
    char *line;
    t_tokens *tokens;
    (void)env;
        (void)argc;   
        (void)argv;
    while (1) {
        line = readline("minishell$ ");

        if (line == NULL) {
            printf("exit\n"); //? stderr
            break;
        }

        if (*line != '\0')
            add_history(line);

        if (check_unclosed_quotes(line) == -1)
            printf("error: unclosed quotes\n"); //? stderr

        else
        {   
            tokens = tokenize(line);

            check_simple_command(tokens);
            ft_star_heredoc(tokens);
            // parse_tokens(tokens);
        }
        free(line);
    }
    return 0;
}
