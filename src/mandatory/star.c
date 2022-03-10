/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 23:26:00 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/11 00:22:56 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strstr(char *haystack, char *needle)
{
	U_INT	i;
	U_INT	j;
	U_INT	len;

	len = ft_strlen(haystack);	
	i = 0;
	j = 0;
	printf("ned = %s, hay = %s\n", needle, haystack);
	if (ft_strlen(needle) > len)
		return (NULL);
	if (haystack[i] & needle[i] && haystack[i] == needle[i]) //???? может и без этого работает
	{
		while (haystack[i] && needle[i] && haystack[i] == needle[i])
			i++;
			// printf("1\n");
		if (needle[i])
			return (NULL);
		else
			return (&haystack[i]);
			// printf("2\n");
	}
	while (i < len && haystack[i + j] != '\0')
	{
		// printf("3\n");
		while (haystack[i + j] == needle[j] && ((i + j) < len) && \
				needle[j] != '\0')
			j++;
			// printf("4\n");
		if (!needle[j])
			return (&haystack[i]);
		if (haystack[i + j] != '\0' && needle[j] == '\0')
			return (NULL);
			// printf("5\n");
		if (j == ft_strlen(needle))
			return (&haystack[i]);
		i++;
		j = 0;
		// printf("6\n");
	}
	return (NULL);
}

// static char	*check_star(t_input *input, U_INT *k)
// {
// 	U_INT	m;
// 	U_INT	i;
// 	char	**new_arr;
// 	U_INT	start;

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


void find_star_in_comm(t_input *input) // заполняет input->star
// почистить в конце всей программы эту новую структуру!!!!!
{
	DIR					*dir;
    struct				dirent *entry;
	t_env				*new;

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
}


char	*change_star(t_comm **tmp, t_input *input, U_INT *k)
{
	t_env	*copy;
	t_env	*min;
	t_comm		*orig;
	
	orig = *tmp;
	copy = input->star;
	if (*k == 0)
	{
		min = copy;
		while (copy)
		{
			if (modif_strncmp(copy->key, min->key, 1) > 0)
				min = copy;
			copy = copy->next;
		}
		orig->words[*k] = modif_strdup(min->value, input);
		(*k)++;
		return (modif_strdup(min->value, input));
	}
	else
		{
			while (copy)
			{
				orig->words[*k] = modif_strdup(copy->value, input);
				(*k)++;
				copy = copy->next;
			}
			return (orig->words[*k]);

		}
}


// t_status	start_strstr(char *haystack, char *needle)
// {
// 	U_INT	i;
// 	U_INT	j;
// 	U_INT	len;

// 	len = ft_strlen(haystack);	
// 	i = 0;
// 	j = 0;
// 	printf("ned = %s, hay = %s\n", needle, haystack);
// 	if (ft_strlen(needle) > len)
// 		return (NULL);
// 	if (haystack[i] & needle[i] && haystack[i] == needle[i]) //???? может и без этого работает
// 	{
// 		while (haystack[i] && needle[i] && haystack[i] == needle[i])
// 			i++;
// 			// printf("1\n");
// 		if (needle[i])
// 			return (NULL);
// 		else
// 			return (&haystack[i]);
// 			// printf("2\n");
// 	}
// 	while (i < len && haystack[i + j] != '\0')
// 	{
// 		// printf("3\n");
// 		while (haystack[i + j] == needle[j] && ((i + j) < len) && \
// 				needle[j] != '\0')
// 			j++;
// 			// printf("4\n");
// 		if (!needle[j])
// 			return (&haystack[i]);
// 		if (haystack[i + j] != '\0' && needle[j] == '\0')
// 			return (NULL);
// 			// printf("5\n");
// 		if (j == ft_strlen(needle))
// 			return (&haystack[i]);
// 		i++;
// 		j = 0;
// 		// printf("6\n");
// 	}
// 	return (NULL);
// }

// // с s до *i мы ходим по vrem 
// // с s до *i мы ходим по copy->value 
// void letter_in_start(t_comm **tmp, U_INT *k, char *vrem, t_input *input)
// {
// 	char *tmp;
// 	U_INT s;
// 	t_env *copy;

