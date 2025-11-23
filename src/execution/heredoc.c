#include "../../include/minishell.h"

#include "../../include/minishell.h"

char	*read_heredoc(char *delimiter)
{
	char *input;
	char *filename;
	int fd;

	filename = ft_strdup("./.heredoc_temp");
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("heredoc");
		free(filename);
		return (NULL);
	}

	while (1)
	{
		input = readline("> ");
		if (!input)
			break ;
		if (ft_strcmp(input, delimiter) == 0)
		{
			free(input);
			break ;
		}

		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);

		free(input);
	}

	close(fd);
	return (filename);
}
