/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 23:26:00 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/10 00:24:40 by pveeta           ###   ########.fr       */
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

void add_list_in_templ(t_templ	*templ, char *str)
{
	U_INT	i;
	U_INT	m;
	t_templ	*copy;

	copy = templ;
	while (str[i])
	{
		m = i;
		if (str[0] != '*')
		{
			copy->status = 1;
			while (str[i] != '*')
				i++;
			copy->value = modif_substr(str, m, i - m, input);
			copy = copy->next;
			i++;
		}
		else
		{
			while (str[i] && str[i] != '*')
				i++;
			copy->value = modif_substr(str, m, i - m, input);
			if (!str[i])
				copy->status = 3;
				copy->next = NULL;
			else
			{
				copy->status = 2;
				copy = copy->next;
			}
		}
		printf("copy->value = %d, copy->status = %u\n", copy->value, copy->status);
	}
}

t_templ	*create_template_list(char *str)
{
	U_INT	num;
	U_INT	i;
	t_templ	*templ;

	num = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] != '*')
			num++;
		while (str[i] && str[i] != '*')
			i++;
		while (str[i] && str[i] == '*')
			i++;
	}
	templ = malloc(sizeof(templ) * num);
	add_list_in_templ(templ, str);
	return(temple);
}


t_status	check_template(t_input *input, char *str, t_templ *temple)
{	
	t_status	flag; // 0 - start, 1 - not, 2 - finish
	// U_INT		start;
	t_templ	*copy_templ;
	U_INT	i;

	// start = 0;
	i = 0;
	flag = 0;
	copy_templ = temple;
	while (copy_templ && flag == 0)
	{
		if (copy_templ->status == 1) // start word
    	{
			if (copy_templ->value[0] == str[0] && 
			ft_strncmp(copy_templ->value, str, ft_strlen(copy_templ->value)) == 0)
			{
				copy_templ = copy_templ->next;
				// start = ft_strlen(copy_templ->value) - 1; // -1 is ok????????
			}
			else
				flag = 1;
		}	
    	// else if (copy_templ->status == 2)
		else
    	{
         while (str[i] && copy_templ)
		{
			if (ft_strncmp(copy_templ->value, str + i, ft_strlen(copy_templ->value)) == 0)
				copy_templ = copy_templ->next;
			else
				i++;
			if (copy_templ)
				flag = 1;
			else if () // дописать тут случай, когда статус 2 и2, какие должны быть флаги
		}
		if ()


		 if (ft_strncmp(copy_templ->value, str, ft_strlen(copy_templ->value)) == 0)
          {
            copy_templ = copy_templ->next;
             Start = ft_strlen(vrem->value);
             If (vrem == NULL)
                 Flag = 3;
           }
       Else
            Flag == 2;
     }

    Else If (vrem->status ==3)
    {
         if (ft_strncmp(vrem->value, copy->value + start, ft_strlen(vrem->value)) == 0)
          {
            Vrem=vrem->next;
             Start = ft_strlen(vrem->value);
             If (vrem == NULL)
                 Flag = 3;
           }
       Else
            Flag == 2;
     }
If (flag == 3)
   {
       Вырезаем в words;
       k++;
        If (k == 1)
            Break ;
    }
Copy =copy->next;
}


		copy_templ = copy_templ->next;
	}

	if (flag == 2)
		return (success);
	else
		return (fail);
}

char **big_circle(t_env *copy, t_templ *temple, t_input *input)
{
	char 	**addition;
	// t_env	*copy_star;
	// t_templ	*copy_terml;
	U_INT	i; //ходим по массиву addition
	
	addition = malloc(sizeof(char *) * ft_lstsize(input->star));
	while (i < ft_lstsize(input->star))
	{
		while (copy)
		{
			if (check_template(input, copy->value, templ) == success);
				addition[i++] = modif_strdup(copy->value, input);
			copy = copy->next;
		}
		addition[i++] = NULL;
	}
	return (addition);
}


char **realloc_words(char **old, t_input *input, t_input *k, t_templ *templ)
{
	char 		**res;
	U_INT 		start;
	t_env		*copy;
	t_status	flag;
	char		**addition;

	copy = input->star;
	start = 0;

	while (old[i])
	{
		if (ft_strchr(old[i], '*') == NULL)
			i++;
		else
		{
			addition = big_circle(copy, temple, input);
			i = 0;
		}
	}

// здесь дописать именно переаллоцирование памяти

}


void	find_star(t_comm *tmp, t_input *input, t_input *k)
{
	U_INT	i;
	t_templ	*templ;

	i = 0;
	while (tmp->words[i])
	{	
		if (ft_strchr(tmp->words[i], '*') == NULL)
			i++;
		else if (ft_strlen(tmp->words[i] > 1))
		{
			templ = create_template_list(tmp->words[i]);
			tmp->words = realloc_words(tmp->words, input, k, templ);
			free_templ(temple);
			i = 0;
		}
		else if (ft_strlen(tmp->words[i] == 1))
			push_all_files();
	})
}