// 	s = *i;
// 	while (vrem[*i] && vrem[*i] != '*')
// 		(*i)++;
// 	if (*i - s > 0)
// 	{
// 		tmp = modif_substr(vrem, s, *i - s, input); // ls
// 		copy = input->star;
// 		while (copy)
// 		{
// 			printf("ft_strstr(copy->value, tmp) = %s\n", ft_strstr(copy->value, tmp));
// 			if (start_strstr(copy->value, tmp) != NULL)
// 			{
// 				while (vrem[i])
// 				if (vrem + *i && vrem + *i == '*')
// 				{
// 					(*i)++;
// 					if (vrem + *i)
// 						letter_in_middle_or_end();
// 				}
				
// 				orig->words[*k] = modif_strdup(copy->value, input);
// 				// printf("~!!!!orig->words[*k = %d] = %s\n", *k, orig->words[*k]);
// 				(*k)++;
// 				flag = 1;
// 			}
// 			copy = copy->next;
// 		}
// 		if (flag == 0)
// 		{
// 			orig->words[*k] = modif_strdup(vrem, input);
// 			(*k)++;
// 		}	


// 	}




// 	tmp = 
// }


// void	star_in_word(t_comm **tmp, U_INT *k, char *vrem, t_input *input)
// {
// 	U_INT		i;
// 	U_INT		m;
// 	char		*template;
// 	t_env		*copy;
// 	t_status	flag;
// 	t_comm		*orig;

// 	orig = *tmp;
// 	flag = 0;
// 	i = 0;
// 	if (vrem[0] != '*')
// 		letter_in_start();
// 	else
// 		common_temp(tmp, k, vrem, input);
// }
// 		while (vrem[i])
// 		{
// 			if (vrem[i] == '*' && i != ft_strlen(vrem) - 1)
// 		}
// 	}
// 	while (vrem[i] && vrem[i] == '*')
// 		i++;
// 	if (vrem[i]) // кроме звезды в начале есть что-то еще
// 	{
// 		// printf("во vrem НЕ только звезда, k = %d\n", *k);
// 		m = i;
// 		while (vrem[i] && vrem[i] != '*')
// 			i++;
// 		template = modif_substr(vrem, m, i - m, input);
// 		printf("template = %s\n", template);
// 		copy = input->star;
// 		while (copy)
// 		{
// 			printf("ft_strstr(copy->value, template) = %s\n", ft_strstr(copy->value, template));
// 			if (ft_strstr(copy->value, template) != NULL)
// 			{
// 				orig->words[*k] = modif_strdup(copy->value, input);
// 				// printf("~!!!!orig->words[*k = %d] = %s\n", *k, orig->words[*k]);
// 				(*k)++;
// 				flag = 1;
// 			}
// 			copy = copy->next;
// 		}
// 		if (flag == 0)
// 		{
// 			orig->words[*k] = modif_strdup(vrem, input);
// 			(*k)++;
// 		}	
// 	}
// 	else
// 	{
// 		// printf("во vrem только звезда, k = %d\n", *k);
// 		// tmp->words[*k] = change_star(tmp, input, k); // нашли в слове только * или ** 
// 		change_star(tmp, input, k);
// 		// {
// 			// if (*k == 0)
// 			// 	orig->words[*k + 1] = NULL;
// 		// }
// 	}
// 	orig->words[*k] = NULL;
// }

void add_list_in_templ(t_templ	*templ, char *str, t_input *input, U_INT num)
{
	U_INT	i;
	U_INT	m;
	t_templ	*copy;

	copy = templ;
	i = 0;
	// printf("str[i = %d] = %s\n", i, str + i);
	while (str[i])
	{
		m = i;
		if (str[0] != '*')
		{
			printf("status = 1\n");
			copy->status = 1;
			while (str[i] && str[i] != '*')
				i++;
			copy->value = modif_substr(str, m, i - m, input);
			printf("copy->value = %s\n", copy->value);
			if (str + i + 1)
				copy = copy->next;
			else
				copy->next = NULL;
			i++;
		}
		printf("1\n");
		if (str[0] == '*')
		{
			// printf("status = 3\n");
			copy->status = 3;
			i = 1;
			m = 1;
			while (str[i] && str[i] != '*')
				i++;
			copy->value = modif_substr(str, m, i - m, input);
			// printf("copy->value = %s\n", copy->value);
			if (str[i + 1])
				copy = copy->next;
			else
				copy->next = NULL;
			i++;
		}
		else if (str[i])
		{
			printf("1,5; str + i = %s\n", str + i);
			while (str[i] && str[i] != '*')
				i++;
			copy->value = modif_substr(str, m, i - m, input);
			// printf("not 1 status; copy->value = %s\n", copy->value);
			if (!str[i])
				{
					// printf("status = 3\n");
					copy->status = 3;
					copy->next = NULL;
				}
			else
			{
				// printf("status = 2\n");
				copy->status = 2;
				copy = copy->next;
			}
		}
		printf("2\n");
		// printf("copy->value = %s, copy->status = %u\n", copy->value, copy->status);
	}
}


