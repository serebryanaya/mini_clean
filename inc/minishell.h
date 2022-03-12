/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 20:08:31 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/12 19:24:16 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MINISHELL_H_
# define _MINISHELL_H_

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
// # include <dir.h>
# include <string.h> // for strerror
# include <sys/errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>

#include <sys/types.h> //для *
#include <dirent.h> //для *

#define U_INT	unsigned int

/* Стандартное описание ошибок:
0 Undefined error
strerror(1) - Operation not permitted
strerror(2) - No such file or directory
3 - No such process
4- Interrupted system call
5- Input/output error
 6 - Device not configured
 7 - Argument list too long
 8 - Exec format error
 9 - Bad file descriptor
 10 - No child processes
 11 - Resource deadlock avoided
 12 - Cannot allocate memorymake fclean
 13 - Permission denied
 14 - Bad address
 15 - Block device required
 16 - Resource busy
 17 - File exists
 18 - Cross-device link
 19 - Operation not supported by device
 20 - Not a directory
 21 - Is a directory
 22 - Invalid argument
 23 - Too many open files in system
 24 - Too many open files
 25 - Inappropriate ioctl for device
 26 - Text file busy
 27 - File too large
 28 - No space left on device
 29 - Illegal seek
 30 - Read-only file system
 31 - Too many links
 32 - Broken pipe
 33 - Numerical argument out of domain
 34 - Result too large
 35 - Resource temporarily unavailable
 36 - Operation now in progress
 37 - Operation already in progress
 38 - Socket operation on non-socket
 39 - Destination address required
 40 - Message too long
 41 - Protocol wrong type for socket
 42 - Protocol not available
 43 - Protocol not supported
 44 - Socket type not supported
 45 - Operation not supported
 46 - Protocol family not supported
 47 - Address family not supported by protocol family
 48 - Address already in use
 49 - Can't assign requested address
 50 - Network is down
 51 - Network is unreachable
 52 - Network dropped connection on reset
 53 - Software caused connection abort
 54 - Connection reset by peer
 55 - No buffer space available
 56 - Socket is already connected
 57 - Socket is not connected
 58 - Can't send after socket shutdown
 59 - Too many references: can't splice
 60 - Operation timed out
 61 - Connection refused
 62 - Too many levels of symbolic links
 63 - File name too long
 64 - Host is down
 65 - No route to host
 66 - Directory not empty
 67 - Too many processes
 68 - Too many users
 69 - Disc quota exceeded
 70 - Stale NFS file handle
 71 - Too many levels of remote in path
 72 - RPC struct is bad
 73 - RPC version wrong
 74 - RPC prog. not avail
 75 - Program version wrong
 76 - Bad procedure for program
 77 - No locks available
 78 - Function not implemented
 79 - Inappropriate file type or format
 80 - Authentication error
 81 - Need authenticator
 82 - Device power is off
 83 - Device error
 84 - Value too large to be stored in data type
 85 - Bad executable (or shared library)
 86 - Bad CPU type in executable
 87 - Shared library version mismatch
 88 - Malformed Mach-o file
 89 - Operation canceled
 90 - Identifier removed
 91 - No message of desired type
 92 - Illegal byte sequence
 93 - Attribute not found
 94 - Bad message
 95 - EMULTIHOP (Reserved)
 96 - No message available on STREAM
 97 - ENOLINK (Reserved)
 98 - No STREAM resources
 99 - Not a STREAM
 100 - Protocol error
 101 - STREAM ioctl timeout
 102 - Operation not supported on socket
 103 - Policy not found
 104 - State not recoverable
 105 - Previous owner died
 106 - Interface output queue is full
Наши: 
127 - command not found
258 - command not found
*/

typedef enum e_status
{
	success, //0
	fail, //1
	end, //2
}	t_status;

typedef struct s_direct
{
	char			*name;
	char			*stop_word;
	int				value;
	int				incoming;
	int				twin;
	struct s_direct	*next;
}	t_direct;

typedef enum e_build //del???
{
	NO,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
}	t_build;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				equal;
	struct s_env	*next;
}	t_env;

typedef struct s_templ
{
	char			*value;
	U_INT			status;
	struct s_templ	*next;
}	t_templ;

