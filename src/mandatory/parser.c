/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 20:21:05 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/09 22:39:33 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	go_through_word(char *str, U_INT *i, t_input *input)
{
	while (str[*i] && str[*i] != '>' && str[*i] != '<' \
		&& str[*i] != '|' && str[*i] != ' ')
	{
		if (str[*i] == '\'')
			go_to_end_quote(str, i, '\'', input);
		else if (str[*i] == '\"')
			go_to_end_quote(str, i, '\"', input);
		else
			(*i)++;
	}
}

// static inline void	add_command2(t_input *input, char *str, U_INT *j, U_INT *i)
// {
// 	if (str[*i] == ' ')
// 		(*i)++;
// 	else if (str[*i] == '>' || str[*i] == '<')
// 		find_direct_in_comm(input, str, *j, i);
// }

// static inline void	add_command3(t_input *input, char *vrem, t_comm	**tmp, U_INT *k)
// {
// 	if (ft_strchr(vrem, '*') == NULL)
// 	{
// 		(*tmp)->words[*k] = modif_strdup(vrem, input);
// 		(*k)++;
// 		(*tmp)->words[*k] = NULL;
// 	}
// 	else
// 		star_in_word(tmp, k, vrem, input);
// }

// static void	add_command(t_input *input, char *str, U_INT j, U_INT *i)
// {
// 	t_comm	*tmp;
// 	U_INT	k;
// 	U_INT	m;
// 	char	*vrem;

// 	m = 0;
// 	k = 0;
// 	tmp = input->command;
// 	while (m++ < j)
// 		tmp = tmp->next;
// 	while (str[*i] && str[*i] != '|')
// 	{
// 		if (str[*i] == ' ' || str[*i] == '>' || str[*i] == '<')
// 			add_command2(input, str, &j, i);
// 		else
// 		{
// 			m = *i;
// 			go_through_word(str, i, input);
// 			vrem = modif_substr(str, m, *i - m, input);
// 			(*i)++; //???
// 			add_command3(input, vrem, &tmp, &k);
// 			// if (ft_strchr(vrem, '*') == NULL)
// 			// {
// 			// 	tmp->words[k] = modif_strdup(vrem, input);
// 			// 	k++;
// 			// 	tmp->words[k] = NULL;
// 			// }
// 			// else
// 			// 	star_in_word(&tmp, &k, vrem, input);
// 			free(vrem);
// 		}
// 	}
// }

static void	add_command(t_input *input, char *str, U_INT j, U_INT *i)
{
	t_comm	*tmp;//cmd
	U_INT	k; //num
	U_INT	m; //j
	char	*vrem;

	m = 0;
	k = 0;
	tmp = input->command;
	while (m++ < j)
		tmp = tmp->next;
	while (str[*i] && str[*i] != '|')
	{
		if (str[*i] == ' ')
			(*i)++;
		else if (str[*i] == '>' || str[*i] == '<')
			find_direct_in_comm(input, str, j, i);
		else
		{
			m = *i;
			go_through_word(str, i, input);
			tmp->words[k] = modif_substr(str, m, *i - m, input);
			// printf("tmp->words[k = %d] = %s\n", k, tmp->words[k]);
			k++;
			tmp->words[k] = NULL;
			// vrem = modif_substr(str, m, *i - m, input);
			// (*i)++; //???
			// printf("vrem = %s\n", vrem);
			// if (ft_strchr(vrem, '*') == NULL)
			// {
			// 	tmp->words[k] = modif_strdup(vrem, input);
			// 	printf("нет звездыб tmp->words[ k = %d] = %s\n", k, vrem);
			// 	k++;
			// 	tmp->words[k] = NULL;
			// }
			// else
			// 	{
			// 		printf("есть звезда: tmp->words[ k = %d] = %s\n", k, vrem);
			// 		star_in_word(&tmp, &k, vrem, input);
			// 		// if (k == 0)
			// 		// 	{
			// 		// 		input->stop = 1;
			// 		// 		tmp->words[k + 1] = NULL;		
			// 		// 		while (str[*i] && str[*i] != '|')
			// 		// 			(*i)++;				
			// 		// 	}			

			// 	}
			// // if (input->stop == 1)
			// // {
			// // 	while (str[*i] && str[*i] != '|')
			// // 	(*i)++;
			// // }
			// free(vrem);
			// int z = 0;
			// while (z <= k)
			// {
			// 	printf("ПРОВЕРКА - tmp->words[z = %u] = %s, k = %d\n", z, tmp->words[z], k);
			// 	z++;
			// }
		// 	tmp->words[k] = modif_substr(str, m, *i - m, input);
		// 	// printf("tmp->words[%u] = %s\n", k, tmp->words[k]);
		// 	k++;
		// 	tmp->words[k] = NULL;
		// }
	}
	find_star(tmp, input, k);

	// if (!str[*i] && k == 0)
	// {
	// 	tmp->words[k] = modif_substr(str, m, *i - m, input);
	// 	printf("tmp->words[%u] = %s\n", k, tmp->words[k]);
	// }
}
}

// t_status	parser(char *str_command, t_input *input, U_INT i, U_INT j)
// {
// 	t_status	flag;

// 	flag = create_empty_t_comm(str_command, input);
// 	if (flag == fail)
// 	{
// 		print_error(input, 12, "malloc", NULL);
// 		return (fail);
// 	}
// 	else if (flag == end)
// 	{
// 		input->token = modif_strdup("newline", input);
// 		print_token(input);
// 		return (end);
// 	}
// 	while (str_command[i])
// 	{
// 		if (str_command[i] != '|')
// 			add_command(input, str_command, j, &i);
// 		else
// 		{
// 			i++;
// 			j++;
// 		}
// 	}
// 	return (success);
// }

t_status	parser(char *str_command, t_input *input, U_INT i, U_INT j)
{
	// U_INT		i;
	// U_INT		j;
	t_status	flag;

	i = 0;
	j = 0; // число пайпов
	flag = create_empty_t_comm(str_command, input);
	if (flag == fail) // посчтитали пайпы и замаллочили структуру с командами
	{
		print_error(input, 12, "malloc", NULL);
		return (fail);
	}
	else if (flag == end) // посчтитали пайпы и замаллочили структуру с командами
	{
		input->token = modif_strdup("newline", input);
		print_token(input);
		return (end);
	}
	while (str_command[i])
	{
		if (str_command[i] != '|')
		{
			add_command(input, str_command, j, &i); // заполняем структуры (разделитель - пайп)
			// printf("t_status parser\n");
		}
		else
		{
			i++;
			j++;
		}
	}
	return (success);
}