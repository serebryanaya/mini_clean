#include "minishell.h"

int	launch_pwd(t_input *input)
{
	char	*pwd;

	pwd = getcwd(NULL, 0); //получаем абсолютный путь к рабочему каталогу
	if (!pwd)
		print_error(input, errno, "getcwd", NULL); 
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
