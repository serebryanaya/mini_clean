/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 20:21:05 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/08 00:19:52 by pveeta           ###   ########.fr       */
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

static void find_star_in_comm(t_input *input, char *str, U_INT j, U_INT *i)
{
	DIR					*dir;
    struct				dirent *entry;
	t_env				*new;

	if (str[*i + 1] && str[*i + 1] != ' ')
		return ;
	dir = opendir(getcwd(NULL, 0));
    if (!dir)
	{
        perror("diropen");
        exit(1);
    }
    while (1)
	{
        entry = readdir(dir);
		if (entry != NULL)
		{
			if (entry->d_name[0] != '.')
				{
					new = create_new_list(ft_strjoin_for_3\
						(modif_itoa((int)entry->d_ino, input), "=", modif_strdup(entry->d_name, input), input), input);
					if (!input->star)
						input->star = new;
					else
						add_list_back(new, &input->star);
				}
			// printf("%lld - %s [%d] %d\n",
            // 	entry->d_ino, entry->d_name, entry->d_type, entry->d_reclen);
			// if (new)
			// printf("new = %s %s\n",
            // 	new->key, new->value);
			// // free(entry);
		}
		else
			break ;
    }
    closedir(dir);
	(*i)++;
}

int	modif_strncmp(char *str1, char *str2, int n)
{
	unsigned int	i;
    int             len1;
    int             len2;

    len1 = ft_strlen(str1);
    len2 = ft_strlen(str2);
    if (len1 > len2)
        n = len1;
    else
        n = len2;
	i = 0;
	while (str1[i] == str2[i] && str1[i] != '\0' && str2[i] != '\0' && i < n)
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)str2[i] - (unsigned char)str1[i]);
}

// static char	*check_star(t_input *input, U_INT *k)
// {
// 	U_INT	m;
// 	U_INT	i;
// 	char	**new_arr;

// 	i = 1;
// 	while (i <= k)
// 	{
// 		if (ft_strcmp(input->command->words[i], "*") != success)
// 			i++;
// 		else
// 		{
// 			m = ft_lstsize(input->star) + k;
// 			new_arr = malloc(sizeof(char *) * m);
// 		}
// 		input->command->words[i] != 
// 		i++;
// 	}
// 	m = ft_lstsize(input->star) + k;
// 	new_arr = malloc(sizeof(char *) * m);


// }

// char	*change_star(t_input *input, U_INT *i)
// {
// 	char	*command;
// 	t_env	*copy;
// 	t_env	*min;
	
// 	copy = input->star;
// 	if (*i == 0)
// 	{
// 		min = copy;
// 		while (copy)
// 		{
// 			if (modif_strncmp(copy->key, min->key, 1) > 0)
// 				min = copy;
// 			copy = copy->next;
// 		}
// 		return (min->value);
// 	}
// 	else
// 		return (check_star(input, k));
	// while (copy)
	// {

	// 	copy = copy->next;
	// }

// }

// int	ft_lstsize(t_list *lst)
// {
// 	U_INT	len;

// 	len = 0;
// 	while (lst != NULL)
// 	{
// 		lst = lst->next;
// 		len++;
// 	}
// 	return (len);
// }

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
		

		
		/* !!!Вот эту часть надо куда-то переноить, где выделяют память под слова
		else if (str[*i] == '*')
		{
			find_star_in_comm(input, str, j, i);
			tmp->words[k] = change_star(input, &k);
			// printf("tmp->words[k - 1] = %s\n", tmp->words[k - 1]);
			k++;
			tmp->words[k++] = NULL;
		}
		*/



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
