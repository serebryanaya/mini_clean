#include "minishell.h"

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	unsigned int	i;

	i = 0;
	while (str1[i] == str2[i] && str1[i] != '\0' && str2[i] != '\0' && i < n)
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}

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

void	handler_pipex(int sig_num)
{
	if (sig_num == SIGINT) //SIGINT сигнал для остановки процесса пользователем с терминала Ctrl+C
		write(2, "\n", 1);
	
	//SIGQUIT сигнал. посылаемый процессу для остановки и указывающий,
	//что система должна выполнить дамп памяти для процесса (Ctrl+\)
	

	if (sig_num == SIGQUIT)
		write(2, "Quit: 3\n", 8); //почему 3? 3 - No such process В программе он должен выводить Quit: 3?
}

void in_signal(t_input *input, U_INT i, t_status flag, U_INT counter) //void	set_signal(int i, t_cmd *cmd)
{
	t_comm *copy;
	U_INT len;

	// printf("1111in_signal\n");
	// printf("dot_getpid = %d\n", getpid());
	copy = input->command;
	while (counter++ < i)
		copy = copy->next;
	if (copy->words == NULL || copy->words[0] == NULL)
	{
		// printf("in_signal\n");
		signal(SIGINT, SIG_IGN); //при значении обработчика равно SIG_IGN сигнал игнорируется
		signal(SIGQUIT, SIG_IGN);
		return ;
	}
	len = ft_strlen(copy->words[0]);
		// printf("len = %d\n", len);
	if (ft_strlen(copy->words[0]) > 9 && ft_strcmp(&copy->words[0][len - 10], "/minishell") == success) //почему 9?
		flag = 1;
	if (ft_strcmp(copy->words[0], "minishell") == fail && flag == 0)
	{
		// printf("in_signal2\n");
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
			close (fd_file[0]);
		if (fd_file[1])
			close (fd_file[1]);
	}
}

void child_dup2(t_input *input, t_comm *copy, U_INT i, int *fd) //static void	open_dup(int i, t_cmd *cmd, t_arg *arg)
{
	int	res;

	res = 0;
	if (copy->direct_in != NULL)
		res = dup2(fd[0], STDIN_FILENO);
	else if (i != 0 && input->fd)
		res = dup2(input->fd[i - 1][0], STDIN_FILENO);
	if (res == -1)
		print_error(input, errno, "dup2", NULL);
	if (copy->direct_out != NULL)
		res = dup2(fd[1], STDOUT_FILENO);
	else if (input->fd && i < input->num_of_command - 1)
//		res = dup2(fd[i][1], STDOUT_FILENO); //fd[i][1] -?
		res = dup2(input->fd[i][1], STDOUT_FILENO); //fd[i][1] -?
	if (res == -1)
		print_error(input, errno, "dup2", NULL);
	close_fd(input, fd, input->num_of_command - 1, 0);
}

