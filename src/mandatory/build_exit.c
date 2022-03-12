/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 22:03:32 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/12 19:24:45 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

U_INT	launch_exit(t_input *input, t_comm *command)
{
	int			flag;
	long long	status;
	char		*str;

	flag = 0;
	status = 0;
	// printf("exit\n");
	// if (input->num_of_command != 1 && ft_strcmp(command->words[0], "exit") == success)// need???
	// 	{
	// 		// printf("!!!!GO OUT FROM EXIT\n");
	// 		exit (0);
	// 	}
	//  if (input->num_of_command != 1)
	//  	exit (0);
	// else if (input->num_of_command != 1 && ft_strcmp(input->command->words[0], "exit") != success)
	// 	exit (-1);
// if (command->words[1])
// printf("command->words[1] = %s\n", command->words[1]);
// if (command->words[2])
// printf("command->words[2] = %s\n", command->words[2]);

	if (command && command->words && command->words[1])
	{
		// printf("!!!!copy->words[1] = %s\n", copy->words[1]);
		// input->still_work = fail;
		status = modif_atoi(command->words[1], &flag, 0); // !!!!!!
		if (flag == -1)
		{
			// if (input->num_of_command == 1) //НЕ УДАЛЯТЬ!!!
			// 	write(2, "exit\n", 5); //НЕ УДАЛЯТЬ!!!
			str = ft_strjoin_for_3("minishell: exit: ", command->words[1], ": numeric argument required\n", input);
			write(2, str, ft_strlen(str));
			free(str);
			// printf("exit\n");//???
			// print_error(input, 255, ft_strjoin("exit: ", command->words[1], input), "numeric argument required");
			input->num_error = 255;
			exit(255);
		}
		else if (command->words[2])
		{
			// if (input->num_of_command == 1)//НЕ УДАЛЯТЬ!!!
			// 	write(2, "exit\n", 5); //НЕ УДАЛЯТЬ!!!
			str = ft_strjoin("minishell: exit: ", "too many arguments\n", input);
			write(2, str, ft_strlen(str));
			free(str);
			input->num_error = 1;
			return (1);
			// input->num_error = 1;
			// print_error(input, 1, "exit", "too many arguments");
		}
				else
			{
				input->num_error = status;
			if (input->num_of_command == 1)
				write(2, "exit\n", 5);
				exit (status);
			}
	}
		else
			{
				input->num_error = status;
				printf("exit\n");
				exit (status);
			}

	return (0);
}
