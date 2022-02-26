/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 20:20:45 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/23 18:11:17 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline t_status	catch_str(char **command, t_input *input, U_INT i)
{
	signal(SIGQUIT, SIG_IGN); // SIQuit = CTRL+\ , SIG_IGN - Сигнал   прерывания  игнорируется. \
	В программе он должен выводить Quit: 3
	// Ctrl D = вводу конца файла. Обработка НЕ через сигнал
	signal(SIGINT, my_handler); // SIGINT = Ctrl C - перевод на новую строку
	// *command = readline("minishell-1.0$ ");
	*command = readline("catch_str --> MINISHELL-1.0$ ");// вернуть верхний вариант!
	if (*command == NULL)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		write(1, "exit\n", 5);
		free_all(input);
		exit(0);
	}
	if(*command && *command[0])
		add_history(*command);
	while ((*command)[i] == ' ')
		i++;
	if ((*command)[i] == '\0')
	{
		free(*command);
		return (fail);
	}
	return (success);
}

static inline void go_parse_and_open(char *str_command, t_input *input, char **envp)
{
	if (parser(str_command, input) != success)
		return ;
	clean_command(input);
	// printf("0\n");
	clean_direct(input);
	// // printf("1\n");

	put_envp(envp, input);
	make_env_array(input, &input->arg_env);
	// printf("nput->arg_env[0] = %s\n", input->arg_env[0]);
	// // // printf("2\n");
	try_open(input);
}

void print_all(t_input *input)
{
	if (input)
	{
		printf("есть  input\n");
		printf("input->num_of_command = %u\n", input->num_of_command);
		if (input->arg_env)
		{
			printf("есть  arg_env\n");
			int i = 0;
			while (input->arg_env[i])
{
					printf("input->arg_env[%d] = %s\n", i, input->arg_env[i]);
		i++;
	}
		}
		else
			printf("no  arg_env!\n");
		
		if (input->fd)
		{
			printf("есть  fd\n");
			int i = -1;
			while (input->fd[++i])
				printf("input->fd[%d] = %d\n", i, *input->fd[i]);
		}
		else
			printf("no  fd!\n");

		printf("input->num_error = %u\n", input->num_error);
		printf("input->token = %s\n", input->token);

		if (input->command)
		{
			printf("есть  command\n");
			int i = -1;
			while (input->command->words[++i])
				printf("input->command->words[%d] = %s\n", i, input->command->words[i]);
			printf("input->command->build_number = %d\n", input->command->build_number);
			input->command = input->command->next;
		}
		else
			printf("no  commandd!\n");

		if (input->direct)
		{
			printf("есть  direct\n");
			int i = 0;
			printf("input->direct->name = %s\n", input->direct->name);
			printf("input->direct->stop_word = %s\n",input->direct->stop_word);
			printf("input->direct->value = %d\n", input->direct->value);
			printf("input->direct->incoming = %d\n", input->direct->incoming);
			printf("input->direct->twin = %d\n", input->direct->twin);
			input->direct = input->direct->next;
		}
		else
			printf("no  direct!\n");


	}
	else
		printf("no  input!\n");
}


int	main(int argc, char **argv, char **envp)
{
	t_input		input;
	char		*str_command;

	if (argc != 1)
	{
		printf("minishell-1.0: %s: %s\n", argv[1], strerror(2));
		// printf("from main: TYPE ERROR %d\n", 127); //???? удалить!
		exit(127);//exit(1)??
	}
	init_input(&input);
	put_envp(envp, &input);
	put_shlvl(&input); // вызов шелла в шелле - пока не работает!
	while (1)
	{
		// printf("new_cycle\n");
		// put_envp(envp, &input);
		// put_shlvl(&input); // вызов шелла в шелле - пока не работает!
		if (catch_str(&str_command, &input, 0) == fail)
			continue ;//считываем строку через readline
//		print_all(&input);
		if (finder(str_command, &input) != fail) // здесь надо дописать обработку ошибок
		{
//				printf("2.2\n");
				go_parse_and_open(str_command, &input, envp);
//				printf("main\n");
		}
		//  print_all(&input);
//		printf("334\n");



		// if (str_command)//???
			free_str_command(str_command, &input, 0);
		// print_all(&input);
	}
	return(0);
}
