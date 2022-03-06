/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_and_dups.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 15:19:35 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/06 20:41:13 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		// printf("00res = %s\n", res);
		if (access(res, F_OK))
		{
			// printf("11res = %s\n", res);
			free(res);
			res = NULL;
		}
		counter++;
	}
	if (res == NULL)
		res = bin;
	// printf("25462res = %s\n", res);
	clean_path(path, 0);
    // printf("22res = %s\n", res);
	return (res);
}
// path = get_path(copy->words[0], input, 0);
char *get_path(char *bin, t_input *input, U_INT i) //bin = cmd static char	*ft_path(char *cmd, t_arg *arg)
{
	char *path;

	// printf("111bin = %s\n", bin);
	// printf("5555input->arg_env[%d] = %s\n", i, input->arg_env[i]);
	if (!access(bin, F_OK)) //F_OK проверяет существование файла. В случае успеха возвращается 0, при ошибке -1
		{
            // printf("222bin = %s\n", bin);
            return (bin); // точно тут не должно быть сообщение об ошибке?
        }
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
		print_error(input, errno, "bin", NULL);
	}
	// printf("333bin = %s\n", bin);
	// return(get_path2(bin, i, input, 0));
	return(get_path2(bin, input, i));
}

void it_is_child(t_input *input, U_INT i, U_INT counter) //void	child(int i, t_arg *arg)
{
	t_comm	*copy;
	char	*path;
    char    *str;

	copy = input->command;
	// printf("111num_error = %d, errno = %d\n", input->num_error, errno);
	signal(SIGINT, handler_child);
	signal(SIGQUIT, handler_child);
	while (counter++ < i)
		copy = copy->next;
	if (!copy || !copy->words || !copy->words[0]) //if (copy->words == NULL || !copy->words[0])
		exit(0);//почему 0?
	// if (ft_strcmp(copy->words[0], "head") == 0)
		// printf("pid = %d, 1\n", getpid());
    str = ft_strjoin(copy->words[0], ": command not found", input);
    // modif_strdup(copy->words[0], input);
    //  printf("copy = %s\n", copy->words[0])
    //  if (copy->next && copy->next->words[0])
    child_dup(input, copy, i);
		// if (ft_strcmp(copy->words[0], "head") == 0)
		// printf("pid = %d, 2\n", getpid());
	if (copy->build_number != 0)
	{
		// printf(", getpid() = %d, copy->build_number = %u\n", getpid(), copy->build_number);
		launcher(input, copy);
        // if (copy->words[0] && ft_strcmp(copy->words[0], "exit") == 0)
        //     exit(input->num_error);
        // exit (launcher(input, copy)); //спросить у Оли
		// print_error(input, launcher(input, copy), NULL, NULL); //сообщение об ошибке?
	}
			// if (ft_strcmp(copy->words[0], "head") == 0)
			// printf("519595um_error = %d, errno = %d\n", input->num_error, errno);
// printf("222num_error = %d, errno = %d\n", input->num_error, errno);
	else
    {
        // printf("NO BUILD getpid() = %d, copy->build_number = %u\n", getpid(), copy->build_number);
        if (copy->next && copy->next->words[0] && ft_strcmp(copy->next->words[0], "exit") == 0)
            exit(7);
        //  printf("e111xecve =,  copy->words[0] = %s, \n",  copy->words[0]);
        path = get_path(copy->words[0], input, 0);
        // printf("434\n");
        // printf("e333xecve =,  copy->words[0] = %s, \n",  copy->words[0]);
	if (!path)
		{
			// printf("3\n");
            print_error(input, 127, NULL, str);
			// exit (127) ;
		}
	// child_dup(input, copy, i);
			// if (ft_strcmp(copy->words[0], "head") == 0)
		// printf("3\n");
			// printf("222num_error = %d, errno = %d\n", input->num_error, errno);
	// if (ft_strcmp(copy->words[0], "head") == 0)	
	// printf("path = %s\n", path);
    // if (copy->words[1] && ft_strcmp(copy->words[1], "exit") == 0)
    //     	{
    //             printf("3\n");
    //             free(str);
    //             exit (0);	
    //         }
    // printf("4355\n");
    // printf("execve =,  input->arg_env = %s \n",  input->arg_env[0]);
    //     printf("execve =,  copy->words[0] = %s, \n",  copy->words[0]);
    if (execve(path, copy->words, input->arg_env) != 0)
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
			// free(path);
			// str = modif_strdup(copy->words[0], input);
            // printf("5953\n");
// 			printf("1111errno = %d\n", errno);
// // 			if (copy->words[0])
// // printf("copy->words[0] = %s\n", copy->words[0]);
// 			input->num_error = 127;
// 			// write(1, "minishell: ", 11);
// 			// write(2, "execve: ", 8);
// 			// write(2, "command not found\n", 18);
			
// 			// exit (258) ;
// 			print_error(input, 127, NULL, "command not found");
// 			exit (127);
        // printf("4\n");
			print_error(input, 127, NULL, str); //мб это вернуть???
            free(str);
		}
    }
    
        // printf("33num_error = %d, errno = %d\n", input->num_error, errno);
	// input->num_error = 0;
	// printf("45\n");
    free(str);
    exit (input->num_error);	
		// free(path);
		// printf("5953\n");
	// printf("333num_error = %d, errno = %d\n", input->num_error, errno);
	// free(path);
}