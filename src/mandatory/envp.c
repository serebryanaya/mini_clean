/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_shlvl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 16:10:51 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/13 15:13:49 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void add_list_back(t_env *new, t_env **envp)
{
    t_env *copy;

    if (*envp == NULL)
        *envp = new;
    else
    {
        copy = *envp;
        while (copy->next != NULL)
            copy = copy->next;
        copy->next = new;
    }
}

static inline t_env *create_new_list(char *str, t_input *input)
{
    U_INT   i;
    t_env *tmp;

    // printf("str = %s\n", str);
   
    /*
	протестировать без этого, какая будет ошибка при некорректном вводе команды

	if (!((str[0] >= 'A' && str[0] <= 'Z') || \
         (str[0] >= 'a' && str[0] <= 'z') ||
        str[0] == '_'))
        // printf("%s: %s: not a valid identifier\n", input->command->(*bin), str);//??????
        print_error(input, 1, "export", "not a valid identifier");//пока не могу это протестировать

	*/
    tmp = malloc(sizeof(t_env));
    if (tmp == NULL)
        return (NULL);
    i = 0;
    while (str[i] && str[i] != '=')
		i++;
    tmp->key = modif_substr(str, 0, i, input);
    if (!str[i] || !str[i + 1])
        tmp->value = modif_strdup("\0", input);
    else
        tmp->value = modif_substr(str, i + 1, ft_strlen(str) - i + 1, input);
    tmp->next = NULL;
    return(tmp);
}

t_status	put_envp(char **envp, t_input *input)
{
    U_INT   i;
    t_env   *new;

    i = 0;

    while (envp[i] != NULL)
    {
        new = create_new_list(envp[i], input);
		if (!new)
			print_error(input, 12, "malloc", NULL);
        add_list_back(new, &input->envp);
        i++;
    }
	return (success);	
}