t_templ	*create_template_list(char *str, t_input *input)
{
	U_INT	num;
	U_INT	i;
	t_templ	*templ;
	t_templ	*copy;

	num = 0;
	i = 0;
	templ = NULL;
	while (str[i])
	{
		if (str[i] != '*')
			num++;
		while (str[i] && str[i] != '*')
			i++;
		while (str[i] && str[i] == '*')
			i++;
	}
	// printf("str = %s, list size = %u\n", str, num);
	while (num--)
	{
		copy = malloc(sizeof(t_templ));
		copy->next = NULL;
		if (!templ)
			templ = copy;
		else
			lstadd_back(&templ, copy);
	}
	// printf("dddddd\n");
	add_list_in_templ(templ, str, input, num);
	return(templ);
}

void free_temple(t_templ *temple)
{
	t_templ	*copy;

	while (temple)
	{
		copy = temple;
		temple = temple->next;
		printf("очистка листка с шаблоном: copy->value = %s\n", copy->value);
		free(copy->value);
		free(copy);
	}
}

t_status	check_template(t_input *input, char *str, t_templ *temple)
{	
	t_status	flag; // 0 - start, 1 - not, 2 - finish
	// U_INT		start;
	t_templ	*copy_templ;
	t_templ	*last;
	U_INT	i;

	// start = 0;
	i = 0;
	flag = 0;
	copy_templ = temple;
	while (copy_templ && flag == 0)
	{
		if (copy_templ->status == 1) // start word
    	{
			printf("ищем статус 1\n");
			if (copy_templ->value[0] == str[0] && 
			ft_strncmp(copy_templ->value, str, ft_strlen(copy_templ->value)) == 0)
			{
				i += ft_strlen(copy_templ->value);
				printf("успех! у нас совпало начало и строки! Начнем дальше с i = %d, str[i] = %s\n", i, str + i);
				if (copy_templ->next)
					copy_templ = copy_templ->next;
				else
					return (success);
				// start = ft_strlen(copy_templ->value) - 1; // -1 is ok????????
			}
			else
				flag = 1;
			printf("прошли статус 1. Флаг = %d\n", flag);
		}	
		else if (copy_templ->status == 2)
    	{
			// printf("ищем статус 2\n");
			// printf("462!!! смотрим на str + %u = %s\n", i, str + i); 
			// printf("463!!! copy_templ->value = %s\n", copy_templ->value); 
			while (str + i && copy_templ && copy_templ->value)
			{
				// printf("466: смотрим на str + %u = %s\n", i, str + i); 
				// printf("ft_strlen(copy_templ->value) = %u\n", ft_strlen(copy_templ->value));
				// printf("2\n");
				if (ft_strncmp(copy_templ->value, str + i, ft_strlen(copy_templ->value)) == 0)
					{
						i += ft_strlen(copy_templ->value);
						last = copy_templ;
						// printf("last->value = %s, i = %u, str + i = %s\n", last->value, i, str + i);
						copy_templ = copy_templ->next;
					}
				else
					++i;
			}
			if (copy_templ)
				flag = 1;
		}
		else if (copy_templ->status == 3)
		{
			// printf("ищем статус 3\n");
			while (str[i] && copy_templ && copy_templ->value)
			{
				// printf("466: смотрим на str + %u = %s\n", i, str + i); 
				// printf("ft_strlen(copy_templ->value) = %u\n", ft_strlen(copy_templ->value));
				// printf("2\n");
				if (ft_strncmp(copy_templ->value, str + i, ft_strlen(copy_templ->value)) == 0)
					{
						// printf("!!!str + i = %s\n", str + i);
						i += ft_strlen(copy_templ->value);
						// printf("222!!!str + i = %s\n", str + i);
						// copy_templ = copy_templ->next;
						// if (str + i)
						// 	++i;
						if (!str[i])
						{
							printf("33!!!str + i = %s\n", str + i);
							last = copy_templ;
							printf("last->value = %s, i = %u\n", last->value, i);
							return (success);
						}
					}
				else
					++i;
			}
			if (copy_templ || (str && str + i))
				flag = 1;
			else
				flag = 2;
		}
	// printf("finish : Флаг = %d\n", flag);
	}
	// free_temple(temple);
	if (flag == 2)
		return (success);
	else
		return (fail);
}

