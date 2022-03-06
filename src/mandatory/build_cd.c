/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 22:03:58 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/06 21:15:47 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline char *get_path(t_input *input, t_comm *command, t_env *copy)
{
	char *path;
	char *tmp;

	path = NULL;
	tmp = NULL;
	if (command->words[1][0] != '~')
	{
		tmp = getcwd(NULL, 0);
		if (!tmp)
			print_error(input, errno, "getcwd", NULL);
		path = ft_strjoin_for_3(tmp, "/", command->words[1], input);
		free(tmp);
		return (path);
	}
	while (copy && ft_strcmp(copy->key, "HOME"))
		copy = copy->next;
	if (copy)
	{
		tmp = modif_substr(command->words[1], 1, \
			ft_strlen(command->words[1]) - 1, input);
		path = ft_strjoin(copy->value, tmp, input);	
		free(tmp);
	}
return (0);
}

void	change_old(t_input *input, char *old)
{
	t_env *copy;
	char *tmp;

	copy = input->envp;
	while (ft_strcmp(copy->key, "OLDPWD") != success)
		copy = copy->next;
	if (!copy)
		{
			tmp = ft_strjoin_for_3("OLPWD", "=", old, input);
			create_new_list(tmp, input);
		}
	else
	{
		free(copy->value);
		copy->value = old;
	}		
}

void	change_current(t_input *input, char *path)
{
	t_env *copy;
	char *tmp;

	copy = input->envp;
	while (ft_strcmp(copy->key, "PWD") != success)
		copy = copy->next;
	if (!copy)
		{
			tmp = ft_strjoin_for_3("PWD", "=", path, input);
			create_new_list(tmp, input);
		}
	else
	{
		free(copy->value);
		copy->value = path;
	}	

}

U_INT	launch_cd(t_input *input, t_comm *command)
{
	char 	*path;
	t_env	*copy;
	char	*old;

	path = NULL;
	old = getcwd(NULL, 0);
	if (!old)
		print_error(input, errno, "getcwd", NULL); 
	copy = input->envp;
	if (!input || !command->words[1])
	{
		while (copy && ft_strcmp(copy->key, "HOME"))
			copy = copy->next;
		if (copy)
			path = modif_strdup(copy->value, input);	
	}
	else
		path = get_path(input, command, copy);
	if (!path)
	{
		write(2, "cd: HOME not set\n", 17); // print_error(input, 1, "cd", "HOME not set");
		return (1);
	}
	if (chdir(path) != success)
		print_error(input, 1, command->words[0], "No such file or directory"); // если 2 аргуента - отд.функция
	change_old(input, old);
	change_current(input, path);
	input->num_error = 1;
	free(path);
	return (input->num_error);
}

