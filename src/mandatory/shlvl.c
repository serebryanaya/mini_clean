/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 18:32:46 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/04 21:09:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline void put_shlvl2(t_input *input, U_INT level) // эту штуку надо тестировать, когда есть весь код
{
	t_env   *copy;
    int     flag;
    
    copy = input->envp;
    flag = 0;
    while (copy != NULL)
	{
		if (ft_strcmp(copy->key, "SHLVL") == 0 && copy->value[0] == '\0')
		{
			free(copy->value);
			copy->value = modif_strdup(modif_itoa(level, input), input);
			return ;
		}
		else if (ft_strcmp(copy->key, "SHLVL") == 0 && copy->value[0] != '\0')
		{
			level += modif_atoi(copy->value, &flag, 0, 0);
			free(copy->value);
			copy->value = modif_strdup(modif_itoa(level, input), input); 
			return ;   
		}
		copy = copy->next;
	}
}

void put_shlvl(t_input *input)
{
    t_env   *copy;
    
    copy = input->envp;
    // printf("check: input->envp->key = %s\n", input->envp->key);
    if (!copy)
    {
        // printf("check\n");
        copy = malloc(sizeof(t_env));
        if (copy == NULL)
            print_error(input, 12, "malloc", NULL);
        copy->key = modif_strdup("SHLVL", input);
        copy->value = modif_strdup("1", input);
        // add_list_back(copy, &input->envp);
        input->envp = copy;
        copy->next = NULL;
        // printf("check: input->envp->key = %s input->envp->value = %s\n", input->envp->key, input->envp->value);
    }
    else
        put_shlvl2(input, 1);
}