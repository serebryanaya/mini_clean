/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 22:03:32 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/04 22:48:39 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

U_INT	launch_exit(t_input *input, t_comm *command)
{
	int			flag;
	long long	status;

	flag = 0;
	status = 0;
	if (input->num_of_command != 1 && ft_strcmp(command->words[0], "exit") == success)// need???
		{
			// printf("!!!!GO OUT FROM EXIT\n");
			exit (0);
		}
	// else if (input->num_of_command != 1 && ft_strcmp(input->command->words[0], "exit") != success)
	// 	exit (-1);

	if (command && command->words && command->words[1])
	{
		// printf("!!!!copy->words[1] = %s\n", copy->words[1]);
		status = modif_atoi(command->words[1], &flag, 0, 0);
		if (flag == -1)
		{
			// printf("exit\n");//???
			print_status(input, 255, ft_strjoin("exit: ", \
			command->words[1], input), "numeric argument required");
			exit(255);
		}
		else if (command->words[2])
		{
			print_status(input, 1, "exit", "too many arguments");
		}
		else
			exit (status);
	}
