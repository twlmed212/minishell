#include "minishell.h"

static int	ft_help(char **args, int i, int sign)
{
	int	j;

	j = i;
	while (args[1][i])
	{
		if (!isdigit(args[1][i]))
			return (-1);
		i++;
	}
	if (args[2])
		return (-2);
	i = atoi(args[1] + j) * sign;
	return ((unsigned char)(i));
}

static int	ft_exit_check(char **args)
{
	int	i;
	int	sign;
	int	res;

	i = 0;
	sign = 1;
	while (args[1][i] == ' ' || (args[1][i] >= 9 && args[1][i] <= 13))
		i++;
	if (args[1][i] == '-' || args[1][i] == '+')
		if (args[1][i++] == '-')
			sign = -1;
	while (args[1][i] == '0')
		i++;
	res = strlen(args[1] + i);
	if (res > 19)
		return (-1);
	if (res == 19)
	{
		if (sign != -1 && strcmp(args[1] + i, MAX_EXIT) > 0)
			return (-1);
		if (sign == -1 && strcmp(args[1] + i, MIN_EXIT) > 0)
			return (-1);
	}
	return (ft_help(args, i, sign));
}

void builtin_exit(char **args)
{
	int	flag;

	if (!args[1]) // لا توجد وسائط
		exit(0);

	flag = ft_exit_check(args);

	if (flag == -1)
	{
		printf("minishell: %s%s", args[1], NAR_ERROR);
		flag = 2;
	}
	else if (flag == -2)
	{
		printf("%s", TMA_ERROR);
		return ; // لا نخرج من الـ shell
	}

	exit(flag);
}
