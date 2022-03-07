/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 21:20:59 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/07 23:26:53 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int ft_lstsize(t_env *env)
// {
//     size_t  i;
//     i = 0;
//     while (env != NULL)
//     {
//         env = env->next;
//         i++;
//     }
//     return (i);
// }
// t_env   *getnth(t_env *head, int n)
// {
//     int counter;
//     counter = 0;
//     while (counter < n && head)
//     {
//         head = head->next;
//         counter++;
//     }
//     return (head);
// }
// void ft_swap(char *key1, char *key2)
// {
//  char tmp_key;
//  tmp_key = *key1;
//  *key1 = *key2;
//  *key2 = tmp_key;
// }
// void    ft_swap(t_env *first, t_env *second)
// {
//     t_env   *tmp;
//     tmp = first;
//     first = second;
//     second = tmp;
// }
// t_env   *env_sort(t_env *env) //t_str   *bubble_sort(t_str *stack)
// {
//     int     i;
//     int     j;
//     int     k;
//     t_env   *tmp;
//     size_t  size;
//     i = -1;
//     tmp = env;
//     size = ft_lstsize(env);
//     printf("size = %zu\n", size);
//     while (++i < (int)size)
//     {
//         j = i;
//         while (++j < (int)size)
//         {
//             // while (getnth(tmp, i)->key)
//             // {
//                 // if (ft_strcmp(getnth(tmp, i)->key[1], getnth(tmp, j)->key[1]) && getnth(tmp, i)->k
//                 if (getnth(tmp, i)->key > getnth(tmp, j)->key)
//                 {
//                     // printf("key[i] = %s, key[j] = %s\n", getnth(tmp, i)->key, getnth(tmp, j)->key);
//                     ft_swap(&getnth(tmp, i), &getnth(tmp, j));
//                 }
//             //  k++;
//             // }
//         }
//     }
//     return (env);
// }

int ft_isalpha(int c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}
void    ft_lstadd_back(t_env **lst, t_env *new)
{
    t_env   *addnode;
    addnode = *lst;
    if (*lst == NULL)
        *lst = new;
    if (lst != NULL && addnode != NULL)
    {
        while (addnode->next)
            addnode = addnode->next;
    }
    if (addnode != NULL && new != NULL)
        addnode->next = new;
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
int only_export(t_env *env) //int   if_not_cmd1(t_env *env)
{
    t_env   *copy;
    copy = env;
    // env_sort(copy);
    while (copy)
    {
        if (copy->equal == 1)
            printf("declare -x %s%s\"%s\"\n", (copy)->key, "=", (copy)->value);
        else
            printf("declare -x %s%s%s\n", (copy)->key, "=", (copy)->value);
        copy = (copy)->next;
    }
    return (0);
}
void    ft_search_dups(t_env *env, t_env *new, t_input *input) //ft_search_dups(t_env *env, t_env *new, t_arg *arg)
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
            printf("lstnew key = %s, lstnew equal = %d, lstnew value = %s\n", lstnew->key, lstnew->equal, lstnew->value);
            // ft_lstadd_back(&env, lstnew);
            ft_lstadd_back(&input->envp, lstnew);
            // printf("env key = %s, env equal = %d, env value = %s\n", env->key, env->equal, env->value);
            printf("env key = %s, env equal = %d, env value = %s\n", input->envp->key, input->envp->equal, input->envp->value);
        }
        new = new->next;
    }
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
U_INT   launch_export(t_input *input, t_comm * command)
{
    int     i;
    t_env   *new;
    t_env   *tmp;
    input->num_error = 0;
    // i = 0;
    i = 1;
    new = NULL;
    if (command->words[1] == NULL)
    {
        printf("command->words[0] = %s, command->words[1] = %s\n", command->words[0], command->words[1]);
        return (only_export(input->envp));
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
    ft_search_dups(input->envp, new, input);
    // printf("envp key = %s, envp equal = %d, envp value = %s\n", input->envp->key, input->envp->equal, input->envp->value);
    free_new(new);
    return (input->num_error);
    // printf("envp key = %s, envp equal = %d, envp value = %s\n", input->envp->key, input->envp->equal, input->envp->value);
}
//сортировка слиянием
// void merge(t_env *a, t_env *b, t_env **c)
// {
//     t_env tmp;
//     *c = NULL;
//     if (a == NULL)
//  {
//         *c = b;
//         return;
//     }
//     if (b == NULL)
//  {
//         *c = a;
//         return;
//     }
//     if (a->key < b->key)
//  {
//         *c = a;
//         a = a->next;
//     }
//  else
//  {
//         *c = b;
//         b = b->next;
//     }
//     tmp.next = *c;
//     while (a && b)
//  {
//         if (a->key < b->key)
//      {
//             (*c)->next = a;
//             a = a->next;
//         }
//      else
//      {
//             (*c)->next = b;
//             b = b->next;
//         }
//         (*c) = (*c)->next;
//     }
//     if (a)
//  {
//         while (a)
//      {
//             (*c)->next = a;
//             (*c) = (*c)->next;
//             a = a->next;
//         }
//     }
//     if (b)
//  {
//         while (b)
//      {
//             (*c)->next = b;
//             (*c) = (*c)->next;
//             b = b->next;
//         }
//     }
//     *c = tmp.next;
// }
// void split(t_env *src,t_env **low,t_env **high)
// {
//  t_env *fast = NULL;
//     t_env *slow = NULL;
//     if (src == NULL || src->next == NULL)
//  {
//         (*low) = src;
//         (*high) = NULL;
//         return;
//     }
//     slow = src;
//     fast = src->next;
//     while (fast != NULL)
//  {
//         fast = fast->next;
//         if (fast != NULL)
//      {
//             fast = fast->next;
//             slow = slow->next;
//         }
//     }
//     (*low) = src;
//     (*high) = slow->next;
//     slow->next = NULL;
// }
// void mergeSort(Node **head) {
//     Node *low  = NULL;
//     Node *high = NULL;
//     if ((*head == NULL) || ((*head)->next == NULL)) {
//         return;
//     }
//     split(*head, &low, &high);
//     mergeSort(&low);
//     mergeSort(&high);
//     merge(low, high, head);
// }