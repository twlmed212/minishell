#include "../../include/minishell.h"

int check_simple_command(t_tokens *tokens) {
    t_tokens *last = tokens;

    if (!tokens) {
        printf("Empty command.\n");
        return 0; //? exit-status
    }
    if (tokens->type == TOKEN_PIPE) {
        printf("Syntax error: command cannot start with '|'\n");
        return 0;
    }
    while (tokens) {

        if (tokens->type != TOKEN_WORD && tokens->type != TOKEN_PIPE)
        {
            if (!tokens->next) {
                printf("Redirection '%s' missing target file\n", tokens->value);
                return 0;
            }
            if (tokens->next->type != TOKEN_WORD) {
                printf("Redirection '%s' must be followed by a filename\n", tokens->value);
                return 0;
            }
        }
        if (tokens->type == TOKEN_PIPE && (!tokens->next || tokens->next->type == TOKEN_PIPE)) 
        {
            printf("Syntax error: invalid pipe usage\n");
            return 0;
        }
        last = tokens;
        tokens = tokens->next;
    }
    if (last && last->type == TOKEN_PIPE) {
        printf("Syntax error: command cannot end with '|'\n");
        return 0;
    }
    return 1;
}
