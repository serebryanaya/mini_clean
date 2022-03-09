/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 20:21:05 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/09 21:04:43 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	go_through_word(char *str, U_INT *i, t_input *input)
{
	// while (str[*i] == ' ')
	// 	(*i)++;
	while (str[*i] && str[*i] != '>' && str[*i] != '<' \
		&& str[*i] != '|' && str[*i] != ' ')
	{
		if (str[*i] == '\'')
			go_to_end_quote(str, i, '\'', input);
		else if (str[*i] == '\"')
			go_to_end_quote(str, i, '\"', input);
		else
			(*i)++;
			//!!!!сюда добавить обработку звезды в середине или конце файла и пересчет каунтера
	}
}


// static int	modif_strncmp(char *str1, char *str2, int n)
// {
// 	unsigned int	i;
//     int             len1;
//     int             len2;

//     len1 = ft_strlen(str1);
//     len2 = ft_strlen(str2);
//     if (len1 > len2)
//         n = len1;
//     else
//         n = len2;
// 	i = 0;
// 	while (str1[i] == str2[i] && str1[i] != '\0' && str2[i] != '\0' && i < n)
// 		i++;
// 	if (i == n)
// 		return (0);
// 	return ((unsigned char)str2[i] - (unsigned char)str1[i]);
// }






// static void	find_star(t_comm *tmp, t_input *input, U_INT counter)
// {
// 	t_comm	*copy;
// 	U_INT	i;

// 	copy = malloc(sizeof());
// 	while (i <= counter)
// 	{

// 	}
// 	// if (tmp->words[0] == '*')
// 	// {

// 	// }

// }

// static void	add_word(t_comm	*tmp, char *str, U_INT *i, t_input *input)
// {
// 	U_INT	m; //j

// 	m = 0;
// 	m = *i;
// 	while (str[*i] == ' ')
// 		(*i)++;
// 	if (str[*i] == '*')
// 		star_in_start();
// 	go_through_word(str, i, input);
// }

char	*ft_strchr(char *str, char c)
{
	U_INT	i;

	i = 0;
	while (*(str + i) != 0)
	{
		if (*(str + i) != c)
			i++;
		else
			return (str + i);
	}
	return (NULL);
}



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
	// find_star(tmp, input, k);

	// if (!str[*i] && k == 0)
	// {
	// 	tmp->words[k] = modif_substr(str, m, *i - m, input);
	// 	printf("tmp->words[%u] = %s\n", k, tmp->words[k]);
	// }
}


// t_status	search_in_dir()
// {
	
}

t_status	parser(char *str_command, t_input *input)
{
	U_INT		i;
	U_INT		j;
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