char **big_circle(t_templ *temple, t_input *input, U_INT *len)
{
	char 	**addition;
	t_env	*copy; //copy_star;
	// t_templ	*copy_terml;
	// U_INT	i; //ходим по массиву addition
	
	printf("lstsize = %u, *len = %u\n", ft_lstsize(input->star), *len);
	addition = malloc(sizeof(char *) * ft_lstsize(input->star));
	copy = input->star;
	// while (*len < ft_lstsize(input->star))
	// {
		while (copy)
		{
			if (check_template(input, copy->value, temple) == success)
				{
					printf("505 - copy->value = %s\n", copy->value);
					addition[*len] = modif_strdup(copy->value, input);
					printf("записали в новый массив: addition[i = %d] = %s\n", *len, addition[*len]);
					++(*len);
				}
			copy = copy->next;
		}
		addition[*len] = NULL;
		// free_temple(&temple);
	// }
	return (addition);
}


char **realloc_words(char **old, t_input *input, U_INT k, t_templ *templ)
{
	char 		**res;
	U_INT 		i; // индексация old **
	char		**addition;
	U_INT len;
	U_INT		j; // индексация res
	U_INT		n;
	U_INT		m;

	// copy = input->star;
	i = 0;
	j = 0;
	n = 0;
	len = 0;
	m = 0;

	while (old[i])
	{
		// printf("\n\n\nold[i = %u] = %s\n\n\n", i, old[i]);
		while (ft_strchr(old[i], '*') == NULL)
			++i;
		if (old[i] && ft_strchr(old[i], '*') != NULL)
		{
			addition = big_circle(templ, input, &len);
			printf("579^ res size = %u ----- %u(k) + %u(len)\n", (k + len), k, len);
			printf("579^ addition[0] = %s)\n", addition[0]);
			res = malloc(sizeof(char *) * (k + len));
			n = 0;
			while (n != i)
			{
				res[j++] = modif_strdup(old[n], input);
				// printf("res[j = %d] = %s; old[n = %d] = %s\n", j - 1, res[j - 1], n, old[n]);
				free(old[n]);
				++n;
			}
			// printf("n = %d, i = %u, addition[m = %d] = %s\n", n, i, m, addition[m]);
			if (n == i && addition[m])
			{

				res[j++] = addition[m++];
				// printf("res[j = %u] = %s, addition[m = %u] = %s\n", j - 1, res[j - 1], m - 1, addition[m - 1]);
				free(old[i]);
				++i;
			}
			// while (old[i])
			// {
			// 	res[j++] = modif_strdup(old[i], input);
			// 	printf("res[j = %d] = %s\n", j - 1, res[j - 1]);
			// 	free(old[i]);
			// 	++i;
			// }
		}
	}
	res[j] = NULL;
	free(old);
	return(res);
}


void	find_star(t_comm *tmp, t_input *input, U_INT k)
{
	U_INT	i;
	t_templ	*templ;

	i = 0;
	// printf("!ищем звезду: tmp->words[0] = %s, k = %u\n", tmp->words[0], k);
	while (tmp && tmp->words && tmp->words[i])
	{	
		printf("!ищем звезду: tmp->words[i = %u] = %s, k = %u\n", i, tmp->words[i], k);
		// if (ft_strchr(tmp->words[i], '*') == NULL)
		// 	{
		// 		// printf("нет звездыу: tmp->words[i = %u] = %s\n", i, tmp->words[i]);
		// 		++i;
		// 		continue ;
		// 	}
		// else if (ft_strlen(tmp->words[i]) > 1)
		if (ft_strchr(tmp->words[i], '*') != NULL && ft_strlen(tmp->words[i]) > 1)
		{
			printf("1\n");
			// printf("есть звездыу: tmp->words[i = %u] = %s\n", i, tmp->words[i]);
			templ = create_template_list(tmp->words[i], input);
			tmp->words = realloc_words(tmp->words, input, k, templ);
			free_temple(templ);
			printf("2\n");
			// i = 0;
		}
		else if (ft_strchr(tmp->words[i], '*') != NULL && ft_strlen(tmp->words[i]) == 1)
			{
				printf("3\n");
				printf("заглушка для случая, когда пришла только *\n");
			// tmp->words = push_all_files();
			printf("4\n");
			}
		printf("647 - ищем звезду: tmp->words[i = %u] = %s, k = %u\n", i, tmp->words[i], k);
		++i;
		printf("5, i = %u\n", i);
	}
}