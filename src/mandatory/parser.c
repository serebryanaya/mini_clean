/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 20:21:05 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/07 22:40:28 by pveeta           ###   ########.fr       */
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
	}
}

static char	*find_star_in_comm(t_input *input, char *str, U_INT j, U_INT *i)
{
	DIR					*dir;
    struct				dirent *entry;
	unsigned long long	min;
	char				*name;


	if (str[*i + 1] && str[*i + 1] != ' ')
		return (NULL);
	dir = opendir(getcwd(NULL, 0));
	min = 0;
	name = NULL;
    if (!dir)
	{
        perror("diropen");
        exit(1);
    };
    while (1)
	{
        entry = readdir(dir);
		if (entry != NULL)
		{
			if ((entry->d_ino < min || min == 0) && entry->d_name[0] != '.')
				{
					min = entry->d_ino;
					free(name);
					name = modif_strdup(entry->d_name, input);
					printf("min = %lld\n", min);
				}
			printf("%lld - %s [%d] %d\n",
            	entry->d_ino, entry->d_name, entry->d_type, entry->d_reclen);
			// free(entry);
		}
		else
		{
			printf("конец!\n");
			break ;
		}

    }
    closedir(dir);
	(*i)++;
	printf("name = %s\n", name);
    return (name);
}


static void	add_command(t_input *input, char *str, U_INT j, U_INT *i)
{
	t_comm	*tmp;//cmd
	U_INT	k; //num
	U_INT	m; //j
	char	*path;

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
			// printf("нашел галочку\n");
		else if (str[*i] == '*')
		{
			path = find_star_in_comm(input, str, j, i);
			if (path)
			{
				tmp->words[k++] = path;
				tmp->words[k++] = NULL;
			}
		}
			// printf("нашел галочку\n");
		else
		{
			m = *i;
			go_through_word(str, i, input);
			tmp->words[k] = modif_substr(str, m, *i - m, input);
			// printf("tmp->words[%u] = %s\n", k, tmp->words[k]);
			k++;
			tmp->words[k] = NULL;
		}
	}
	// if (!str[*i] && k == 0)
	// {
	// 	tmp->words[k] = modif_substr(str, m, *i - m, input);
	// 	printf("tmp->words[%u] = %s\n", k, tmp->words[k]);
	// }
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
