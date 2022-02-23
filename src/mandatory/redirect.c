/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 17:45:43 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/23 17:29:02 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_direct_back(t_direct *new, t_direct **direct)
{
	t_direct	*copy;

    if (*direct == NULL)
        *direct = new;
    else
    {
        copy = *direct;
        while (copy->next != NULL)
            copy = copy->next;
        copy->next = new;
    }
}

static void	check_direct(t_input *input, t_direct *new,char *str, U_INT *i)
{
if (str[*i] == '<')
	{
		new->incoming = 1;
		if (str[*i + 1] == '<')
		{
			new->twin = 1;
			(*i)++;
			if (str[*i] && str[*i] == '>')
			{
				input->token = modif_strdup(">", input);
				print_token(input);
			}
		}
		else if (str[*i + 1] == '>')
		{
			input->token = modif_strdup("newline", input);
			print_token(input);
		}
	}
	else if (str[*i] == '>')
	{
		if (str[*i + 1] == '>')
		{
			new->twin = 1;
			(*i)++;
			if (str[*i] && str[*i] == '<')
			{
				input->token = modif_strdup("<", input);
				print_token(input);
			}
		}
	}
	(*i)++;
}

void    find_direct_in_comm(t_input *input, char *str, U_INT j, U_INT *i)
{
	U_INT		n;
	t_direct	*new;

	new = malloc(sizeof(t_direct));
	if (!new)
		print_error(input, 12, "malloc", NULL);
	direct_init(new, i, j, str);
    check_direct(input, new, str, i);
	while (str[*i] == ' ')
		(*i)++;
	while (str[*i] && str[*i] != '<' && str[*i] != '>' \
		&& str[*i] != '|' && str[*i] != ' ')
	{
		n = *i;
		go_through_word(str, i, input);
		new->name = modif_substr(str, n, *i - n, input);
		printf("new->name = %s\n", new->name);
	}
	add_direct_back(new, &input->direct);
}

void add_heredoc(t_input *input) // надо ли это вообще???
{
	t_direct	*copy;
	char		*command;

	copy = input->direct;
	// if (copy)
	// 	printf("00copy->name = %s, copy->incoming = %d, copy->twin = %d\n", \
	// copy->name, copy->incoming, copy->twin);
	while (copy)
	{
		if (copy->incoming == 1 && copy->twin == 1)
		{
			copy->stop_word = copy->name;
			command = modif_itoa(copy->value, input);
			copy->name = ft_strjoin("heredoc_", command, input);//после heredoc не должно быть знаков?
			// printf("11copy->name = %s, copy->stop_word = %s\n", copy->name, copy->stop_word);
			free(command);
		}
		copy = copy->next;
	}
}

