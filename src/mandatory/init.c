/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:14:27 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/08 20:29:36 by pveeta           ###   ########.fr       */
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
	// signal(SIGQUIT, SIG_IGN); // SIQuit = CTRL+\ , SIG_IGN - Сигнал   прерывания  игнорируется. \
	// В программе он должен выводить Quit: 3
	// // Ctrl D = вводу конца файла. Обработка НЕ через сигнал
	// signal(SIGINT, my_handler); // SIGINT = Ctrl C - перевод на новую строку
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

void mark_direct(t_input *input) // не понимаю, зачем это. пока убрала
{
	U_INT	i;

	printf("зашел в mark_direct\n");
	if (!input->command)
		{
			printf("я не нашел команду\n");
			return ;
		}
	i = 0;
	while (input->direct)
	{
		while (i < input->direct->value)
			{
				printf("direct->value=%d\n", input->direct->value);
				input->command = input->command->next;
				i++;
			}
		if (input->direct->incoming == 1)
		{
			printf("direct_incoming=%d\n", input->direct->incoming);
			input->command->direct_in = input->direct;
		}
		else
			input->command->direct_out = input->direct;
		input->direct = input->direct->next;
	}
	printf("вышел из mark_direct\n");
}

void choose_build(t_comm *command) // у меня е_нам, может быть можно проще написать
{
	t_comm *copy;

	copy = command;
	while (copy != NULL)
	{
		// printf("command->words[0] = %s, command->build_number = %d\n", command->words[0], \
		// command->build_number);
		if (copy->words[0] == NULL)
			continue; // зачем???
		else
		{
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
			else if (ft_strcmp(copy->words[0], "*") == 0)
				copy->build_number = -1;
			
		}
		copy = copy->next;
	}
	// 	printf("command->words[0] = %s, command->build_number = %d\n", command->words[0], \
	// command->build_number);
}