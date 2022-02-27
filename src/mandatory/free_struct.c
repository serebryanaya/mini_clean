/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 16:22:34 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/27 21:18:54 by pveeta           ###   ########.fr       */
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
		// printf("1\n");
	// if (input->problem)
	// 	free(input->problem);
	if (input->token)
		free(input->token);
				// printf("2\n");
	if (input->direct)
		free_direct(input);
				// printf("3\n");
	// if (input->fd)
	// 	free_fd(input);
	if (input->envp)
		free_env(input);
				// printf("4\n");
	if (input->command)
		free_t_comm(input);
				// printf("5\n");
	init_input(input); // need???
}

void	free_t_comm(t_input *input)
{
	t_comm	*copy;
	char	*str;
	U_INT i;

	i = 0;
	if (!input || !input->command)
		return ;
	while (input->command)
	{
		copy = input->command;
		input->command = input->command->next;
		// while (copy->words[i]) // ????
		// 	free(copy->words[i++]);
		// free(copy->words);
		free(copy);
	}
	input->command = NULL;
}

void free_direct(t_input *input)
{
	t_direct *copy;

	if (!input->direct)
		return ;
	while (input->direct)
	{
		copy = input->direct;
		input->direct = input->direct->next;
		if (copy->incoming == 1 && copy->twin == 1)
			unlink(copy->name);
		free(copy->name);
		free(copy->stop_word);
		free(copy);
	}
	input->direct = NULL;
}

void free_str_command(char *str_command, t_input *input, U_INT i)
{
	free(str_command);
	free_direct(input);
	free_t_comm(input); // del?????
	if (input->fd)
	{
		while (input->fd && input->fd[i])
			free(input->fd[i++]);
		free(input->fd);
		input->fd = NULL;
	}
	// i = 0;
	// while (input->arg_env && input->arg_env[i])
	// 	free(input->arg_env[i++]);
	// free(input->arg_env);
	// input->arg_env = NULL;
	
	input->num_of_command = 0;
	input->still_work = success;
	// input->num_error = 0;
	free_arg_env(input);
	// init_input(input);
}