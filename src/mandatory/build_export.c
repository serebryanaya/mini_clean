/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 21:20:59 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/08 20:28:47 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env   *create_sort_env(t_env *old, t_input *input)
{
    t_env   *new;

    // printf("create\n");
    if (old->equal == 1)
        new = create_new_list(ft_strjoin_for_3(old->key, "=", old->value, input), input);
    else
        new = create_new_list(modif_strdup(old->key, input), input);
    return (new);    
}

t_env   *env_sort(t_env *list, t_env *new, t_input *input) //t_str   *bubble_sort(t_str *stack)
{
    int     i;
    int     j;
    t_env   *tmp;
    size_t  size;

    i = -1;
    tmp = list;
    // printf("env_sort\n");
    while (tmp)
    {
        if (tmp == list && modif_strncmp(tmp->key, new->key, 1) < 0)
        {
            ft_lstadd_front(&list, new);
        }
        if (tmp->next != NULL && modif_strncmp(tmp->key, new->key, 1) > 0 &&\
            modif_strncmp(tmp->next->key, new->key, 1) < 0)
            ft_lstadd_middle(new, tmp);
        if (tmp->next == NULL && modif_strncmp(tmp->key, new->key, 1) > 0)
            ft_lstadd_back(&list, new);
      tmp = tmp->next;
    }
    return (list);
}

t_env	*sort_export(t_input *input)
{
    U_INT   i;
    t_env   *old;
    t_env   *new;
    t_env   *list;
    
    // printf("sort_export\n");
    list = NULL;
    old = input->envp;
    while (old != NULL)
    {
        new = create_sort_env(old, input);
        if (list == NULL)
        {
            list = new;
            old = old->next;
            continue ;
        }
        list = env_sort(list, new, input);
        old = old->next;
    }
    return (list);
}

int ft_isalpha(int c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

t_env   *ft_lstnew_env(t_env *env, t_input *input) //t_env  *ft_one_lst(t_env *env, t_arg *arg)
{
    t_env   *newnode;
    newnode = (t_env *)malloc(sizeof(t_env));
    if (newnode == NULL)
        print_error(input, 12, "malloc", NULL);
    newnode->key = modif_strdup(env->key, input);
    newnode->equal = env->equal;
    newnode->value = modif_strdup(env->value, input);
    newnode->next = NULL;
    return (newnode);
}

void    free_new(t_env *new)
{
    t_env   *copy;
    while (new)
    {
        copy = new;
        new = new->next;
        free(copy->key);
        free(copy->value);
        free(copy);
    }
}

int only_export(t_input *input) //int   if_not_cmd1(t_env *env)
{
    t_env   *copy;

    // printf("only_export\n");
    copy = sort_export(input);
    while (copy)
    {
        if (copy->equal == 1)
        {
            // printf("1sort->key = %s, sort->equal = %d, sort->value = %s\n", copy->key, copy->equal, copy->value);
            printf("declare -x %s%s\"%s\"\n", copy->key, "=", copy->value);
        }
            
        // if (copy->equal == 1 && copy->value == NULL)
        // {
        //     printf("value = NULL\n");
        //     printf("declare -x %s%s\"\"\n", copy->key, "=");
        // }
        if (copy->equal != 1)
        {
            // printf("2sort->key = %s, sort->equal = %d, sort->value = %s\n", copy->key, copy->equal, copy->value);
            printf("declare -x %s\n", copy->key);
        }
        copy = (copy)->next;
    }
    free_new(copy);
    return (0);
}
void    change_env(t_env *env, t_env *new, t_input *input) //ft_search_dups(t_env *env, t_env *new, t_arg *arg)
{
    t_env   *copy2;
    t_env   *lstnew;

    while (new)
    {
        copy2 = env;
        while (copy2)
        {
            if (!ft_strcmp(new->key, copy2->key))
            {
                // printf("new->key = %s, copy2->key = %s\n",new->key, copy2->key);
                free(copy2->value);
                copy2->value = modif_strdup(new->value, input);
                break ;
            }
            else
            {
                // printf("copy2 key = %s, copy2 equal = %d, copy2 value = %s\n", copy2->key, copy2->equal, copy2->value);
                copy2 = copy2->next;
            }
        }
        if (!copy2)
        {
            // printf("!copy2\n");
            lstnew = ft_lstnew_env(new, input);
            // printf("lstnew key = %s, lstnew equal = %d, lstnew value = %s\n", lstnew->key, lstnew->equal, lstnew->value);
            // ft_lstadd_back(&env, lstnew);
            ft_lstadd_back(&input->envp, lstnew);
            // printf("env key = %s, env equal = %d, env value = %s\n", env->key, env->equal, env->value);
            // printf("env key = %s, env equal = %d, env value = %s\n", input->envp->key, input->envp->equal, input->envp->value);
        }
        new = new->next;
    }
}


U_INT   launch_export(t_input *input, t_comm * command)
{
    int     i;
    t_env   *new;
    t_env   *tmp;
    t_env   *env;

    input->num_error = 0;
    env = input->envp;
    i = 1;
    new = NULL;
    if (command->words[1] == NULL)
    {
        // printf("command->words[0] = %s, command->words[1] = %s\n", command->words[0], command->words[1]);
        return (only_export(input));
    }
    // while (input->command->words[++i])
    while (command->words[i])
    {
        if (ft_isalpha(command->words[i][0]))
        {
            // printf("command[%d] = %c\n", i, input->command->words[i][0]);
            // printf("command[%d] = %s\n", i, input->command->words[i]);
            tmp = create_new_list(command->words[i], input);
            // printf("key = %s, equal = %d, value = %s\n", tmp->key, tmp->equal, tmp->value);
            ft_lstadd_back(&new, tmp);
            // printf("new key = %s, new equal = %d, new value = %s\n", new->key, new->equal, new->value);
            // ft_lstadd_back(&input->envp, tmp);
        }
        else
        {
        //  printf("command[%d] = %c\n", i, input->command->words[i][0]);
            // input->num_error = 1;
            // write(2, "export: `", 9);
            // write(2, input->command->words[i], ft_strlen(input->command->words[i]));
            // write(2, "': not a valid identifier\n", 26);
            printf("%s%s%s\n", "export: `", command->words[i], "': not a valid identifier");
        }
        i++;
    }
    change_env(input->envp, new, input);
    // printf("envp key = %s, envp equal = %d, envp value = %s\n", input->envp->key, input->envp->equal, input->envp->value);
    free_new(new);
    return (input->num_error);
    // printf("envp key = %s, envp equal = %d, envp value = %s\n", input->envp->key, input->envp->equal, input->envp->value);
}
