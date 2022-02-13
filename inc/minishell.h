/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 20:08:31 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/13 19:24:36 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MINISHELL_H_
# define _MINISHELL_H_

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h> // for strerror
# include <sys/errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>

#define U_INT	unsigned int

/* Стандартное описание ошибок:
strerror(1) - Operation not permitted
strerror(2) - No such file or directory

*/

typedef enum e_status
{
	success,
	fail,
}	t_status;

typedef struct s_direct
{
	char			*name;
	int				value;
	int				incoming;
	int				twin;
	struct s_direct	*next;
}	t_direct;

typedef enum e_build
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
	struct s_env	*next;
}	t_env;

typedef struct s_comm
{
	char			**bin;
	t_direct		*infile;
	t_direct		*outfile;
	t_build			number;
	struct s_comm	*next;
}	t_comm;

typedef struct s_input
{
	int			std_in; //сохранение первичного stdin. Del?
	int			std_out; //сохранение первичного stdout. Del?
	U_INT		num_of_command;
	char		**arg_env;
	int			**fd; //fd[2]?
	U_INT		num_error;
	// char		*problem;//нестандартное описание ошибки
	char		*token; // нужен для вывода ошибки при недопустимом начале строки
	t_comm		*command;
	t_direct	*direct;
	t_env		*envp; //структура дял хранения истории
	t_status	still_work;//помечаем, пора ли делать exit или продолжим работать
}	t_input;


/*----сheck_forbidden-исключаем не те символы в начале команды---*/
t_status check_forbidden(char *str, t_input *input);

/*----init-инициализация структур---*/ 
void		init_input(t_input *input);

/*----free_struct-очистка структур---*/ 
// void		free_env(t_input *input);
void		free_all(t_input *input);

/*----main----*/
int			main(int argc, char **argv, char **envp);

/*----envp - заполнение t_env *envp из argv[3]----*/
t_status	put_envp(char **envp, t_input *input);

/*----signals - обработчики сигналов---*/ 
void		my_handler(int signal); // (void) ?

/*----shlvl - считаем уровень истории---*/ 
void		put_shlvl(t_input *input);

/*----utils_cut - полезные функции для обзрезки строк---*/ 
char		*ft_strjoin(char *s1, char *s2, t_input *input);
char		*ft_strjoin_for_3(char *s1, char *s2, char *s3, t_input *input);
char		*modif_strdup(char *src, t_input *input);
char		*modif_substr(char *s, U_INT start, U_INT len, t_input *input);
/*----utils_print - полезные функции для печати---*/ 
void		print_error(t_input *input, U_INT error_num, char *msg, char *descrip);

/*----utils_other - полезные функции для другой работы со строками---*/ 
U_INT		ft_strlen(char *s);
char		*modif_itoa(int n, t_input *input);
t_status	ft_strcmp(char *s1, char *s2);
long		modif_atoi(char *s, int i, long number);


//mac:     gcc file.c -L/Users/$USER/.brew/Cellar/readline/8.1.1/lib/ -I/Users/$USER/.brew/Cellar/readline/8.1.1/include -lreadline -o filename
//linux:   gcc -L/usr/local/lib -I/usr/local/include *.c -lreadline

#endif