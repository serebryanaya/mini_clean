/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_t_comm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 20:27:46 by pveeta            #+#    #+#             */
/*   Updated: 2022/02/17 20:27:46 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	go_through_redirect(char *str, U_INT *i, char c, t_input *input)
{
	(*i)++;
	if (str[*i] == c)
		(*i)++;
	while (str[*i] == ' ')
		(*i)++;
	while (str[*i] && str[*i] != '>' && str[*i] != '<' \
		&& str[*i] != '|' && str[*i] != ' ')
	{
		if (str[*i] == '\'')
			go_to_end_quote(str, i, '\'', input);
		if (str[*i] == '\"')
			go_to_end_quote(str, i, '\"', input);
		else
			(*i)++;
	}
}

// static void	check_star_in_word(char *str, U_INT *counter, U_INT *i, char *d_name)
// {
// 	if (!(str[*i + 1]) || (str[*i + 1] && str[*i + 1] == '*' && !(str[*i + 2])))
// 	{
// 		(*counter)++;
// 		return ;
// 	}
		
// }


static void	go_through_star(char *str, U_INT *i, t_input *input, U_INT *counter)
{
	DIR					*dir;
    struct				dirent *entry;
	
	// printf("нашли одинокую звезду\n");
	if (*counter == 0)
		(*counter)++;
	else
	{
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
				(*counter)++;
				// check_star_in_word(str, counter, i, entry->d_name);
		}
		else
			break ;
    }
    closedir(dir);
	}
	// while (str[*i] && str[*i] != '|')
	// 	(*i)++;
	while (str[*i] && str[*i] != '\0' && str[*i] != '|' && str[*i] != ' ')
		(*i)++;
	// if (str[*i] && str[*i] == '*')
	// 	(*i)++;
}

U_INT	ft_lstsize(t_env *lst)
{
	U_INT	len;

	len = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		len++;
	}
	return (len);
}

U_INT	get_number_of_words(char *str, U_INT *i, t_input *input)
{
	U_INT	counter;

	counter = 0;
	find_star_in_comm(input);
	// printf("check111\n");
	while (str[*i] != '\0' && str[*i] != '|')
	{
		if (str[*i] == ' ')
			(*i)++;
		else if (str[*i] == '<')
			go_through_redirect(str, i, '<', input);
		else if (str[*i] == '>')
			go_through_redirect(str, i, '>', input);
		// else if ((str[*i] == '*') && ((!str[*i + 1]) || ((str[*i + 1]) && (str[*i + 1] == '*' || str[*i + 1] == ' '))))
		else if (str[*i] == '*')
		{
			// printf("go to go_through_star\n");
			// find_star_in_comm(input);
			if (counter == 0)
				counter++;
			else
				counter += ft_lstsize(input->star);
			while (str[*i] && str[*i] != '\0' && str[*i] != '|' && str[*i] != ' ')
				(*i)++;
		} 
		else
		{
			// printf("go to go_through_word\n");
			counter++;
			go_through_word(str, i, input);
		}
		// printf("count word = %d, current position = %c\n", counter, str[*i]);
	}
	printf("check: counter = %d\n", counter);
	return (counter);
}

static t_comm	*create_list_comm(char *str, t_input *input, U_INT *i)
{
	U_INT	counter;
	t_comm	*new;

	// printf("создаю новую ноду с командой\n");
	new = malloc(sizeof(t_comm));
	if (new == NULL)
		return (NULL);
	new->direct_in = NULL;
	new->direct_out = NULL;
	new->next = NULL;
	new->build_number = 0;
	counter = get_number_of_words(str, i, input); 
	// printf("количество слов до пайпа = %d\n", counter);
	new->words = malloc(sizeof(char *) * (counter + 1));
	if (!new->words)
		return (NULL);
	new->words[0] = NULL;
	return (new);
}

static void add_comm_back(t_comm *new, t_comm **command)
{
	t_comm	*copy;

	if (*command == NULL)
		*command = new;
	else
	{
		copy = *command;
		while (copy->next != NULL)
			copy = copy->next;
		copy->next = new;
	}
}

t_status create_empty_t_comm(char *str_command, t_input *input)
{
	U_INT	i;
	t_comm	*new;

	i = 0;
	while (str_command[i])
	{
		while (str_command[i] && str_command[i] != '|')
		{
			// printf("созадем ноду под команду до пайпа\n");
			new = create_list_comm(str_command, input, &i);
			if (!new)
				return (fail);
			add_comm_back(new, &input->command);
		}
		if (str_command[i])
			i++;
	}
	if (!str_command[i] && (str_command[i - 1] == '|' || \
		str_command[i - 1] == '&' || str_command[i - 1] == '>' || \
		str_command[i - 1] == '<'))
		return (end);
	return (success);	
}

/*

void	go_through_word(char *str, U_INT *i, t_input *input, U_INT *counter)
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
			{
				(*i)++;
				(*counter)++;
			}
			//!!!!сюда добавить обработку звезды в середине или конце файла и пересчет каунтера
	}
}

*/