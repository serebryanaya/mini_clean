/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   star.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 23:26:00 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/08 20:16:29 by pveeta           ###   ########.fr       */
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
	// printf("ned = %s, hay = %s\n", needle, haystack);
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

void	star_in_word(t_comm **tmp, U_INT *k, char *vrem, t_input *input)
{
	U_INT		i;
	U_INT		m;
	char		*template;
	t_env		*copy;
	t_status	flag;
	t_comm		*orig;

	orig = *tmp;
	flag = 0;
	i = 0;
	while (vrem[i] && vrem[i] == '*')
		i++;
	if (vrem[i]) // кроме звезды в начале есть что-то еще
	{
		// printf("во vrem НЕ только звезда, k = %d\n", *k);
		m = i;
		while (vrem[i] && vrem[i] != '*')
			i++;
		template = modif_substr(vrem, m, i - m, input);
		// printf("template = %s\n", template);
		copy = input->star;
		while (copy)
		{
			// printf("ft_strstr(copy->value, template) = %s\n", ft_strstr(copy->value, template));
			if (ft_strstr(copy->value, template) != NULL)
			{
				orig->words[*k] = modif_strdup(copy->value, input);
				// printf("~!!!!orig->words[*k = %d] = %s\n", *k, orig->words[*k]);
				(*k)++;
				flag = 1;
			}
			copy = copy->next;
		}
		if (flag == 0)
		{
			orig->words[*k] = modif_strdup(vrem, input);
			(*k)++;
		}	
	}
	else
	{
		// printf("во vrem только звезда, k = %d\n", *k);
		// tmp->words[*k] = change_star(tmp, input, k); // нашли в слове только * или ** 
		change_star(tmp, input, k);
		// {
			// if (*k == 0)
			// 	orig->words[*k + 1] = NULL;
		// }
	}
	orig->words[*k] = NULL;
}