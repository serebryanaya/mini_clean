/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 23:26:00 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/12 22:45:24 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_new_env(t_env **new, DIR *dir, struct dirent *entry, t_input *input)
{
	*new = create_new_list(ft_strjoin_for_3 \
		(modif_itoa((int)entry->d_ino, input), "=", \
		modif_strdup(entry->d_name, input), input), input);
	(*new)->equal = 0;
	if (!input->star)
		input->star = *new;
	else
		add_list_back(*new, &input->star);
	return(*new);
}

void	find_star_in_comm(t_input *input)
{
	DIR				*dir;
	struct dirent	*entry;
	t_env			*new;

	dir = opendir(getcwd(NULL, 0));
	if (!dir)
		return ;
	while (1)
	{
		entry = readdir(dir);
		if (entry != NULL)
		{
			if (entry->d_name[0] != '.')
				// new = create_new_env(&new, dir, entry, input);
			{
			new = create_new_list(ft_strjoin_for_3 \
			(modif_itoa((int)entry->d_ino, input), "=", \
			modif_strdup(entry->d_name, input), input), input);
			new->equal = 0;
			if (!input->star)
				input->star = new;
			else
				add_list_back(new, &input->star);
			}
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

void add_list_in_templ(t_templ	*templ, char *str, t_input *input, U_INT num)
{
	U_INT	i;
	U_INT	m;
	t_templ	*copy;
	U_INT	n;

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
				++i;
			copy->value = modif_substr(str, m, i - m, input);
			printf("copy->value = %s\n", copy->value);
			while (str[i] && str[i] == '*')
				++i;
			--i;
			if (str[i])
				copy = copy->next;
			else
				copy->next = NULL;
			// i++;
		}
		// printf("1\n");
		if (str[i] == '*')
		{
			while (str[i] == '*')
				++i;
			n = i;
			m = i;
			copy->status = 3;
			while (str[n])
			{
				if (str[n++] == '*')
					copy->status = 2;
			}
			printf("status = %u, str + i = %s\n", copy->status, str + i);
			// i = 1;
			// m = 1;
			while (str[i] && str[i] != '*')
				i++;
			copy->value = modif_substr(str, m, i - m, input);
			printf("copy->value = %s\n", copy->value);
			if (str[i + 1])
				copy = copy->next;
			else
				copy->next = NULL;
			
		}
		++i;
		
		// else if (str[i])
		// {
		// 	printf("1,5; str + i = %s\n", str + i);
		// 	while (str[i] && str[i] != '*')
		// 		i++;
		// 	copy->value = modif_substr(str, m, i - m, input);
		// 	// printf("not 1 status; copy->value = %s\n", copy->value);
		// 	if (!str[i])
		// 		{
		// 			// printf("status = 3\n");
		// 			copy->status = 3;
		// 			copy->next = NULL;
		// 		}
		// 	else
		// 	{
		// 		// printf("status = 2\n");
		// 		copy->status = 2;
		// 		copy = copy->next;
		// 	}
		// }
		printf("2\n");
		// printf("copy->value = %s, copy->status = %u\n", copy->value, copy->status);
	}
}

t_status	only_star(char *str)
{
	U_INT		i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '*')
			return (success);
		++i;
	}
	return (fail);
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
	printf("str = %s, list size = %u\n", str, num);
	while (num--)
	{
		copy = malloc(sizeof(t_templ)); // защитить маллоки!!!!
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

int check_status_one(t_templ **copy_templ, U_INT *i, char *str) // не работает ls m**e - он не идет проверять е, останавливается на проверке m
{
	t_templ	*last;

	// last = *copy_templ;
	printf("ищем статус 1\n");
	if ((*copy_templ)->value[0] != str[0] || 
	ft_strncmp((*copy_templ)->value, str, ft_strlen((*copy_templ)->value)) != 0)
		return (fail);
	*i += ft_strlen((*copy_templ)->value);
	printf("успех! у нас совпало начало строки! Начнем дальше с i = %d, *copy_templ = %s, str[i] = %s\n", *i, (*copy_templ)->value, str + *i);
	if (!(*copy_templ)->next)
	{
		printf("\n\nуспех! у нас совпало начало и строки! Выходим. str[i] = %s\n", str + *i);
		return (success);
	}

	if ((*copy_templ)->next)
		{
			printf("last->next = %s\n", (*copy_templ)->next->value);
			*copy_templ = (*copy_templ)->next;
		}
	return (3);
}

int check_status_two(t_templ **copy_templ, U_INT *i, char *str)
{
	t_templ	*last;

	last = *copy_templ;
	printf("ищем статус 2\n");
	while (str[*i] && last && last->value)
	{
		if (ft_strncmp(last->value, str + *i, ft_strlen(last->value)) == 0)
		{
			*i += ft_strlen(last->value);
			printf("успех! у нас совпало середина! Начнем дальше с i = %d, last->value = %s, str[i] = %s\n", *i, last->value, str + *i);
			if (!last->next)
				return (success);
			else if (last->next)
				last = last->next;
		}
		else
			++(*i);
	}
	if (last)
		{
			// printf("в шаблоне что-то осталось. Выход с ошибкой\n");
			return (fail);
		}
	return (3);
}

int check_status_three(t_templ **copy_templ, U_INT *i, char *str)
{
	t_templ	*last;

	last = *copy_templ;
	printf("\n\nищем статус 3, str + i = %s, last->value = %s\n", str + *i, last->value);
	while (str[*i] && last && last->value)
	{
		if (ft_strncmp(last->value, str + *i, ft_strlen(last->value)) == 0)
		{
			// printf("возможно, слово подойдет!\n");
			*i += ft_strlen(last->value);
			printf("успех! у нас совпало конце строки! Начнем дальше с i = %d, last->value = %s, str[i] = %s\n", *i, last->value, str + *i);
			if (!last->next && !str[*i])
				{
					// printf("слово подощло! last->value = %s\n", last->value);
					return (success);
				}
			else if (last->next)
				last = last->next;
		}
		else
			++(*i);
	}
	if (last || str[*i])
		{
			// printf("в шаблоне или строке что-то осталось. Выход с ошибкой\n");
			return (fail);
		}
	// printf("слово подощло! last->value = %s\n", last->value);
	return (success);
}



t_status	check_template(t_input *input, char *str, t_templ *temple)
{	
	// t_status	flag; // 0 - start, 1 - not, 2 - finish
	// U_INT		start;
	t_templ	*copy_templ;
	t_templ	*last;
	U_INT	i;
	int		itog;

	// start = 0;
	itog = 0;
	i = 0;
	// flag = 0;
	copy_templ = temple;
	while (str[i])
	{
		if (copy_templ->status == 1) // start word
		{
			itog = check_status_one(&copy_templ, &i, str);
			if (itog == fail || itog == success)
				return (itog);
		}
		if (copy_templ->status == 2)
		{
			itog = check_status_two(&copy_templ, &i, str);
			if (itog == fail || itog == success)
				return (itog);
		}
		if (copy_templ->status == 3)
		{
			itog = check_status_three(&copy_templ, &i, str);
			if (itog == fail || itog == success)
				return (itog);
		}
	}
	return (success);
}


U_INT check_tmp_in_dir(t_templ *templ, t_input *input, U_INT len, U_INT i)
{
	t_status	flag;
	// t_templ		*copy_templ;
	t_env		*copy;

	flag = 0;
	copy = input->star;
	// copy_templ = templ;
	while (copy)
	{
		// printf ("результат проверки template = %u\n\n\n\n", check_template(input, copy->value, templ));
		if (check_template(input, copy->value, templ) == success)
			{
				++len;
				copy->equal = 1;
				printf ("результат проверки template; copy->value = %s, copy->equal = %d\n", copy->value, copy->equal);
			}
		copy = copy->next;
	}
	return (len);
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



char **big_circle(t_templ *temple, t_input *input, U_INT size_good_temp)
{
	char 	**addition;
	t_env	*copy; //copy_star;
	// t_templ	*copy_terml;
	U_INT	i; //ходим по массиву addition
	
	i = 0;
	printf("size_good_temp = %u\n", size_good_temp);
	addition = malloc(sizeof(char *) * (size_good_temp));
	copy = input->star;
	// while (*len < ft_lstsize(input->star))
	// {
		while (copy)
		{
			printf("проверяем массив star: copy->value = %s, copy->equal = %d\n", copy->value, copy->equal);	
			if (copy->equal == 1)
				{
					addition[i] = modif_strdup(copy->value, input);
					printf("записали в новый массив: addition[i = %d] = %s\n", i, addition[i]);
					copy->equal = 0;
					++i;
				}
			copy = copy->next;
		}
		addition[i] = NULL;
	return (addition);
}


// char **realloc_words(char **old, t_input *input, U_INT k, t_templ *templ)
// {
// 	char 		**res;
// 	U_INT 		i; // индексация old **
// 	char		**addition;
// 	U_INT len;
// 	U_INT		j; // индексация res
// 	U_INT		n;
// 	U_INT		m;

// 	// copy = input->star;
// 	i = 0;
// 	j = 0;
// 	n = 0;
// 	len = 0;
// 	m = 0;

	
// 	while (old[i])
// 	{
// 		// printf("\n\n\nold[i = %u] = %s\n\n\n", i, old[i]);
// 		while (ft_strchr(old[i], '*') == NULL)
// 			++i;
// 		if (old[i] && ft_strchr(old[i], '*') != NULL)
// 		{
// 			addition = big_circle(templ, input, &len);
// 			printf("579^ res size = %u ----- %u(k) + %u(len)\n", (k + len), k, len);
// 			printf("579^ addition[0] = %s)\n", addition[0]);
// 			res = malloc(sizeof(char *) * (k + len));
// 			n = 0;
// 			while (n != i)
// 			{
// 				res[j++] = modif_strdup(old[n], input);
// 				// printf("res[j = %d] = %s; old[n = %d] = %s\n", j - 1, res[j - 1], n, old[n]);
// 				free(old[n]);
// 				++n;
// 			}
// 			// printf("n = %d, i = %u, addition[m = %d] = %s\n", n, i, m, addition[m]);
// 			if (n == i && addition[m])
// 			{

// 				res[j++] = addition[m++];
// 				// printf("res[j = %u] = %s, addition[m = %u] = %s\n", j - 1, res[j - 1], m - 1, addition[m - 1]);
// 				free(old[i]);
// 				++i;
// 			}
// 			// while (old[i])
// 			// {
// 			// 	res[j++] = modif_strdup(old[i], input);
// 			// 	printf("res[j = %d] = %s\n", j - 1, res[j - 1]);
// 			// 	free(old[i]);
// 			// 	++i;
// 			// }
// 		}
// 	}
// 	res[j] = NULL;
// 	free(old);
// 	return(res);
// }


char **easy_realloc(char **old, t_input *input, U_INT size_good_temp, t_templ *templ)
{
	char 		**res;
	U_INT 		i; // индексация old **
	U_INT		j; // индексация res
	U_INT 		k; //длина old**
	U_INT 		n; //длина old**
	char		**addition;

	j = 0;
	i = 0;
	k = 0;
	n = 0;

	while (old[k])
		++k; // посчитали текущий размер массива слов

	
	res = malloc(sizeof(char *) * (k + size_good_temp + 1)); // нужна ли 1???
	while (old[i] && ft_strchr(old[i], '*') == NULL)
	{
		res[j++] = modif_strdup(old[i], input);
		printf("res[j = %d] = %s; old[i = %d] = %s\n", j - 1, res[j - 1], i, old[i]);
		free(old[i]);
		old[i] = NULL;
		++i;
	}
	if (old[i] && ft_strchr(old[i], '*') != NULL)
	{
		addition = big_circle(templ, input, size_good_temp);
		printf("579^ k = %u, size_good_temp = %u\n", k, size_good_temp);
		printf("579^ addition[0] = %s\n", addition[0]);
		while (addition[n])
		{
			res[j++] = modif_strdup(addition[n], input);
			printf("res[j = %d] = %s; addition[n] = %d] = %s\n", j - 1, res[j - 1], n, addition[n]);
			free(addition[n++]);
		}
		free(addition);
		free(old[i]);
		++i;
	}
	while (old[i])
	{
		res[j++] = modif_strdup(old[i], input);
		printf("res[j = %d] = %s; old[i = %d] = %s\n", j - 1, res[j - 1], i, old[i]);
		free(old[i]);
		old[i] = NULL;
		++i;
	}
	res[j] = NULL;
	free(old);
	return(res);
}


char **realloc_words(char **old, t_input *input, U_INT size_good_temp, t_templ *templ)
{
	char 		**res;
	U_INT 		i; // индексация old **
	U_INT		j; // индексация res
	U_INT 		k; //длина old**
	U_INT 		n; //длина old**
	char		**addition;

	j = 0;
	i = 0;
	k = 0;
	n = 0;

	while (old[k])
		++k; // посчитали текущий размер массива слов

	
	res = malloc(sizeof(char *) * (k + size_good_temp + 1)); // нужна ли 1???
	while (old[i] && ft_strchr(old[i], '*') == NULL)
	{
		res[j++] = modif_strdup(old[i], input);
		printf("res[j = %d] = %s; old[i = %d] = %s\n", j - 1, res[j - 1], i, old[i]);
		free(old[i]);
		old[i] = NULL;
		++i;
	}
	if (old[i] && ft_strchr(old[i], '*') != NULL)
	{
		addition = big_circle(templ, input, size_good_temp);
		printf("579^ k = %u, size_good_temp = %u\n", k, size_good_temp);
		printf("579^ addition[0] = %s\n", addition[0]);
		while (addition[n])
		{
			res[j++] = modif_strdup(addition[n], input);
			printf("res[j = %d] = %s; addition[n] = %d] = %s\n", j - 1, res[j - 1], n, addition[n]);
			free(addition[n++]);
		}
		free(addition);
		free(old[i]);
		++i;
	}
	while (old[i])
	{
		res[j++] = modif_strdup(old[i], input);
		printf("res[j = %d] = %s; old[i = %d] = %s\n", j - 1, res[j - 1], i, old[i]);
		free(old[i]);
		old[i] = NULL;
		++i;
	}
	res[j] = NULL;
	free(old);
	return(res);	
}


// char **realloc_words(char **old, t_input *input, U_INT size_good_temp, t_templ *templ)
// {
// 	U_INT k;
// 	char 		**res;
// 	U_INT 		i; // индексация old **
// 	char		**addition;
// 	// U_INT len;
// 	U_INT		j; // индексация res
// 	U_INT		n;
// 	U_INT		m;

// 	// copy = input->star;
// 	i = 0;
// 	j = 0;
// 	n = 0;
// 	// len = 0;
// 	m = 0;
// 	k = 0;

// 	while (old[k])
// 		++k; // посчитали текущий размер массива слов

	
// 	while (k + size_good_temp - 1)
// 	{
// 		k = 0;
// 		while (old[k])
// 		{
// 			printf("old[k = %u] = %s\n\n\n", k, old[k]);
// 			++k; // посчитали текущий размер массива слов
// 		}
// 			// ++k; // посчитали текущий размер массива слов
// 		// printf("\n\n\nold[0] = %u] = %s\n\n\n", i, old[i]);
// 		i = 0;
// 		while (ft_strchr(old[i], '*') == NULL)
// 			++i;
// 		if (old[i] && ft_strchr(old[i], '*') != NULL)
// 		{
// 			addition = big_circle(templ, input, size_good_temp);
// 			printf("579^ k = %u, size_good_temp = %u\n", k, size_good_temp);
// 			printf("579^ addition[0] = %s\n", addition[0]);
// 			res = malloc(sizeof(char *) * (k + size_good_temp));
// 			n = 0;
// 			while (n != i)
// 			{
// 				res[j++] = modif_strdup(old[n], input);
// 				printf("res[j = %d] = %s; old[n = %d] = %s\n", j - 1, res[j - 1], n, old[n]);
// 				free(old[n]);
// 				++n;
// 			}
// 			// printf("n = %d, i = %u, addition[m = %d] = %s\n", n, i, m, addition[m]);
// 			if (n == i && addition[m])
// 			{

// 				res[j++] = addition[m++];
// 				printf("res[j = %u] = %s, addition[m = %u] = %s\n", j - 1, res[j - 1], m - 1, addition[m - 1]);
// 				free(old[i]);
// 				++i;
// 			}
// 			res[j] = NULL;
// 			free(old);
// 			old = res;
// 			// while (old[i])
// 			// {
// 			// 	res[j++] = modif_strdup(old[i], input);
// 			// 	printf("res[j = %d] = %s\n", j - 1, res[j - 1]);
// 			// 	free(old[i]);
// 			// 	++i;
// 			// }
// 		}
// 	}
// 	// res[j] = NULL;
// 	// free(old);
// 	return(res);
// }

// void	find_star(t_comm *tmp, t_input *input, U_INT k)
// {
// 	U_INT	i;
// 	t_templ	*templ;

// 	i = 0;
// 	// printf("!ищем звезду: tmp->words[0] = %s, k = %u\n", tmp->words[0], k);
// 	while (tmp && tmp->words && tmp->words[i])
// 	{	
// 		printf("!ищем звезду: tmp->words[i = %u] = %s, k = %u\n", i, tmp->words[i], k);
// 		// if (ft_strchr(tmp->words[i], '*') == NULL)
// 		// 	{
// 		// 		// printf("нет звездыу: tmp->words[i = %u] = %s\n", i, tmp->words[i]);
// 		// 		++i;
// 		// 		continue ;
// 		// 	}
// 		// else if (ft_strlen(tmp->words[i]) > 1)
// 		if (ft_strchr(tmp->words[i], '*') != NULL && ft_strlen(tmp->words[i]) > 1)
// 		{
// 			printf("1\n");
// 			// printf("есть звездыу: tmp->words[i = %u] = %s\n", i, tmp->words[i]);
// 			templ = create_template_list(tmp->words[i], input);
// 			tmp->words = realloc_words(tmp->words, input, k, templ);
// 			free_temple(templ);
// 			printf("2\n");
// 			// i = 0;
// 		}
// 		else if (ft_strchr(tmp->words[i], '*') != NULL && ft_strlen(tmp->words[i]) == 1)
// 			{
// 				printf("3\n");
// 				printf("заглушка для случая, когда пришла только *\n");
// 			// tmp->words = push_all_files();
// 			printf("4\n");
// 			}
// 		printf("647 - ищем звезду: tmp->words[i = %u] = %s, k = %u\n", i, tmp->words[i], k);
// 		++i;
// 		printf("5, i = %u\n", i);
// 	}
// }


void		put_one_in_equal(t_input *input)
{
	t_env	*copy;

	copy = input->star;
	while (copy)
	{
		copy->equal = 1;
		copy = copy->next;
	}
}


void		put_nol_in_equal(t_input *input)
{
	t_env	*copy;

	copy = input->star;
	while (copy)
	{
		copy->equal = 0;
		copy = copy->next;
	}
}


void	find_star(t_comm *tmp, t_input *input, U_INT k)
{
	U_INT	i;
	t_templ	*templ;
	U_INT	len;

	i = 0;
	len = 0;
	// printf("!ищем звезду: tmp->words[0] = %s, k = %u\n", tmp->words[0], k);
	while (tmp && tmp->words && tmp->words[i])
	{	
		printf("!ищем звезду: i = %d, k = %u\n", i, k);
		printf("!ищем звезду: tmp->words[i = %u] = %s\n", i, tmp->words[i]);
		// if (ft_strchr(tmp->words[i], '*') == NULL)
		// 	{
		// 		// printf("нет звездыу: tmp->words[i = %u] = %s\n", i, tmp->words[i]);
		// 		++i;
		// 		continue ;
		// 	}
		// else if (ft_strlen(tmp->words[i]) > 1)
		if (ft_strchr(tmp->words[i], '*') != NULL && only_star(tmp->words[i]) == success)
		{
			printf("1\n");
			// printf("есть звездыу: tmp->words[i = %u] = %s\n", i, tmp->words[i]);
			templ = create_template_list(tmp->words[i], input);
			len = check_tmp_in_dir(templ, input, 0, 0);
			printf("есть звездa, но шаблон не подошел[len = %u\n", len);
			if (len)
				{
					tmp->words = realloc_words(tmp->words, input, len, templ);
					i = 0;
				}
			else
				++i;
			put_nol_in_equal(input);
			// if (templ)
			// 	free_temple(templ);
			// put_nol_in_equal(input);
			
			printf("2\n");
			// i = 0;
		}
		else if (ft_strchr(tmp->words[i], '*') != NULL && only_star(tmp->words[i]) == fail)
			{
				put_one_in_equal(input);
				printf("3\n");
				tmp->words = easy_realloc(tmp->words, input, ft_lstsize(input->star), templ);
				i = 0;
				put_nol_in_equal(input);

				// printf("заглушка для случая, когда пришла только *\n");
			// tmp->words = push_all_files();
			printf("4\n");
			// ++i;
			}
		else if (ft_strchr(tmp->words[i], '*') == NULL)
			++i;
		// printf("647 - ищем звезду: tmp->words[i = %u] = %s, k = %u\n", i, tmp->words[i], k);
		
		printf("5, i = %u\n", i);
	}
	// if (templ)
	// 	free_temple(templ);
}
