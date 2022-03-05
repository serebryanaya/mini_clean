/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 22:03:48 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/24 22:03:51 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

// t_list	*ft_lstlast(t_list *lst)
// {
// 	t_list	*lastnode;

// 	lastnode = lst;
// 	if (lastnode == NULL)
// 		return (NULL);
// 	while (lastnode->next)
// 		lastnode = lastnode->next;
// 	return (lastnode);
// }

// t_env	*ft_env_lst_last(t_env *lst)
// {
// 	while (lst->next)
// 		lst = lst->next;
// 	return (lst);
// }

void	ft_lstadd_back(t_env **lst, t_env *new) //lib func
{
	t_env	*addnode;

	addnode = *lst;
	if (*lst == NULL)
		*lst = new;
	if (lst != NULL && addnode != NULL)
	{
		while (addnode->next)
			addnode = addnode->next;
	}
	if (addnode != NULL && new != NULL)
	{
		addnode->next = new;
		// printf("newkey = %s, newequal = %d, newvalue = %s\n", new->key, new->equal, new->value);
		// printf("nodekey = %s, nodeequal = %d, nodevalue = %s\n", addnode->next->key, addnode->next->equal, addnode->next->value);
	}
		
}

// void	ft_lstadd_back(t_env **lst, t_env *new)
// {
// 	t_env	*tmp;

// 	if (*lst)
// 	{
// 		// while ((*lst)->next)
// 		// 	*lst = (*lst)->next;
// 		// tmp = *lst;	
// 		tmp = ft_env_lst_last(*lst);
// 		tmp->next = new;
// 	}
// 	else
// 		*lst = new;
// }

// t_list	*ft_lstnew(void *content)
// {
// 	t_list	*newnode;

// 	newnode = (t_list *)malloc(sizeof(t_list));
// 	if (newnode == NULL)
// 		return (NULL);
// 	newnode->content = content;
// 	newnode->next = NULL;
// 	return (newnode);
// }

t_env	*ft_lstnew_env(t_env *env, t_input *input) //t_env	*ft_one_lst(t_env *env, t_arg *arg)
{
    t_env	*newnode;

	newnode = (t_env *)malloc(sizeof(t_env));
	if (newnode == NULL)
        print_error(input, 12, "malloc", NULL);
	newnode->key = modif_strdup(env->key, input);
	newnode->equal = env->equal;
	newnode->value = modif_strdup(env->value, input);
	newnode->next = NULL;
	return (newnode);
}

int	only_export(t_env *env) //int	if_not_cmd1(t_env *env)
{
	t_env	*copy;

	copy = env;
	while (copy)
	{
		if (copy->equal == 1)
			printf("declare -x %s%s\"%s\"\n", (copy)->key, "=", (copy)->value);
		else
			printf("declare -x %s\n", (copy)->key);
		copy = (copy)->next;
	}
	return (0);
}

void	change_env(t_env *env, t_env *new, t_input *input) //ft_search_dups(t_env *env, t_env *new, t_arg *arg)
{
	t_env	*copy2;
	t_env	*lstnew;	

	while (new != NULL)
	{
		copy2 = env;
		while (copy2 != NULL)
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
		if (copy2 == NULL)
		{
			// printf("!copy2\n");
			lstnew = ft_lstnew_env(new, input);
			// printf("lstnew key = %s, lstnew equal = %d, lstnew value = %s\n", lstnew->key, lstnew->equal, lstnew->value);
			ft_lstadd_back(&env, new);
			// ft_lstadd_back(&input->envp, lstnew);
			// while (env->next)
			// {
			// 	env = env->next;
			// 	// printf("123env key = %s, env equal = %d, env value = %s\n", env->key, env->equal, env->value);
			// }
			
			// printf("env key = %s, env equal = %d, env value = %s\n", input->envp->key, input->envp->equal, input->envp->value);
		}
		new = new->next;
		// printf("newkey = %s, newequal = %d, newvalue = %s\n", new->key, new->equal, new->value);
	}
}

void	free_new(t_env *new)
{
	t_env	*copy;

	while (new)
	{
		copy = new;
		new = new->next;
		free(copy->key);
		free(copy->value);
		free(copy);
	}
}

int	launch_export(t_input *input)
{
	int		i;
	t_env	*env;
	t_env	*new;
	t_env	*tmp;

	// env = input->envp;
	input->num_error = 0;
	// i = 0;
	i = 1;
	new = NULL;
	if (input->command->words[1] == NULL)
	{
		// printf("command->words[0] = %s, command->words[1] = %s\n", input->command->words[0], input->command->words[1]);
		return (only_export(input->envp));
	}
	// while (input->command->words[++i])
	while (input->command->words[i])
	{
		if (ft_isalpha(input->command->words[i][0]))
		{
			// printf("command[%d] = %s\n", i, input->command->words[i]);
			tmp = create_new_list(input->command->words[i], input);
			// printf("key = %s, equal = %d, value = %s\n", tmp->key, tmp->equal, tmp->value);    
			ft_lstadd_back(&new, tmp);
			// printf("new key = %s, new equal = %d, new value = %s\n", new->key, new->equal, new->value);
		}
		else
		{
		// 	printf("command[%d] = %c\n", i, input->command->words[i][0]);
			input->num_error = 1;
			write(2, "export: `", 9);
			write(2, input->command->words[i], ft_strlen(input->command->words[i]));
			write(2, "': not a valid identifier\n", 26);
		}
		i++;
	}
	env = input->envp;
	// change_env(env, tmp, input);
	change_env(env, new, input);
	// printf("envp key = %s, envp equal = %d, envp value = %s\n", input->envp->key, input->envp->equal, input->envp->value);
	// free_new(new);
	return (input->num_error);
	// printf("envp key = %s, envp equal = %d, envp value = %s\n", input->envp->key, input->envp->equal, input->envp->value);
}
