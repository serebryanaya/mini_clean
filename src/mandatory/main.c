/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 20:20:45 by pveeta            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/03/08 21:05:58 by pveeta           ###   ########.fr       */
=======
/*   Updated: 2022/03/09 21:00:44 by pveeta           ###   ########.fr       */
>>>>>>> star
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline t_status	catch_str(char **command, t_input *input, U_INT i)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, main_handler);
	*command = readline("minishell-1.0$ ");
	if (*command == NULL)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		write(2, "exit\n", 5);
		free_all(input);
		exit(0);
	}
	if (*command && *command[0])
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

static inline void	go_parse_and_open(char *str_command, \
t_input *input, char **envp)
{
<<<<<<< HEAD
	if (parser(str_command, input, 0, 0) != success)
=======
	
	if (parser(str_command, input) != success)
>>>>>>> star
		return ;
	clean_command(input);
	clean_direct(input);
	make_env_array(input, &input->arg_env);
	try_open(input);
			if (input->command)
		{
			printf("есть  command\n");
			int i = -1;
			while (input->command->words[++i])
				printf("input->command->words[%d] = %s\n", i, input->command->words[i]);
			printf("input->command->build_number = %d\n", input->command->build_number);
			input->command = input->command->next;
		}
}

int	main(int argc, char **argv, char **envp)
{
	t_input		input;
	char		*str_command;

	if (argc != 1)
	{
		printf("minishell-1.0: %s: %s\n", argv[1], strerror(2));
		exit(127);
	}
	init_input(&input);
	put_envp(envp, &input);
	put_shlvl(&input);
	while (1)
	{
		if (catch_str(&str_command, &input, 0) == fail)
			continue ;
<<<<<<< HEAD
		if (finder(str_command, &input) != fail)
=======
		if (finder(str_command, &input) != fail) // здесь надо дописать обработку ошибок
>>>>>>> star
			go_parse_and_open(str_command, &input, envp);
		free_str_command(str_command, &input, 0);
	}
	return (0);
}
