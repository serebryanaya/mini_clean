/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 22:03:58 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/08 21:40:57 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cd_print_error(t_input *input, U_INT error_num, char *msg, char *descrip)
{
	input->num_error = error_num;
	
	// printf("cd_print_error\n");
	write(2, "minishell: ", 11);
	if (msg != NULL)
	{
		write(2, msg, ft_strlen(msg));
		write(2, ": ", 2);
	}
	if (descrip == NULL)
		write(2, strerror(error_num), ft_strlen(strerror(error_num)));
	else
		write(2, descrip, ft_strlen(descrip));
	write(2, "\n", 1);
	free_all(input);
	// exit(error_num);
}

static inline char	*get_path(t_input *input, t_comm *command, t_env *copy)
{
	char	*path;
	char	*tmp;

	path = NULL;
	tmp = NULL;
	if (command->words[1][0] != '~')
	{
		tmp = getcwd(NULL, 0);
		if (!tmp)
			cd_print_error(input, errno, "getcwd", NULL);
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
	// printf("%s\n", path);
	return (0);
}

static void	change_envp(t_input *input, char *path, t_status flag)
{
	t_env	*copy;
	char	*tmp;
	t_env	*new;
	char	*key;

	path = getcwd(NULL, 0);
	if (!path)
		cd_print_error(input, errno, "getcwd", NULL);
	if (flag == 0)
		key = modif_strdup("OLDPWD", input);
	else
		key = modif_strdup("PWD", input);
	copy = input->envp;
	while (copy && ft_strcmp(copy->key, key) != success)
		copy = copy->next;
	if (!copy)
	{
		tmp = ft_strjoin_for_3(key, "=", path, input);
		new = create_new_list(tmp, input);
		free(tmp);
		if (!new)
			cd_print_error(input, 12, "malloc", NULL);
		add_list_back(new, &input->envp);
	}
	else
	{
		free(copy->value);
		copy->value = path;
	}
	free(key);
}

char	*cd_oldpwd(t_input *input, t_comm *command, t_env *copy)
{
	char 	*path;
	char	*tmp;

	path = NULL;
	while (copy && ft_strcmp(copy->key, "OLDPWD"))
		copy = copy->next;
	// printf("key=%s\n", copy->key);	
	if (copy)
	{
		tmp = modif_substr(command->words[1], 1, \
			ft_strlen(command->words[1]) - 1, input);
		path = ft_strjoin(copy->value, tmp, input);
		free(tmp);
		return (path);
		// printf("%s\n", path);
	}
	return (0);
}

U_INT	launch_cd(t_input *input, t_comm *command)
{
	char 	*path;
	t_env	*copy;

	path = NULL;
	copy = input->envp;
	if (!input || !command->words[1])
	{
		while (copy && ft_strcmp(copy->key, "HOME"))
			copy = copy->next;
		if (copy)
		{
			path = modif_strdup(copy->value, input);
			// printf("cd_printf\n");
		}
	}
	// else
	// 	// path = get_path(input, command, copy);
	else
	{
		if (command->words[1][0] == '-')
		{
			path = cd_oldpwd(input, command, copy);
			if (!path)
			{
				write(2, "cd: OLDPWD not set\n", 17);
				return (1);
			}
			else
				printf("%s\n", path);
		}
		else
		{
			printf("command->words[1]=%s\n", command->words[1]);
			path = get_path(input, command, copy);
			if (!path)
			{
				write(2, "cd: HOME not set\n", 17); // print_error(input, 1, "cd", "HOME not set");
				return (1);
			}
		}
	}
	if (!path)
	{
		write(2, "cd: HOME not set\n", 17); // print_error(input, 1, "cd", "HOME not set");
		return (1);
	}
	change_envp(input, NULL, 0);
	if (chdir(path) != success)
		cd_print_error(input, 1, command->words[0], "No such file or directory"); // если 2 аргуента - отд.функция
	change_envp(input, NULL, 1);
	input->num_error = 1;
	free(path);
	return (input->num_error);
}
