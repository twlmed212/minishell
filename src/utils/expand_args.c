/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 02:38:26 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/22 06:10:14 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**expand_args(char **args, t_env_and_exit *shell)
{
	char **expanded;
	char *temp;
	int i;
	int count;

	if (!args)
		return (NULL);

	count = 0;
	while (args[count])
		count++;

	expanded = malloc(sizeof(char *) * (count + 1));
	if (!expanded)
		return (NULL);

	i = 0;
	while (args[i])
	{
		temp = expand_string(args[i], shell);
		expanded[i] = temp;
		i++;
	}
	expanded[i] = NULL;

	return (expanded);
}