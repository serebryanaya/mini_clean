/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:14:27 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/13 19:26:04 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    init_input(t_input *input)
{
	input->std_in = STDIN_FILENO;
	input->std_out = STDOUT_FILENO;
	input->num_of_command = 0;
	input->arg_env = NULL;
	input->fd = NULL;
	input->num_error = 0;
	// input->problem = NULL;
	input->token = NULL;
    input->command = NULL;
	input->direct = NULL;
	input->envp = NULL;
	input->still_work = success;
	signal(SIGQUIT, SIG_IGN); // SIQuit = CTRL+\ , SIG_IGN - Сигнал   прерывания  игнорируется. \
	В программе он должен выводить Quit: 3
	// Ctrl D = вводу конца файла. Обработка НЕ через сигнал
    signal(SIGINT, my_handler); // SIGINT = Ctrl C - перевод на новую строку
}