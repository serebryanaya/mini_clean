/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_other3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pveeta <pveeta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 21:09:06 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/08 21:10:17 by pveeta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strchr(char *str, char c)
{
	U_INT	i;

	i = 0;
	while (*(str + i) != 0)
	{
		if (*(str + i) != c)
			i++;
		else
			return (str + i);
	}
	return (NULL);
}

void	iter_str(char *str, U_INT *i)
{
	while (str[*i] && str[*i] != '\0' && str[*i] != '|' && str[*i] != ' ')
		(*i)++;
}
