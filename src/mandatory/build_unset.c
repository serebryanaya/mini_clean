/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 22:03:53 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/24 22:03:56 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_env(t_input *input)
{
	t_env	*copy;

	copy = input->envp->next;
	input->envp->next = copy->next;
	free(copy->key);
	free(copy->value);
	free(copy);
}

// int	find_key(t_input **input, char *str)
// {
// 	t_env	*copy;

// 	copy = *input->envp;
// 	if (!ft_strcmp(str, copy->key))
// 	{
// 		*input->envp = (*input->envp)->next;
// 		free(copy->key);
// 		free(copy->value);
// 		free(copy);
// 		return (1);
// 	}
// 	return (0);
// }

int	find_key(t_input *input, char *str)
{
	t_env	*copy;

	copy = input->envp;
	if (!ft_strcmp(str, copy->key))
	{
		input->envp = input->envp->next;
		free(copy->key);
		free(copy->value);
		free(copy);
		return (1);
	}
	return (0);
}

int	launch_unset(t_input *input)
{
	int		i;
	t_env	*copy1;
	t_env	*copy2;

	i = 0;
	while (input->command->words[++i])
	{
		if (!input->envp)
			break ;
		if (input->command->words[i][0] == '_')
			continue ;
		if (find_key(input, input->command->words[i]))
			continue ;
		copy1 = input->envp;
		copy2 = copy1->next;
		while (copy2 && ft_strcmp(input->command->words[i], copy2->key))
		{
			copy1 = copy2;
			copy2 = copy2->next;
		}
		if (copy2)
			remove_env(input);
	}
	return (0);
}