void child_dup(t_input *input, t_comm *copy, U_INT i) //static void	open_dup(int i, t_cmd *cmd, t_arg *arg)
{
	int fd[2];

	fd[0] = -2;//-100???
	if (copy->direct_in != NULL)
		fd[0] = open(copy->direct_in->name, O_RDONLY);
	if (fd[0] == -1)
		print_error(input, errno, copy->direct_in->name, NULL);
	fd[1] = -2;//-100???
	if (copy->direct_out != NULL && copy->direct_out->twin == 1)
		fd[1] = open(copy->direct_out->name, O_WRONLY | O_CREAT | O_APPEND, 0644); // запись + если файл не существует, то он будет создан
	// + Файл открывается в режиме добавления. 0644 = mode задает права доступа: Право на чтение для всех, право на запись для владельца
	else if (copy->direct_out != NULL && copy->direct_out->twin == 0)
		fd[1] = open(copy->direct_out->name, O_WRONLY | O_CREAT | O_TRUNC, 0644); // trunc = перезапись
	if (fd[1] == -1)
		print_error(input, errno, copy->direct_out->name, NULL);
	child_dup2(input, copy, i, fd);
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
	// return(get_path2(bin, input, i));
//inline char *get_path2(char *bin, U_INT i, t_input *input, U_INT counter)
// char *get_path2(char *bin, U_INT i, t_input *input, U_INT nmb) //static char	*cmd_path(char *cmd, char *arg_cmd, int num, t_arg *arg)
char *get_path2(char *bin, t_input *input, U_INT i)
{
	//bin = cmd
    char *res; //tmp
	char **path;
	U_INT counter; // i

	// printf("input->arg_env[%d] = %s", i, input->arg_env[i]);
	// path = ft_split(input->arg_env[nmb] + 5, ':');//почему +5? PATH=...:....:...nmb-?
	path = ft_split(input->arg_env[i] + 5, ':');//почему +5? PATH=...:....:...nmb-?
	if (path == NULL)
		print_error(input, 12, "malloc", NULL);
	counter = 0;
	res = NULL;
	while (path[counter] != NULL && res == NULL)
	{
//		printf("get_path2\n");
		res = ft_strjoin_for_3(path[counter], "/", bin, input); //char	*ft_strjoin_for_3(char *s1, char *s2, char *s3, t_input *input)
		// printf("res = %s\n", res);
		if (access(res, F_OK))
		{
			free(res);
			res = NULL;
		}
		counter++;
	}
	if (res == NULL)
		res = bin;
	clean_path(path, 0);
	free(bin);
	return (res);
}
// path = get_path(copy->words[0], input, 0);
char *get_path(char *bin, t_input *input, U_INT i) //bin = cmd static char	*ft_path(char *cmd, t_arg *arg)
{
	char *path;

	// printf("111bin = %s\n", bin);
	// printf("5555input->arg_env[%d] = %s\n", i, input->arg_env[i]);
	if (!access(bin, F_OK)) //F_OK проверяет существование файла. В случае успеха возвращается 0, при ошибке -1
		return (bin); // точно тут не должно быть сообщение об ошибке?
	// printf("222bin = %s\n", bin);
	// while (input->arg_env[i] != NULL && ft_strcmp(input->arg_env[i], "PATH=") == fail) // fail=1
	while (input->arg_env[i] != NULL && ft_strncmp(input->arg_env[i], "PATH=", 5) != 0)
	{
		
		// printf("get_path\n");
		// printf("input->arg_env[%d] = %s\n", i, input->arg_env[i]);
		i++;
	}
	// printf("111input->arg_env[%d] = %s\n", i, input->arg_env[i]);
	if (input->arg_env[i] == NULL)
	{
		// printf("2222input->arg_env[%d] = %s\n", i, input->arg_env[i]);
		access(bin, F_OK);
		print_error(input, errno, bin, NULL);
	}
	// printf("333bin = %s\n", bin);
	// return(get_path2(bin, i, input, 0));
	return(get_path2(bin, input, i));
}

void it_is_child(t_input *input, U_INT i, U_INT counter) //void	child(int i, t_arg *arg)
{
	t_comm	*copy;
	char	*path;

	copy = input->command;
	printf("111num_error = %d, errno = %d\n", input->num_error, errno);
	signal(SIGINT, handler_child);
	signal(SIGQUIT, handler_child);
	while (counter++ < i)
		copy = copy->next;
	if (copy->words == NULL || !copy->words[0])
		exit(0);//почему 0?
	if (ft_strcmp(copy->words[0], "head") == 0)
		printf("1\n");
	child_dup(input, copy, i);
		if (ft_strcmp(copy->words[0], "head") == 0)
		printf("2\n");
	if (copy->build_number != 0)
	{
		// printf("copy->build_number = %u\n", copy->build_number);
		print_error(input, launcher(input), NULL, NULL);
	}
			if (ft_strcmp(copy->words[0], "head") == 0)
			printf("519595um_error = %d, errno = %d\n", input->num_error, errno);
	path = get_path(copy->words[0], input, 0);
			if (ft_strcmp(copy->words[0], "head") == 0)
		// printf("3\n");
			printf("222num_error = %d, errno = %d\n", input->num_error, errno);
	if (ft_strcmp(copy->words[0], "head") == 0)	
	printf("path = %s\n", path);
	if (execve(path, copy->words, input->arg_env) == -1)
//	execve() выполняет программу, заданную параметром filename.
//	Программа должна быть или двоичным исполняемым файлом, или
//	скриптом, начинающимся со строки вида "#! интерпретатор [аргументы]".
//	В последнем случае интерпретатор - это правильный путь к исполняемому файлу,
//	который не является скриптом; этот файл будет выполнен как интерпретатор [arg] filename.
//	argv - это массив строк, аргументов новой программы. envp -- это массив строк в формате key=value,
//	которые передаются новой программе в качестве окружения (environment).
//	Как argv, так и envp завершаются нулевым указателем. К массиву аргументов и к окружению
//	можно обратиться из функции main(), которая объявлена как int main(int argc, char *argv[], char *envp[]).
		{
			printf("5953\n");
			print_error(input, errno, copy->words[0], NULL);
		}
	printf("333num_error = %d, errno = %d\n", input->num_error, errno);
}

void modif_wait(pid_t *id, U_INT counter, t_input *input, U_INT i)
{
	int exit_status;

	while (i < counter)
	{
		waitpid(-1, &exit_status, 0); // -1 означает, что нужно ждать любого дочернего процесса.
		printf("exit_status = %d\n", exit_status);
		i++;
	}
	if (input != NULL)
	{
		if (WIFEXITED(exit_status)) // не равно нулю, если дочерний процесс успешно завершился.
			// input->num_error = WEXITSTATUS(exit_status); //возвращает восемь младших битов значения, которое вернул завершившийся дочерний процесс
						input->num_error = WIFEXITED(exit_status); //возвращает восемь младших битов значения, которое вернул завершившийся дочерний процесс
		else
			input->num_error = WIFEXITED(exit_status) + 128; // +128 для дочек
	}
	free(id);
}

void my_pipe(t_input *input, U_INT i) //void	pipex(t_arg *arg)
{
	pid_t	*id;

	id = malloc(sizeof(pid_t) * input->num_of_command);
	if (id == NULL)
		print_error(input, errno, "malloc", NULL);
	// printf("000getpid = %d\n", getpid());
	while (i < input->num_of_command)
	{
		// printf("my_pipe\n");
		in_signal(input, i, 0, 0); //проверить
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

void	pipes(t_input *input) //void	ft_pipe(t_arg *arg)
{
	int	i;

	input->fd = malloc(sizeof(int *) * (input->num_of_command - 1)); // почему -1?
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
