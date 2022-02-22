/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_open.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:17:22 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/20 21:21:41 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline t_status try_open2(t_input *input)
{
	t_direct *copy;

	copy = input->direct;
	while (copy != NULL)
	{
		// if (copy->incoming == 0 || copy->twin == 0)
		// {
		// 	if (try_open3(copy, input) == TRUE)
		// 		return (fail);
		// }
		else
			launch_heredoc(copy->name, input);
		copy = copy->next;
	}
	return (success);
}

void try_open(t_input *input)
{
	if (try_open2(input) == fail)
		return ;
	while (input->command != NULL)
	{
		input->num_of_command++;
		input->command = input->command->next;
	}
	if (input->num_of_command == 1 && input->command->number != 0)
		one_command(input);
	else if (input->num_of_command > 1)
		pipes(input); //ft_pipe(arg);
	else if (input->num_of_command != 1 || input->command->bin[0] != NULL)
		my_pipex(input, 0); 

	}
}