typedef struct s_comm
{
	char			**words;
	t_direct		*direct_in;
	t_direct		*direct_out;
	t_build			build_number;
	struct s_comm	*next;
}	t_comm;

typedef struct s_input
{
	int			std_in; //сохранение первичного stdin. Del?
	int			std_out; //сохранение первичного stdout. Del?
	int			num_of_command;
	char		**arg_env;
	int			**fd; //fd[2]?
	U_INT		num_error;
	// char		*problem;//нестандартное описание ошибки
	char		*token; // нужен для вывода ошибки при недопустимом начале строки
	t_comm		*command;
	t_direct	*direct;
	t_env		*envp; //структура дял хранения истории. Когда ее чистить???
	t_env		*star;
	t_status	still_work;//помечаем, пора ли делать exit или продолжим работать
}	t_input;


// /*----сheck_forbidden-исключаем не те символы в начале команды---*/
// t_status check_forbidden(char *str, t_input *input);

/*----Все билд-ины---*/ 
U_INT		launch_pwd(t_input *input);
U_INT		launch_env(t_input *input); 
U_INT		launch_unset(t_input *input, t_comm *command);
U_INT		launch_export(t_input *input, t_comm *command);
U_INT		launch_exit(t_input *input, t_comm *command);
U_INT		launch_echo(t_input *input);
U_INT		launch_cd(t_input *input, t_comm *command);
U_INT		launch_exit(t_input *input, t_comm *command);
void		cd_print_error(t_input *input, U_INT error_num, char *msg, char *descrip);
char		*get_path_cd(t_input *input, t_comm *command, t_env *copy);
t_env		*create_sort_env(t_env *old, t_input *input);
t_env		*env_sort(t_env *list, t_env *new, t_input *input);
t_env		*sort_export(t_input *input);

/*----child_and_dups - работа с дочками, поиск пути---*/ 
void		it_is_child(t_input *input, U_INT i, U_INT counter);

/*----init-инициализация структур---*/ 
void		init_input(t_input *input);
void		direct_init(t_direct *new, U_INT *i, U_INT j, char *str);
void		choose_build(t_comm *command);
void		mark_direct(t_input *input);

/*----dollar - обработка доллара---*/ 
char		*find_dollar(char *tmp, U_INT *i, t_input *input);
char		*once_more_find_dollar(char *mid, t_input *input, U_INT i);
// char		*dollar_in_quotes(char *tmp, U_INT *i, char c, t_input *input);
char		*find_envp(char *mid, U_INT *i, t_input *input);
char		*find_question(char *mid, U_INT *i, t_input *input, char c);


/*----free_struct-очистка структур---*/ 
void		free_env(t_input *input);
void		free_all(t_input *input);
void		free_t_comm(t_input *input);
void		free_direct(t_input *input);
void		free_str_command(char *str_command, t_input *input, U_INT i);

/*----main----*/
int			main(int argc, char **argv, char **envp);

/*----envp - заполнение t_env *envp из argv[3]----*/
t_status	put_envp(char **envp, t_input *input);
void		make_env_array(t_input *input, char ***full_envp);
t_env		*create_new_list(char *str, t_input *input);
void		add_list_back(t_env *new, t_env **envp);

/*----finder- препарсинг, поиск спецсимвлов----*/
t_status    finder(char *str_command, t_input *input);

/*----one_build_in - запуск билд-ина, если у нас всего 1 команда----*/
int			reverse_redir(t_input *input);
void		reverse_redir2(t_input *input, int fd);
U_INT		launcher(t_input *input, t_comm	*command);

/*----parser - начало парсинга----*/
t_status	parser(char *str_command, t_input *input, U_INT i, U_INT j);
void		go_through_word(char *str, U_INT *i, t_input *input);
void		find_star_in_comm(t_input *input);

/*----pre_open - начало работы команд----*/
void		try_open(t_input *input);

/*----put_t_comm - заполнение пустой структуры t_comm----*/
t_status	create_empty_t_comm(char *str_command, t_input *input);

/*----redirect - поиск редиректов---*/ 
void		find_direct_in_comm(t_input *input, char *str, U_INT j, U_INT *i);
void		add_heredoc(t_input *input);

