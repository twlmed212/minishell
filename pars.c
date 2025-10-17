#include "minishell.h"

int count_tokens(t_tokens *tokens) {
    int len = 0;
    
    while (tokens) {
        len++;
        if (tokens->type == TOKEN_PIPE)  
            break;
        tokens = tokens->next;
    }
    return len;
}

t_command *new_command(int len) {
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd) return NULL;
    cmd->args = malloc(sizeof(char *) * len); 
    cmd->args[0] = NULL;
    cmd->redirs = NULL;
    cmd->next = NULL;
    return cmd;
}

void print_commands(t_command *cmd_list) {
    int i, j = 0;
    while (cmd_list) {
        printf("Command #%d:\n", j++);
        for (i = 0; cmd_list->args[i]; i++) {
            printf("  arg[%d]: %s\n", i, cmd_list->args[i]);
        }
        t_redirect *redir = cmd_list->redirs;
        while (redir) {
            printf("  redir type %d => file: %s\n", redir->type, redir->filename);
            redir = redir->next;
        }
        cmd_list = cmd_list->next;
    }
}


// ------------ Token Parser ------------

t_command *parse_tokens(t_tokens *tokens) {

    t_command *cmd_list = NULL, *last = NULL;
    int argc = 0;
    while (tokens) {
        t_command *cmd = new_command(count_tokens(tokens));
        argc = 0;
        while (tokens && tokens->type != TOKEN_PIPE) {
            if (tokens->type == TOKEN_WORD) {
                cmd->args[argc] = ft_strdup(tokens->value);
                printf("%s ",cmd->args[argc]);
                argc++;
                tokens = tokens->next;

            }else if (tokens->type == TOKEN_REDIR_IN || tokens->type == TOKEN_REDIR_OUT ||
                     tokens->type == TOKEN_REDIR_APPEND || tokens->type == TOKEN_HEREDOC){

                int redir_type = tokens->type;
                tokens = tokens->next;
                if (!tokens || tokens->type != TOKEN_WORD) {
                    printf("Syntax error: expected filename after redirection\n");
                    return NULL;
                }
                t_redirect *redir = malloc(sizeof(t_redirect));
                if (!redir) {
                    printf("malloc");
                    return NULL;
                }
                redir->type = redir_type;
                redir->filename = ft_strdup(tokens->value);
                redir->next = NULL;

                if (!cmd->redirs)
                    cmd->redirs = redir;
                else {
                    t_redirect *last_redir = cmd->redirs;
                    while (last_redir->next)
                        last_redir = last_redir->next;
                    last_redir->next = redir;
                }
            tokens = tokens->next;
            }
        }
        cmd->args[argc] = NULL;

        if (!cmd_list)
            cmd_list = cmd;
        else
            last->next = cmd;

        last = cmd;
        if (tokens && tokens->type == TOKEN_PIPE)
            tokens = tokens->next;
    }

    print_commands(cmd_list);

    return cmd_list;
}



