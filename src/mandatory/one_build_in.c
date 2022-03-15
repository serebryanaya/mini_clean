/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_build_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 23:04:01 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/07 23:30:31 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

U_INT	launcher(t_input *input, t_comm	*command) //int	run_built(t_cmd *cmd, t_arg *arg)
{
	if (command->build_number == 1)
		return (launch_echo(input));
	else if (command->build_number == 2)
		return (launch_cd(input, command));
	else if (command->build_number == 3)
		return (launch_pwd(input));
	else if (command->build_number == 4)
		return (launch_export(input, command));
	else if (command->build_number == 5)
		return (launch_unset(input, command));
	else if (command->build_number == 6)
		return (launch_env(input));
	else if (command->build_number == 7)
		return (launch_exit(input, command));
	// else if (command->build_number == -1)
	// 	return (launch_star(input, command));
	return (success);
}

// int	reverse_redir(t_input *input)
// {
// 	int fd_safe;
// 	int	new_file; // новый файл для редиректа
	
// 	if (!input->command->direct_out)
// 		return (-1);
// 	if (input->command->direct_out->twin == 1) // это heredoc >>
// 		new_file = open(input->command->direct_out->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	else //это >
// 	{
// 		printf("direct_out->name=%s\n", input->command->direct_out->name);
// 		new_file = open(input->command->direct_out->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	}
// 	printf("1new_file = %d\n", new_file);
// 	if (new_file == -1)
// 	{
// 		printf("new_file == -1\n");
// 		print_error(input, errno, input->command->direct_out->name, NULL);
// 	}
// 	dup2(STDOUT_FILENO, fd_safe); // fd_safe = STDOUT_FILENO
// 	if (fd_safe == -1)
// 	{
// 		printf("1error_dup2\n");
// 		close(new_file);
// 		print_error(input, errno, "dup2", NULL);
// 	}
// 	if (dup2(new_file, STDOUT_FILENO) == -1) // STDOUT_FILENO = new_file, перезаписали new_file в STDOUT_FILENO
// 	{
// 		printf("2error_dup2\n");
// 		close(new_file);
// 		close(fd_safe);
// 		print_error(input, errno, "dup2", NULL);
// 	}
// 	printf("2new_file = %d\n", new_file);
// 	close(new_file);
// 	printf("3new_file = %d\n", new_file);
// 	// printf("fd_safe=%d\n", fd_safe);
// 	return(fd_safe);
// 	// return (0);
// }

int	reverse_redir(t_input *input) //void	open_dup(int i, t_cmd *cmd, t_arg *arg)
{
	int	new_file; // новый файл для редиректа
	
	if (!input->command->direct_out)
		return (-1);
	if (input->command->direct_out->twin == 1) // это heredoc >>
		new_file = open(input->command->direct_out->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else //это >
	{
		printf("direct_out->name=%s\n", input->command->direct_out->name);
		new_file = open(input->command->direct_out->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	printf("1new_file = %d\n", new_file);
	if (new_file == -1)
	{
		printf("new_file == -1\n");
		print_error(input, errno, input->command->direct_out->name, NULL);
	}
	if (dup2(new_file, STDOUT_FILENO) == -1) // STDOUT_FILENO = new_file, перезаписали new_file в STDOUT_FILENO
	{
		printf("2error_dup2\n");
		close(new_file);
		print_error(input, errno, "dup2", NULL);
	}
	printf("2new_file = %d\n", new_file);
	close(new_file);
	printf("3new_file = %d\n", new_file);
	// return (new_file);
	return (0);
}

// void	reverse_redir2(t_input *input, int fd)
// {
// 	int res;

// 	res = 0;
// 	if (fd == -1)
// 		return ;
// 	res = dup2(fd, STDOUT_FILENO);
// 	// res = dup(fd);
// 	// // if (dup2(fd, STDOUT_FILENO) == -1) // STDOUT_FILENO = fd1 вернули станд.вывод
// 	if (res == -1) // STDOUT_FILENO = fd1 вернули станд.вывод
// 	{
// 		printf("3error_dup2, fd = %d, res=%d\n", fd, res);
// 		close(fd);
// 		print_error(input, errno, "dup2", NULL);
// 		return ;
// 	}
// 	close(fd);
// }
