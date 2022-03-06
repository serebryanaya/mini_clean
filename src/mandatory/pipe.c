/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 15:16:49 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/06 20:41:24 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fd(t_input *input, int *fd_file, U_INT counter, U_INT i)
{
	while (i < counter)
	{
		if (!input->fd || !input->fd[i])
			continue ;
		close(input->fd[i][0]);
		close(input->fd[i][1]);
		free(input->fd[i]);
		i++;
	}
	free(input->fd);
	input->fd = NULL;
	if (fd_file)
	{
		if (fd_file[0])
			close(fd_file[0]);
		if (fd_file[1])
			close(fd_file[1]);
	}
}

void clean_path(char **path, U_INT i) //static void	free_path(char **path)
{
	while (path != NULL && path[i] != NULL)
	{
		free(path[i]);
		i++;
	}
	free(path);
}


void modif_wait(pid_t *id, U_INT counter, t_input *input, U_INT i)
{
	int exit_status;

	while (i < counter)
	{
		waitpid(-1, &exit_status, 0); // -1 означает, что нужно ждать любого дочернего процесса.
		i++;
	}
// printf("дочка завершилась. exit_status = %d\n WIFEXITED(exit_status) = %d\n WIFSIGNALED(exit_status) = %d\n WTERMSIG(exit_status) = %d\n WEXITSTATUS(exit_status) = %d\n input->num_error = %d, errno = %d\n", \
// exit_status, WIFEXITED(exit_status), WIFSIGNALED(exit_status), WTERMSIG(exit_status), WEXITSTATUS(exit_status), input->num_error, errno);
	if (input != NULL)
	{
		// if (WIFSIGNALED(exit_status) || input->num_of_command == 1) // не равно нулю, если дочерний процесс успешно завершился.
		if (WIFEXITED(exit_status)) // не равно нулю, если дочерний процесс успешно завершился.
				{
					// if (input->num_error != 0)
						// write(1, "minishell: ", 11);
						// printf("dgthh");
					input->num_error = WEXITSTATUS(exit_status); //возвращает восемь младших битов значения, которое вернул завершившийся дочерний процесс
				printf("дочка завершилась хорошо, через exit - input->num_error = %d, errno = %d\n", input->num_error, errno);
			}
	}
	else if (WIFSIGNALED(exit_status))
				{
					input->num_error = WTERMSIG(exit_status); //возвращает восемь младших битов значения, которое вернул завершившийся дочерний процесс
				printf("дочка завершилась хорошо, через сигнал - input->num_error = %d, errno = %d\n", input->num_error, errno);
			}
		else
			{
			// input->num_error = WTERMSIG(exit_status) + 128; // +128 для дочек
			
			input->num_error = exit_status + 128; // +128 для дочек
						printf("дочка завершилась ПЛОХО!!!! - input->num_error = %d, errno = %d\n", input->num_error, errno);
}

	/*
printf("дочка завершилась. exit_status = %d\n WIFEXITED(exit_status) = %d\n WIFSIGNALED(exit_status) = %d\n WTERMSIG(exit_status) = %d\n WEXITSTATUS(exit_status) = %d\n input->num_error = %d, errno = %d\n", \
exit_status, WIFEXITED(exit_status), WIFSIGNALED(exit_status), WTERMSIG(exit_status), WEXITSTATUS(exit_status), input->num_error, errno);

	if (input != NULL)
	{
		// if (WIFSIGNALED(exit_status) || input->num_of_command == 1) // не равно нулю, если дочерний процесс успешно завершился.
		if (WIFEXITED(exit_status)) // не равно нулю, если дочерний процесс успешно завершился.
		
			// input->num_error = WEXITSTATUS(exit_status); //возвращает восемь младших битов значения, которое вернул завершившийся дочерний процесс
						{
							input->num_error = WEXITSTATUS(exit_status); //возвращает восемь младших битов значения, которое вернул завершившийся дочерний процесс
						printf("дочка завершилась хорошо - input->num_error = %d, errno = %d\n", input->num_error, errno);
	}
		else
			{
			// input->num_error = WTERMSIG(exit_status) + 128; // +128 для дочек
			input->num_error = 128 + 128; // +128 для дочек
						printf("дочка завершилась ПЛОХО!!!! - input->num_error = %d, errno = %d\n", input->num_error, errno);
}
	}
	*/
	free(id);
}

void make_fork(t_input *input, t_comm *command, U_INT i) //void	pipex(t_arg *arg)
{
	pid_t	*id;

	id = malloc(sizeof(pid_t) * input->num_of_command);
	if (!id)
		print_error(input, errno, "malloc", NULL);
	// printf("000getpid = %d\n", getpid());
	while (i < input->num_of_command)
	{
		// printf("my_pipe\n");
		in_signal(input, i, input->command, 0); //проверить set_signal(i, arg->cmd);!!!!!!
		// printf("my_pipe2\n");
		id[i] = fork(); //создаем дочерние процессы
		if (id[i] == 0) //при успешном создании дочернего процесса
		{
			// printf("my_pipe3\n");
			// printf("55552getpid = %d\n", getpid());
			it_is_child(input, i, 0);
			// printf("my_pipe4\n");
		}
		else if (id[i] == -1)
		{
			// printf("my_pipe3\n");
			input->num_error = errno;
			close_fd(input, NULL, input->num_of_command - 1, 0); //ft_close
			modif_wait(id, i, input, 0); //ft_waitpid
			print_error(input, input->num_error, "fork", NULL);
		}
		i++;
	}
// printf("55552getpid = %d\n", getpid());
	close_fd(input, NULL, input->num_of_command - 1, 0);
	modif_wait(id, input->num_of_command, input, 0);
// printf("66552getpid = %d\n", getpid());

}

void	open_pipes(t_input *input) //void	ft_pipe(t_arg *arg)
{
	int	i;

	input->fd = malloc(sizeof(int *) * (input->num_of_command - 1));
	if (!input->fd)
		print_error(input, errno, "malloc", NULL);
	i = 0;
	while (i < (input->num_of_command - 1))
	{
		input->fd[i] = malloc(sizeof(int) * 2);
		if (!input->fd[i])
			print_error(input, errno, "malloc", NULL);
//		if (pipe(input->fd[i]) == fail)
		if (pipe(input->fd[i]) == -1)
		{
			input->num_error = errno;
			while (i--)
			{
				close(input->fd[i][0]);
				close(input->fd[i][1]);
			}
			print_error(input, errno, "pipe", NULL);
		}
		i++;
	}
}
