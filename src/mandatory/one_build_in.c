/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_build_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 23:04:01 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/22 23:37:10 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline U_INT	launcher(t_input *input)
{
	// if (input->command->build_number == 1)
	// 	return (launch_echo(input));
	// else if (input->command->build_number == 2)
	// 	return (launch_cd(input));
	// else if (input->command->build_number == 3)
	// 	return (launch_pwd(input));
	// else if (input->command->build_number == 4)
	// 	return (launch_export(input));
	// else if (input->command->build_number == 5)
	// 	return (launch_unset(input));
	// else if (input->command->build_number == 6)
	// 	return (launch_env(input));
	// else if (input->command->build_number == 7)
	// 	return (launch_exit(input));
	return (success);
}

static inline void	reverse_redir(t_input *input, int *fd1)
{
	int	fd2; // открываем файл для перенаправления станд.вывода
	
	if (input->command->direct_out->twin == 1) // это heredoc <<
		fd2 = open(input->command->direct_out->name, O_WRONLY | O_CREAT | O_APPEND, 0644); //опции ок?
	else //это <
		fd2 = open(input->command->direct_out->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);  //опции ок?
	if (fd2 == -1)
		print_error(input, errno, input->command->direct_out->name, NULL);
	dup2(STDOUT_FILENO, *fd1); // fd1 = 0
	if (*fd1 == -1)
	{
		close(fd2);
		dup2(input->std_out, STDOUT_FILENO); // is it ok????
		print_error(input, errno, "dup2", NULL);
	}
	if (dup2(fd2, STDOUT_FILENO) == -1) // STDOUT_FILENO = fd2
	{
		close(fd2);
		close(*fd1);
		dup2(input->std_out, STDOUT_FILENO); // is it ok????
		print_error(input, errno, "dup2", NULL);
	}
	close(fd2);
}

void	one_build_in(t_input *input)
{
	int	fd;

	fd = 0;
	if (input->command->direct_out)
		reverse_redir(input, &fd);
	input->num_error = launcher(input);
	if (fd == -1)
		return ;
	if (dup2(fd, STDOUT_FILENO) == -1) // теперь вместо станд. вывода исп-ся наш фд, fd = 0
	{
		close(fd);
		dup2(input->std_out, STDOUT_FILENO); // is it ok????
		print_error(input, errno, "dup2", NULL);
		return ;
	}
	close(fd);
}