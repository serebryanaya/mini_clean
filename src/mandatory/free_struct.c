/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 16:22:34 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/22 22:44:40 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline void	free_env(t_input *input)
{
	t_env	*copy;

	if (!input->envp)
		return ;
	while (input->envp != NULL)
	{
		copy = input->envp;
		input->envp = input->envp->next;
		free(copy->key);
		free(copy->value);
		free(copy);
	}
	input->envp = NULL;
}

static inline void	free_arg_env(t_input *input)
{
	U_INT	i;

	i = 0;
	if (!input->arg_env)
		return ;
	while (input->arg_env[i])
	{
		free(input->arg_env[i]);
		i++;
	}
	free(input->arg_env);
	input->arg_env = NULL;
}

// static inline void free_fd(t_input *input) // это интовый массив! удалять с учетом этого
// {

// }

void free_all(t_input *input) // аналог void	free_arg(t_arg *arg), но надо доделывать!
{
	if (input->arg_env)
		free_arg_env(input);
	// if (input->problem)
	// 	free(input->problem);
	if (input->token)
		free(input->token);
	if (input->direct)
		free_direct(input);
	// if (input->fd)
	// 	free_fd(input);
	if (input->envp)
		free_env(input);
	if (input->command)
		free_t_comm(input);
	init_input(input); // need???
}

void	free_t_comm(t_input *input)
{
	t_comm	*copy;
	char	*str;
	U_INT i;

	i = 0;
	if (!input->command)
		return ;
	while (input->command != NULL)
	{
		copy = input->command;
		input->command = input->command->next;
		
		while (input->command->words[i])
		{
			free(input->command->words[i]);
			i++;
		}
		free(copy->words);
		free(copy);
	}
	input->command = NULL;
}

void free_direct(t_input *input)
{
	t_direct *copy;

	if (!input->direct)
		return ;
	while (input->direct != NULL)
	{
		copy = input->direct;
		input->direct = input->direct->next;
		free(copy->name);
		free(copy->stop_word);
		free(copy);
	}
	input->envp = NULL;
}