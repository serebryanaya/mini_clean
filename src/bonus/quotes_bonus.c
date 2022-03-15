/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 18:42:03 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/16 00:11:02 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static inline char	*delete_quotes2(char *tmp, U_INT *i, t_input *input, char c)
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
	mid = modif_substr(tmp, j + 1, *i - 1 - j, input);
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
		if (cut[i] == '\'')
			cut = delete_quotes2(cut, &i, input, cut[i]);
		else if (cut[i] == '\"')
			cut = delete_quotes2(cut, &i, input, cut[i]);
		else if (cut[i] == '$')
			cut = find_dollar(cut, &i, input);
		else
			i++;
	}
	free(*str_command);
	*str_command = cut;
}

char	**del_elem_from_arr(char **old, U_INT index, t_input *input, U_INT len)
{
	char	**new;
	U_INT	i;
	U_INT	j;

	i = 0;
	j = 0;
	while (old[len])
		len++;
	new = malloc(sizeof(char *) * len);
	while (old[j])
	{
		if (j != index)
		{
			new[i++] = modif_strdup(old[j], input);
			free(old[j]);
		}
		else
			free(old[j]);
		j++;
	}
	free(old);
	new[i] = NULL;
	return (new);
}

void	clean_command(t_input *input)
{
	U_INT	i;
	t_comm	*copy;

	copy = input->command;
	while (copy)
	{
		i = 0;
		while (copy->words[i])
		{
			delete_quotes(&copy->words[i], input);
			if (copy->words[i] && copy->words[i][0] == '\0')
			{
				copy->words = del_elem_from_arr(copy->words, i, input, 0);
			}
			else
				i++;
		}
		copy = copy->next;
	}
	choose_build(input->command);
}

void	clean_direct(t_input *input)
{
	U_INT		i;
	t_direct	*copy;

	copy = input->direct;
	while (copy)
	{
		i = 0;
		while (copy->name[i])
		{
			delete_quotes(&copy->name, input);
			i++;
		}
		copy = copy->next;
	}
	add_heredoc(input);
	mark_direct(input);
}
