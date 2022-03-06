/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 22:03:53 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/06 21:20:15 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    remove_env(t_env *env) //void   ft_del_env(t_env *env)
{
    t_env   *copy;
    copy = env->next;
    env->next = copy->next;
    free(copy->key);
    free(copy->value);
    free(copy);
}
int find_key(t_env **env, char *str) //int  ft_check_first(t_env **env, char *str)
{
    t_env   *copy;
    copy = *env;
    if (!ft_strcmp(str, copy->key))
    {
        *env = (*env)->next;
        free(copy->key);
        free(copy->value);
        free(copy);
        return (1);
    }
    return (0);
}
U_INT   launch_unset(t_input *input, t_comm *command)
{
    int     i;
    t_env   *copy1;
    t_env   *copy2;
    i = 0;
    while (command->words[++i])
    {
        if (!input->envp)
            break ;
        if (command->words[i][0] == '_')
            continue ;
        if (find_key(&input->envp, command->words[i]))
            continue ;
        copy1 = input->envp;
        copy2 = copy1->next;
        while (copy2 && ft_strcmp(command->words[i], copy2->key))
        {
            copy1 = copy2;
            copy2 = copy2->next;
        }
        if (copy2)
            remove_env(copy1);
    }
    return (0);
}
