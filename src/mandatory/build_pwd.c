/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 14:29:10 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/05 20:32:45 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

U_INT	launch_pwd(t_input *input)
{
	char	*pwd;

	pwd = getcwd(NULL, 0); //получаем абсолютный путь к рабочему каталогу
	if (!pwd)
		print_error(input, errno, "getcwd", NULL); 
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}