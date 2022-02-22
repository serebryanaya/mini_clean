/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 18:42:03 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/22 22:34:33 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_status	go_to_end_quote(char *str, U_INT *i, char c, t_input *input)
{
	U_INT	m;

	m = *i + 1;
	while (str[m])
	{
		if (str[m] == c)
		{
			*i = m + 1;
			return (success);
		}
		else
			m++;
	}
	input->token = modif_strdup("newline", input);
	print_token(input);
	(*i)++;
	return (fail);
}

static inline char *delete_quotes2(char *tmp, U_INT *i, t_input *input, char c)
{
	U_INT	j;
	char	*start;
	char	*mid;
	char	*end;

	j = (*i)++;
	while (tmp[*i] && tmp[*i] != c)
		(*i)++;
	if (!tmp[*i])
		*i = j + 1;
	start = modif_substr(tmp, 0, j, input);
	mid = modif_substr(tmp, j + 1, *i - 1 -j, input);
	if (c == '\"')
		mid = once_more_find_dollar(mid, input, 0);
	end = ft_strjoin(start, mid, input);
	free(start);
	free(mid);
	mid = modif_strdup(tmp + 1 + *i, input);
	start = ft_strjoin(end, mid, input);
	*i = ft_strlen(end);
	free(tmp);
	free(mid);
	free(end);
	return (start);
}

static void	delete_quotes(char **str_command, t_input *input)
{
	U_INT		i;
	char		*cut;
	char		*start;
	char		*end;

	i = 0;
	cut = modif_strdup(*str_command, input);
	while (cut[i])
	{
		if (cut[i] == '\'' || cut[i] == '\"')
			{
				cut = delete_quotes2(cut, &i, input, cut[i]);
				i = 0; //!!!!!!
			}
		else if (cut[i] == '$')
			{
				cut = find_dollar(cut, &i, input);
				i = 0; //!!!!!!
			}
		else
			i++;
	}
	free(*str_command);
	*str_command = cut;
}
void clean_command(t_input *input)
{
	U_INT   i;
	t_comm  *copy;

	copy = input->command;
	while (copy)
	{
		i = 0;
		while (copy->words[i])
		{
			// printf("1before delete quotes: %s\n", copy->words[i]);
			delete_quotes(&copy->words[i], input);
			// printf("pipe after delete quotes: %s\n", copy->words[i]);
			i++;
		}
		copy = copy->next;
	}
	choose_build(input->command);
}

void clean_direct(t_input *input)
{
	U_INT i;
	t_direct	*copy;

	copy = input->direct;
	while (copy)
	{
		i = 0;
		while (copy->name[i])
		// while (copy->name)
		{
			delete_quotes(&copy->name, input);
			// printf("redirect after delete quotes: %s\n", copy->name);
			i++;
		}
		copy = copy->next;
	}
	// free(copy);
	add_heredoc(input);// надо ли это вообще???
	// mark_direct(input);
}