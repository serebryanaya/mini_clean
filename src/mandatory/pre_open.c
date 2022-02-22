/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_open.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:17:22 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/22 18:36:50 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline t_status	try_open3(t_direct	*copy, t_input *input)
{
	int	fd;

	if (copy->incoming == 0 && copy->twin == 1)
		fd = open(copy->name, O_WRONLY | O_CREAT | O_APPEND, 0644); // ">>" дозапись файла
		//0644 = rw-r–r– - Право на чтение для всех, право на запись для владельца.
	else if (copy->incoming == 0 && copy->twin == 0)
		fd = open(copy->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(copy->name, O_RDONLY, 0644);
	if (fd == -1)
	{
		input->num_error = errno;
		print_error(input, input->num_error, copy->name, strerror(input->num_error));
		return (fail);
	}
	close(fd);
	return (success);
}

static inline void my_gnl(char **new_line, t_input *input)
{
	char *tmp;
	char *finish;

	// tmp = modif_strdup("\0", input);
	*new_line = modif_strdup("\0", input);
	finish = modif_strdup("\0", input); // modif_strdup("\0\0", input);
	while (finish[0] != '\n')
	{
		read(0, finish, 1);
		if (finish[0] == '\n')
			break ;
		tmp = ft_strjoin(*new_line, finish, input);
		if (tmp == NULL)
		{
			free(finish);
			print_error(input, 12, "malloc", NULL);
		}
		free(*new_line);
		*new_line = tmp;
	}
	free(finish);
}

static inline t_status	launch_heredoc(t_direct *copy, t_input *input)
{
	int			fd;
	char		*new_line;
	t_status	flag;

	printf("запуск heredoc");
	fd = open(copy->name, O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0644);
	if (fd == -1)
	{
		print_error(input, errno, copy->name, strerror(errno));
		return (fail); // кажется, это бессмысленно - сработает exit
	}
	flag = 0;
	while (flag == success)
	{
		write(1, "> ", 2);
		my_gnl(&new_line, input); // почему нельзя снова ридлайн?
		if (ft_strcmp(new_line, copy->stop_word) == 1)
		{
			if (write(fd, new_line, ft_strlen(new_line)) == -1)
				print_error(input, errno, copy->name, NULL);
			if (write(fd, "\n", 1) == -1)
				print_error(input, errno, copy->name, NULL);
			return (fail);
		}
		else
			flag = fail;
		free(new_line);
	}
	close(fd);
	return (success);
}

static inline t_status	try_open2(t_input *input)
{
	t_direct	*copy;
	int			fd;

	copy = input->direct;
	while (copy != NULL)
	{
		if (copy->incoming == 0 || copy->twin == 0)
		{
			printf("попытка открытия файла: copy->incoming == %d, \
			copy->twin == %d\n", copy->incoming, copy->twin);
			if (try_open3(copy, input) == fail)
				return (fail);
		}
		else
			if (launch_heredoc(copy, input) == fail)
				return (fail); // нужен ли здесь фейл и выход?
		copy = copy->next;
	}
	return (success);
}

void try_open(t_input *input)
{
	// printf("00попытка открытия файла: input->direct->name == %s", \
	// 		input->direct->name);
	if (try_open2(input) == fail)
		return ;
	while (input->command != NULL)
	{
		input->num_of_command++;
		input->command = input->command->next;
	}
	// printf("проверочки: num_of_command == %d, input->command->number == %d\n", \
	// input->num_of_command, input->command->number);
	// if (input->num_of_command == 1 && input->command->number != 0)
	// 	one_command(input);
	// else if (input->num_of_command > 1)
	// 	pipes(input); //ft_pipe(arg);
	// else if (input->num_of_command != 1 || input->command->bin[0] != NULL)
	// 	my_pipex(input, 0);
}