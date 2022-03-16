/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:14:27 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/15 23:50:59 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_input(t_input *input)
{
	input->num_of_command = 0;
	input->arg_env = NULL;
	input->fd = NULL;
	input->num_error = 0;
	input->token = NULL;
	input->command = NULL;
	input->direct = NULL;
	input->have_star = 0;
}

void	direct_init(t_direct *new, U_INT *i, U_INT j, char *str)
{
	new->name = NULL;
	new->stop_word = NULL;
	new->value = j;
	new->incoming = 0;
	new->twin = 0;
	new->next = NULL;
}

void	mark_direct(t_input *input)
{
	U_INT		i;
	t_direct	*copy;

	copy = input->direct;
	if (!input->command)
		return ;
	i = 0;
	while (copy)
	{
		while (i < copy->value)
		{
			input->command = input->command->next;
			i++;
		}
		if (copy->incoming == 1)
			input->command->direct_in = input->direct;
		else
			input->command->direct_out = input->direct;
		copy = copy->next;
	}
}

void	choose_build2(t_comm **command)
{
	t_comm	*copy;

	copy = *command;
	if (ft_strcmp(copy->words[0], "echo") == 0)
			copy->build_number = 1;
	else if (ft_strcmp(copy->words[0], "cd") == 0)
		copy->build_number = 2;
	else if (ft_strcmp(copy->words[0], "pwd") == 0)
		copy->build_number = 3;
	else if (ft_strcmp(copy->words[0], "export") == 0)
		copy->build_number = 4;
	else if (ft_strcmp(copy->words[0], "unset") == 0)
		copy->build_number = 5;
	else if (ft_strcmp(copy->words[0], "env") == 0)
		copy->build_number = 6;
	else if (ft_strcmp(copy->words[0], "exit") == 0)
		copy->build_number = 7;
}

void	choose_build(t_comm *command)
{
	t_comm	*copy;

	copy = command;
	while (copy != NULL)
	{
		if (copy->words[0] == NULL)
		{
			copy = copy->next;
			continue ;
		}
		else
			copy = copy->next;
	}
	choose_build2(&command);
}
