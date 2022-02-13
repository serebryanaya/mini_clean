/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 20:20:45 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/13 19:28:12 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline void catch_str(char **command, t_input *input, U_INT i)
{
    *command = readline("minishell-1.0$ ");
    if (*command == NULL)
    {
        write(1, "minishell-1.0$ exit\n", 20);
        rl_redisplay(); //нужна ли??? надо проверить, как сохраняется история и коды ошибок, в тч на уровнях
        // тут - minishell: command not found: 0
        free_all(input);
	    exit(0);
    }
    if ((*command)[0] == '\0')
    {
        free(*command);
        // printf("Пришел enter. Идем на новый круг\n"); // del
        return ;
    }
    add_history(*command);
    while ((*command)[i] == ' ') // !может, дальше можно удалить
        i++;
    if ((*command)[i] == '\0')
    {
        free(*command);
        // printf("Пришел конец строки после пробелов. Идем на новый круг\n"); // del
        return ;
    }
    return ;
}

int main(int argc, char **argv, char **envp)
{
    t_input input;
    char *str_command;

    if (argc != 1)
    {
        printf("minishell-1.0: %s: %s\n", argv[1], strerror(2));
        // printf("from main: TYPE ERROR %d\n", 127); //???? удалить!
        exit(127);//exit(1)??
    }
    init_input(&input);
    // // printf("%d, %d \n", input.std_in, input.std_out);
    put_envp(envp, &input);
    put_shlvl(&input); // вызов шелла в шелле - пока не работает!
    while (1)
    {
        // if (catch_str(&str_command, &input, 0) == fail)
        //     continue ;//считываем строку через readline
        catch_str(&str_command, &input, 0);
        if (check_forbidden(str_command, &input) == fail)//исключаем не те символы в начале команды
        {
            input.num_error = 258;
            write(1, "minishell: ", 11);
            write(2, "syntax error near unexpected token", \
            ft_strlen("syntax error near unexpected token")); //????
            write(2, " `", 2); //????
            write(2, input.token, ft_strlen(input.token)); //????
            write(2, "'\n", 2); //????
            free(input.token);
            input.token = NULL;
            printf("TYPE ERROR %d\n", input.num_error); //???? удалить!
        }
        
    }
    // usleep(100000000);
    return(0);
}