/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriddler <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 19:17:26 by rriddler          #+#    #+#             */
/*   Updated: 2022/02/22 19:17:31 by rriddler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	words_number(char const *str, char c)
{
	int	count;

	count = 0;
	while (*str != 0)
	{
		while (*str && *str == c)
			str++;
		if (*str && *str != c)
		{
			count++;
			while (*str && *str != c)
				str++;
		}
	}
	return (count);
}

static char	*next_word(char const *str, char c)
{
	int		i;
	char	*word;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	word = malloc(sizeof(char) * (i + 1));
	if (word == NULL)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != c)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	char	**arr;

	i = 0;
	if (s == NULL)
		return (NULL);
	arr = (char **)malloc(sizeof(char *) * (words_number(s, c) + 1));
	if (arr == NULL)
		return (NULL);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s && *s != c)
		{
			arr[i] = next_word(s, c);
			s = s + ft_strlen(arr[i]);
			i++;
		}
	}
	arr[i] = NULL;
	return (arr);
}

void handler_pipex(int sig_num)
{
	if (sig_num == SIGINT) //SIGINT сигнал для остановки процесса пользователем с терминала Ctrl+C
		write(2, "\n", 1);
	//SIGQUIT сигнал. посылаемый процессу для остановки и указывающий,
	//что система должна выполнить дамп памяти для процесса (Ctrl+\)
	if (sig_num == SIGQUIT)
		write(2, "Quit: 3\n", 8); //почему 3? 3 - No such process
}

void in_signal(t_input *input, U_INT i, t_status flag, U_INT counter)
{
	t_comm *copy;
	U_INT len;

	copy = input->command;
	while (counter++ < i)
		copy = copy->next;
	if (copy->words == NULL || copy->words[0] == NULL)
	{
		signal(SIGINT, SIG_IGN); //при значении обработчика равно SIG_IGN сигнал игнорируется
		signal(SIGQUIT, SIG_IGN);
		return ;
	}
	len = ft_strlen(copy->words[0]);
	if (ft_strcmp(&copy->words[0][len - 10], "/minishell") == success \
		&& ft_strlen(copy->words[0]) > 9) //почему 9?
		flag = fail;
	if (ft_strcmp(copy->words[0], "/minishell") == fail && flag == fail)
	{
		signal(SIGINT, handler_pipex);
		signal(SIGQUIT, handler_pipex);
		return ;
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void handler_child(int sig_num)
{
	if (sig_num == SIGINT)
		exit(2); //почему 2? No such file or directory
	if (sig_num == SIGQUIT)
		exit(3); //почему 3? No such process
}

void close_fd(t_input *input, int *fd_file, U_INT counter, U_INT i)
{
	while (i < counter)
	{
		if (input->fd == NULL || input->fd[i] == NULL)
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
			close (fd_file[0]);
		if (fd_file[1])
			close (fd_file[1]);
	}
}

void child_dup2(t_input *input, t_comm *copy, U_INT i, int *fd) //static void	open_dup(int i, t_cmd *cmd, t_arg *arg)
{
	int	res;

	if (copy->direct_in != NULL)
		res = dup2(fd[0], STDIN_FILENO);
	else if (i != 0 && input->fd != NULL)
		res = dup2(input->fd[i - 1][0], STDIN_FILENO);
	if (res == -1)
		print_error(input, errno, "dup2", NULL);
	if (copy->direct_out != NULL)
		res = dup2(fd[1], STDOUT_FILENO);
	else if (input->fd != NULL && i < input->num_of_command - 1)
//		res = dup2(fd[i][1], STDOUT_FILENO); //fd[i][1] -?
		res = dup2(input->fd[i][1], STDOUT_FILENO); //fd[i][1] -?
	if (res == -1)
		print_error(input, errno, "dup2", NULL);
	close_fd(input, fd, input->num_of_command - 1, 0);
}

void child_dup(t_input *input, t_comm *copy, U_INT i) //static void	open_dup(int i, t_cmd *cmd, t_arg *arg)
{
	int fd[2];

	fd[0] = -100;
	if (copy->direct_in != NULL)
		fd[0] = open(copy->direct_in->name, O_RDONLY);
	if (fd[0] == -1)
		print_error(input, errno, copy->direct_in->name, NULL);
	fd[1] = -100;
	if (copy->direct_out != NULL && copy->direct_out->twin == 1)
		fd[1] = open(copy->direct_out->name, O_WRONLY | O_CREAT | O_APPEND, 0644); // запись + если файл не существует, то он будет создан
	// + Файл открывается в режиме добавления. 0644 = mode задает права доступа: Право на чтение для всех, право на запись для владельца
	else if (copy->direct_out != NULL && copy->direct_out->twin == 0)
		fd[1] = open(copy->direct_out->name, O_WRONLY | O_CREAT | O_TRUNC, 0644); // trunc = перезапись
	if (fd[1] == -1)
		print_error(input, errno, copy->direct_out->name, NULL);
	child_dup2(input, copy, i, fd);
}

//U_INT	launcher(t_input *input)
//{
//	if (input->command->number == 1)
//		return (launch_echo(input));
//	else if (input->command->number == 2)
//		return (launch_cd(input));
//	else if (input->command->number == 3)
//		return (launch_pwd(input));
//	else if (input->command->number == 4)
//		return (launch_export(input));
//	else if (input->command->number == 5)
//		return (launch_unset(input));
//	else if (input->command->number == 6)
//		return (launch_env(input));
//	else if (input->command->number == 7)
//		return (launch_exit(input));
//	return (success);
//}

void clean_path(char **path, U_INT i)
{
	while (path != NULL && path[i] != NULL)
	{
		free(path[i]);
		i++;
	}
	free(path);
}

//inline char *get_path2(char *bin, U_INT i, t_input *input, U_INT counter)
char *get_path2(char *bin, U_INT i, t_input *input, U_INT counter)
{
	char *res; //tmp
	char **path;
//	U_INT counter; // i

	path = ft_split(input->arg_env[i] + 5, ':');//почему +5? PATH=...:....:...
	if (path == NULL)
		print_error(input, 12, "malloc", NULL);
	counter = 0;
	res = NULL;
	while (path[counter] != NULL && res == NULL)
	{
		res = ft_strjoin_for_3(path[counter], "/", bin, input); //char	*ft_strjoin_for_3(char *s1, char *s2, char *s3, t_input *input)
		if (access(res, F_OK) != success)
		{
			free(res);
			res = NULL;
		}
		counter++;
	}
	if (res == NULL)
		res = bin;
	clean_path(path, 0);
	return (res);
}

char *get_path(char *bin, t_input *input, U_INT i) //bin = cmd
{
	char *path;

	if (access(bin, F_OK) != success) //просто проверяет существование файла.
		return (bin); // точно тут не должно быть сообщение об ошибке?
	while (input->arg_env[i] != NULL && ft_strcmp(input->arg_env[i], "PATH") == fail) // "PATH="
		i++;
	if (input->arg_env[i] != NULL)
	{
		access(bin, F_OK);
		print_error(input, errno, bin, NULL);
	}
	return(get_path2(bin, i, input, 0));
}

void it_is_child(t_input *input, U_INT i, U_INT counter) //void	child(int i, t_arg *arg)
{
	t_comm	*copy;
	char	*path;

	copy = input->command;
	signal(SIGINT, handler_child);
	signal(SIGQUIT, handler_child);
	while (counter++ < i)
		copy = copy->next;
	if (copy->words == NULL || copy->words[0] == NULL)
		exit(0);//почему 0?
	child_dup(input, copy, i);
//	if (copy->number != 0)
//		print_error(input, launcher(input), NULL, NULL);
	path = get_path(copy->words[0], input, 0);
	if (execve(path, &copy->words[0], input->arg_env) == -1)
//	execve() выполняет программу, заданную параметром filename.
//	Программа должна быть или двоичным исполняемым файлом, или
//	скриптом, начинающимся со строки вида "#! интерпретатор [аргументы]".
//	В последнем случае интерпретатор - это правильный путь к исполняемому файлу,
//	который не является скриптом; этот файл будет выполнен как интерпретатор [arg] filename.
//	argv - это массив строк, аргументов новой программы. envp -- это массив строк в формате key=value,
//	которые передаются новой программе в качестве окружения (environment).
//	Как argv, так и envp завершаются нулевым указателем. К массиву аргументов и к окружению
//	можно обратиться из функции main(), которая объявлена как int main(int argc, char *argv[], char *envp[]).
		print_error(input, errno, copy->words[0], NULL);
}

void modif_wait(pid_t *id, U_INT i, t_input *input, U_INT counter)
{
	int exit_status;

	while (i < counter)
	{
		waitpid(-1, &exit_status, 0); // -1 означает, что нужно ждать любого дочернего процесса.
		i++;
	}
	if (input != NULL)
	{
		if (WIFEXITED(exit_status) != 1) // не равно нулю, если дочерний процесс успешно завершился.
			input->num_error = WEXITSTATUS(exit_status); //возвращает восемь младших битов значения, которое вернул завершившийся дочерний процесс
		else
			input->num_error = exit_status + 128; // почему 128?
	}
	free(id);
}

void my_pipe(t_input *input, U_INT i)
{
	pid_t	*id;

	id = malloc(sizeof(pid_t) * input->num_of_command);
	if (id == NULL)
		print_error(input, errno, "malloc", NULL);
	while (i < input->num_of_command)
	{
		in_signal(input, i, 0, 0);
		id[i] = fork(); //создаем дочерние процессы
		if (id[i] == 0) //при успешном создании лочернего процесса
			it_is_child(input, i, 0);
		else if (id[i] == -1)
		{
			input->num_error = errno;
			close_fd(input, NULL, input->num_of_command - 1, 0);
			modif_wait(id, i, input, 0);
			print_error(input, errno, "fork", NULL);
		}
		i++;
	}
	close_fd(input, NULL, input->num_of_command - 1, 0);
	modif_wait(id, input->num_of_command, input, 0);
}

void	pipes(t_input *input)
{
	U_INT	i;

	input->fd = malloc(sizeof(int *) * (input->num_of_command - 1)); // почему -1?
	if (input->fd == NULL)
		print_error(input, errno, "malloc", NULL);
	i = 0;
	while (i < (input->num_of_command - 1))
	{
		input->fd[i] = malloc(sizeof(int) * 2);
		if (input->fd[i] == NULL)
			print_error(input, errno, "malloc", NULL);
		if (pipe(input->fd[i]) == fail)
		{
			input->num_error = errno;
			while (i-- != 0)
			{
				close(input->fd[i][0]);
				close(input->fd[i][1]);
			}
			print_error(input, errno, "pipe", NULL);
		}
		i++;
	}
}
