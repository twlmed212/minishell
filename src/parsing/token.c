#include "../../include/minishell.h"

t_tokens *create_token(char *str, int fla) {
    t_tokens *new = malloc(sizeof(t_tokens));
    if (!new) return NULL;
    char *tmp = remove_quotes(str);
    new->value = tmp;
    free(str);
    new->flag  = fla;
    new->next = NULL;
    return new;
}

void add_token(t_tokens **head, t_tokens *new_token) {
    if (!*head)
        *head = new_token;
    else {
        t_tokens *tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_token;
    }
}

void print_tokens(t_tokens *head) {
    t_tokens *current = head;
    while (current) {
        printf("(type=%s)\n", current->value);
        printf("(type=%d)\n", current->flag);    
        current = current->next;
    }
}


int is_operator_start(char c) {
    return (c == '>' || c == '<' || c == '|');
}

char *get_operator_token(const char *line, int *i) {
    if ((line[*i] == '>' && line[*i + 1] == '>') ||
        (line[*i] == '<' && line[*i + 1] == '<')) {
        char *token = ft_strndup(line + *i, 2);
        *i += 2;
        return token;
    }
    char *token = ft_strndup(line + *i, 1);
    (*i)++;
    return token;
}

char *get_quoted_token(const char *line, int *i, int *fla) {
    char quote = line[*i];

    if (quote == '"')
        *fla = 2;
    else if (quote == '\'')
        *fla = 1;
    else
        return NULL; 

    (*i)++; 
    int start = *i;

    while (line[*i] && line[*i] != quote)
        (*i)++;

    int len = *i - start;

    char *token = ft_substr(line, start, len);

    if (line[*i] == quote)
        (*i)++;

    return token;
}

char *get_word_token(const char *line, int *i) {
    int start = *i;
    char c;
    while (line[*i] && !ft_isspace(line[*i]) && !is_operator_start(line[*i])) {
        if (line[*i] == '"' || line[*i] == '\'')
        {
            c = line[*i];
            (*i)++;
            while(line[*i] != c)
                (*i)++;
        }
        (*i)++;
    }
    return ft_strndup(line + start, *i - start);
}
// int (const char *line) {
//     int i = 0;
//    char quote = 0;
//     while (line[i]) {
//         if ((line[i] == '"' || line[i] == '\'') && quote == 0)
//         quote = line[i];
//         else if (line[i] == quote)
//             quote = 0;
//         i++;
//     }
//     if (quote != 0) {
//         printf("minishell: error: unclosed %c quote\n", quote);
//         return 1;
//     }
//     return 0;
// }

void check_token(t_tokens *tokens) {

    while(tokens){
        if (ft_strcmp(tokens->value, "<") == 0)
            tokens->type = REDIR_IN;
        else if (ft_strcmp(tokens->value, ">") == 0)
            tokens->type = REDIR_OUT;
        else if (ft_strcmp(tokens->value, ">>") == 0)
            tokens->type = REDIR_APPEND;
        else if (ft_strcmp(tokens->value, "<<") == 0)
            tokens->type = REDIR_HEREDOC;
        else if (ft_strcmp(tokens->value, "|") == 0)
            tokens->type = TOKEN_PIPE;
        else
            tokens->type = TOKEN_WORD;
        tokens = tokens->next;
    }
}

t_tokens *tokenize(const char *line, int *size) {
    int i = 0;
    t_tokens *tokens = NULL;

    char *token = NULL;
    int fla;

    while (line[i]) {
        while (ft_isspace(line[i]))
            i++;
        if (!line[i])
            break;
        fla = 0;

        if (is_operator_start(line[i]))
            token = get_operator_token(line, &i);
        else
            token = get_word_token(line, &i);
        
        add_token(&tokens, create_token(token, fla));
        (*size)++;
    }

    check_token(tokens);
                    
    return tokens;
}