/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 22:03:41 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/24 22:03:45 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	launch_env(t_input *input)
{
	t_env	*tmp;

    if (!input->envp)
		return (0);
	tmp = input->envp;

    // printf("1launch_env input->envp->key = %s\n", input->envp->key);

    // int i = 0;
    // while (input->arg_env[i] != NULL)
	// {
	//     printf("input->arg_/env[%d] = %s\n", i, input->arg_env[i]);
	// 	i++;
	// }

    // while (tmp)
	// {
	//     printf("key = %s\n, value = %s\n", tmp->key, tmp->value);
	// 	tmp = tmp->next;
	// }

    while (tmp != NULL)
	{
	   	if (tmp->equal == 1)
			printf("%s%s%s\n", tmp->key, "=", tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
