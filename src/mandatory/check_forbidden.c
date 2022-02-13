/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_forbidden.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 18:34:57 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/13 19:26:12 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_status check_forbidden2(char *str, t_input *input, U_INT *i)
{
	printf("start test with i = %d, c = %c\n", *i, str[*i]);
	while (str[*i] == ' ')
		(*i)++;
    printf("20; str + *i = %s\n", str + *i);
	if (str[*i] == '&' || str[*i] == '|' || str[*i] == ';')
	{
		input->token = modif_substr(str, *i, 1, input);
        printf("24; input->token = %s\n", input->token);
		if (str[*i + 1] && str[*i + 1] == str[*i])
		{
			free(input->token);
			input->token = modif_substr(str, *i, 2, input);
		}
	}
    printf("31; str + *i = %s\n", str + *i);
	if (str[*i] == '!' || str[*i] == '>' || str[*i] == '<')
	{
		if ((str[*i + 1] && str[*i + 1] == str[*i] && str[*i] != '!'))
		// {
		// 	printf("now %s\n", str + *i);
			input->token = modif_strdup("newline", input);
		// }
		else
		{
			while (str[++*(i)] == ' ')
				;
			if (!str[*i])
				input->token = modif_strdup("newline", input);
		}
	}
    printf("47; str + *i = %s\n", str + *i);
	if (input->token)
	// {
	// 	printf("check_forbidden2 return FAIL\n");
		return (fail);
	// }
	// printf("check_forbidden2 return SUCCESS\n");
	return (success);
	}

t_status check_forbidden(char *str, t_input *input)
{
	U_INT i;

	i = 0;
	while (str[i])
	{
		if (check_forbidden2(str, input, &i) == success)
			{
                i++;
                // continue ;
            }
		else
			{
                printf("it's fail\n");
                return (fail);
            }
		while (str[i] && str[i] != '|')
			i++;
		if (str[i] == '|')
		{
			if (str[i + 1])
				i++;
			if (check_forbidden2(str, input, &i) == fail)
				return (fail);
		}
		i++;
	}
	return (success);
}