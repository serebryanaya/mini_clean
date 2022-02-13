/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 20:20:45 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/12 17:42:37 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    t_input input;
    char *str_command;

    if (argc != 1)
    {
        printf("minishell: %s: %s\n", argv[1], strerror(2));
        printf("from main: TYPE ERROR %d\n", 127); //???? удалить!
        exit(127);//exit(1)??
    }
    init_input(&input);
    // printf("%d, %d \n", input.std_in, input.std_out);
    put_envp(envp, &input);
    return(0);
}