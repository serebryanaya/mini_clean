/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 16:00:13 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/13 17:30:16 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline int put_exit(int status)
{
	return (status);
}

void	my_handler(int signal) // (void) ?
{
	(void)signal;
	// rl_on_new_line(); //переход на новую строку
	// rl_redisplay(); // Изменить то, что отображается на экране,
	// чтобы отразить текущее содержимое строкового буфера
	write(1, "  \b\b\n", 5); // Как убрать ^\????
	
	printf("1sig_num = %d\n", signal);
	
	// rl_on_new_line();
	// rl_replace_line("", 1); //заменяет буфер на текст в кавычках
	// rl_redisplay();
	// put_exit(0); // ???
}