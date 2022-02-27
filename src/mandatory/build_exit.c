/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 22:03:32 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/27 21:13:21 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

U_INT	launch_exit(t_input *input)
{
	t_comm		*copy;
	int			flag;
	long long	error;

	flag = 0;
	error = 0;
	copy = input->command;
	printf("exit\n");//???
	printf("!!!!input->num_of_command = %d, copy->words[0] = %s,  ft_strcmp(input->command->words[0], \"exit\") = %d\n", \
	input->num_of_command, copy->words[0], ft_strcmp(input->command->words[0], "exit"));
	if (input->num_of_command != 1 && ft_strcmp(input->command->words[0], "exit") == success)
		{
			// printf("!!!!GO OUT FROM EXIT\n");
			exit (0);
		}
	// else if (input->num_of_command != 1 && ft_strcmp(input->command->words[0], "exit") != success)
	// 	exit (-1);

	while (copy && ft_strcmp(copy->words[0], "exit") != success)
		copy = copy->next;
	if (copy && copy->words && copy->words[1])
	{
		// printf("!!!!copy->words[1] = %s\n", copy->words[1]);
		error = modif_atoi(copy->words[1], &flag, 0, 0);
		if (flag == -1)
		{
			// printf("exit\n");//???
			print_error(input, 255, ft_strjoin("exit: ", \
			copy->words[1], input), "numeric argument required");
			exit(255);
		}
		else if (copy->words[2])
		{
			print_error(input, 1, "exit", "too many arguments");
		}
		else
			exit (error);
	}
	// {
	// 	print_error(input, 1, "exit", "too many arguments");
	// }
	
	/* ПРОШЛАЯ ВЕРСИЯ
	if (input->command && input->command->words && ft_strcmp(input->command->words[0], "exit") != success)
		return (0);
	if (input->command && input->command->words && input->command->words[1])
	{
		error = modif_atoi(input->command->words[1], &flag, 0, 0);
		if (flag == -1 && ft_strcmp(input->command->words[1], "*") != success)
		{
			// printf("exit\n");//???
			print_error(input, 255, ft_strjoin("exit: ", \
			input->command->words[1], input), "numeric argument required");
			exit(255);
		}
		else if (flag == -1 && ft_strcmp(input->command->words[1], "*") == success)
			print_error(input, 1, "exit", "too many arguments");
		else if (flag == 0)
			{
				// printf("exit\n");//???
				// free_all(input);
				exit (error);
			}
			*/


	// }
	// else if (ft_strcmp(input->command->words[0], "exit") == success)
	// {
	// 	printf("exit\n");//???
	// 	if (input->num_of_command == 1)
	// 	{
	// 		input->still_work = fail;
	// 		print_error(input, error, "exit", NULL);
	// 	}
	// 	exit(0);
	// }
	return (0);
}



	// if (input->num_of_command == 1)
	// {
	// 	if (input->command->words[1])
	// 	{
	// 		error = modif_atoi(input->command->words[1], flag, 0, 0);
	// 		if (flag != success && ft_strcmp(input->command->words[1], "*") != success)
	// 			print_error(input, 255, ft_strjoin("exit: ", \
	// 			input->command->words[1], input), "numeric argument required");
	// 		else if (flag != success && ft_strcmp(input->command->words[1], "*") == success)
	// 			print_error(input, 1, "exit", "too many arguments");
	// 		else
	// 		{
	// 			printf("exit\n");//???
	// 			input->still_work = fail;
	// 			print_error(input, ((error + 256) % 256), "exit", NULL);
	// 		}
	// 	}
	// 	else
	// 	{
	// 		printf("exit\n");//???
	// 		input->still_work = fail;
	// 		print_error(input, 0, "exit", NULL);
	// 	}
	// }
	// else
	// {

	// }



	// }
	// if (input->num_of_command != 1)
	// {
	// 	if (ft_strcmp(input->command->name, "exit") == success)
	// 		return (0);

	// 	else
	// 		return (0);
	// }
		


	// printf ("res = %d", (256 - 5) % 256);
	// if (command->name[1])
	// {
	// 	if (modif_atoi(char *s, int i, long number) )
	// }
	
	// printf("exit\n");

	// if (command->name[1])
	// pwd = getcwd(NULL, 0); //получаем абсолютный путь к рабочему каталогу
	// if (!pwd)
	// 	print_error(input, errno, "getcwd", NULL); 
	// printf("%s\n", pwd);
	// free(pwd);
	// return (0);
// }