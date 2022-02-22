/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 16:00:13 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/17 19:23:29 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	my_handler(int signal)
{
	(void)signal;
	// printf("\n");
	rl_on_new_line();
	rl_redisplay();
	write(1, "  \b\b\n", 5);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}