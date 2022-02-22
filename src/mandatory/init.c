/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:14:27 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/20 21:15:28 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_input(t_input *input)
{
	input->std_in = STDIN_FILENO;
	input->std_out = STDOUT_FILENO;
	input->num_of_command = 0;
	input->arg_env = NULL;
	input->fd = NULL;
	input->num_error = 0;
	// input->problem = NULL;
	input->token = NULL;
    input->command = NULL;
	input->direct = NULL;
	input->envp = NULL;
	input->still_work = success;
	signal(SIGQUIT, SIG_IGN); // SIQuit = CTRL+\ , SIG_IGN - Сигнал   прерывания  игнорируется. \
	В программе он должен выводить Quit: 3
	// Ctrl D = вводу конца файла. Обработка НЕ через сигнал
	signal(SIGINT, my_handler); // SIGINT = Ctrl C - перевод на новую строку
}

void	direct_init(t_direct *new, U_INT *i, U_INT j, char *str)
{
	new->name = NULL;
	// new->limiter = NULL;//??
	new->value = j;
	new->incoming = 0;
	new->twin = 0;
	new->next = NULL;
}

void mark_direct(t_input *input)
{
	U_INT	i;

	if (!input->command)
		return ;
	i = 0;
	while (input->direct)
	{
		while (i++ < input->direct->value)
			input->command = input->command->next;
		if (input->direct->incoming == 1)
			input->command->direct_in = input->direct;
		else
			input->command->direct_out = input->direct;
		input->direct = input->direct->next;
	}
}

void choose_build(t_comm *command) // у меня е_нам, может быть можно проще написать
{
	t_comm *copy;

	copy = command;
	while (copy != NULL)
	{
		if (command->bin[0] == NULL)
			continue; // зачем???
		else
		{
			if (ft_strcmp(command->bin[0], "echo") == 0)
				command->number = 1;
			else if (ft_strcmp(command->bin[0], "cd") == 0)
				command->number = 2;
			else if (ft_strcmp(command->bin[0], "pwd") == 0)
				command->number = 3;
			else if (ft_strcmp(command->bin[0], "export") == 0)
				command->number = 4;
			else if (ft_strcmp(command->bin[0], "unset") == 0)
				command->number = 5;
			else if (ft_strcmp(command->bin[0], "env") == 0)
				command->number = 6;
			else if (ft_strcmp(command->bin[0], "exit") == 0)
				command->number = 7;
		}
		copy = copy->next;
		// printf("command->bin[0] = %s, command->number = %d\n", command->bin[0], \
		// command->number);
	}
	free(copy);
}