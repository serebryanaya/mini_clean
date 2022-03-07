/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 16:00:13 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/07 17:56:26 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	main_handler(int signal)
{
	(void)signal;
	// printf("\n");
	rl_on_new_line();
	rl_redisplay();
	write(2, "  \b\b\n", 5);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handler_pipex(int sig_num)
{
	if (sig_num == SIGINT) //SIGINT сигнал для остановки процесса пользователем с терминала Ctrl+C
		write(2, "\n", 1);
	
	//SIGQUIT сигнал. посылаемый процессу для остановки и указывающий,
	//что система должна выполнить дамп памяти для процесса (Ctrl+\)
	if (sig_num == SIGQUIT)
		write(2, "Quit: 3\n", 8); //3 - No such process
}

void in_signal(t_input *input, U_INT i, t_comm *copy, U_INT counter) //void	set_signal(int i, t_cmd *cmd)
{
	U_INT		len;
	t_status	flag;
	// printf("1111in_signal\n");
	// printf("dot_getpid = %d\n", getpid());
	
	flag = 0;
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