/*----signals - обработчики сигналов---*/ 
void		main_handler(int signal); // для Сtrl C
void		in_signal(t_input *input, U_INT i, t_comm *command, U_INT counter);
void		handler_child(int sig_num);
void		handler_pipex(int sig_num);

/*----shlvl - считаем уровень истории---*/ 
void		put_shlvl(t_input *input);

/*----star - обработка звезды---*/ 
// void		star_in_word(t_comm **tmp, U_INT *k, char *vrem, t_input *input);
// void		find_star_in_comm(t_input *input); // почистить в конце всей программы эту новую структуру!!!!!
void		find_star(t_comm *tmp, t_input *input, U_INT k);

/*----quotes - работа с кавычками---*/ 
t_status	go_to_end_quote(char *str, U_INT *i, char c, t_input *input);
// void		delete_quotes(char **str_command, t_input *input);//мб тут надо char **!!!
void		clean_command(t_input *input);
void		clean_direct(t_input *input);

/*----utils_cut - полезные функции для обзрезки строк---*/ 
char		*ft_strjoin(char *s1, char *s2, t_input *input);
char		*ft_strjoin_for_3(char *s1, char *s2, char *s3, t_input *input);
char		*modif_strdup(char *src, t_input *input);
char		*modif_substr(char *s, U_INT start, U_INT len, t_input *input);
/*----utils_print - полезные функции для печати---*/ 
void		print_error(t_input *input, U_INT error_num, char *msg, char *descrip);
t_status	print_token(t_input *input);

/*----utils_other - полезные функции для другой работы со строками---*/ 
U_INT		ft_strlen(char *s);
char		*modif_itoa(int n, t_input *input);
t_status	ft_strcmp(char *s1, char *s2);
// long		modif_atoi(char *s, int i, long number);
// long long	modif_atoi(char *s, int *flag, int i, long long number);
long long	modif_atoi(char *s, int *flag, int i);
int			ft_isalpha(int c);

/*----utils_other2 - ПРОДОЛЖЕНИЕ: полезные функции для другой работы со строками---*/ 
int			ft_strncmp(const char *str1, const char *str2, size_t n);
int			modif_strncmp(char *str1, char *str2, int n);
int			words_number(char const *str, char c);
char		*next_word(char const *str, char c);
char		**ft_split(char const *s, char c);

/*----utils_other3 - ПРОДОЛЖЕНИЕ: полезные функции для другой работы со строками---*/ 
char		*ft_strchr(char *str, char c);
void		iter_str(char *str, U_INT *i);

/*----utils_list - ПРОДОЛЖЕНИЕ: полезные функции для другой работы со строками---*/ 
void	ft_lstadd_middle(t_env *new, t_env *tmp);
void	ft_lstadd_back(t_env **lst, t_env *new);
void	ft_lstadd_front(t_env **lst, t_env *new);
U_INT	ft_lstsize(t_env *lst);
void	lstadd_back(t_templ **lst, t_templ *new);

/*----pipes ---*/ 
void		make_fork(t_input *input, t_comm *command, U_INT i);
void		open_pipes(t_input *input);
void		clean_path(char **path, U_INT i); //static void	free_path(char **path)
void		close_fd(t_input *input, int *fd_file, U_INT counter, U_INT i);

char		*path_home(t_input *input, t_comm *command);

//mac:     gcc file.c -L/Users/$USER/.brew/Cellar/readline/8.1.1/lib/ -I/Users/$USER/.brew/Cellar/readline/8.1.1/include -lreadline -o filename
//linux:   gcc -L/usr/local/lib -I/usr/local/include *.c -lreadline

// Функции ридлайна
	// rl_on_new_line(); //переход на новую строку
	// rl_replace_line("", 0); //заменяет буфер на текст в кавычках
	// rl_redisplay(); // Изменить то, что отображается на экране,
	// чтобы отразить текущее содержимое строкового буфера


#endif

// Оля: -сделать запуск echo. Работает некорректно((
//		-обработка ошибок
//		-unlink????	


/* В обработку ошибок: 
bash-3.2$ ytjy >>> gh
bash: syntax error near unexpected token `>'
bash-3.2$ ytjy <<< gh
bash: ytjy: command not found
bash-3.2$ ytjy ||| gh
bash: syntax error near unexpected token `|'
*/

/*
НЕверный экзит-статус
ls | wc
yes | head
ls
*/