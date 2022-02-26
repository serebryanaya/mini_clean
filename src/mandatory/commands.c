#include "minishell.h"

static void	free_path(char **path)
{
	int	i;

	i = -1;
	while (path && path[++i])
		free(path[i]);
	free(path);
}

static char	*cmd_path(char *cmd, char *arg_cmd, int num, t_input *input) //char	*cmd_path(char *cmd, char *arg_cmd, int num, t_arg *arg)
{
	int		i;
	char	**path;
	char	*tmp;

	path = ft_split(input->arg_env[num] + 5, ':');
	if (!path)
		print_error(input, 12, "malloc", NULL);
	tmp = NULL;
	i = -1;
	while (path[++i] && !tmp)
	{
		tmp = ft_strjoin(path[i], cmd);
		if (!tmp)
			print_error(input, 12, "malloc", NULL);
		if (access(tmp, F_OK))
		{
			free(tmp);
			tmp = NULL;
		}
	}
	if (!tmp)
		tmp = arg_cmd;
	free_path(path);
	free(cmd);
	return (tmp);
}

static char	*ft_path(char *cmd, t_input *input) //char	*ft_path(char *cmd, t_arg *arg)
{
	char	*path;
	char	*forward_slash;
	int		i;

	if (!access(cmd, F_OK))
		return (cmd);
	i = 0;
	while (input->arg_env[i] && ft_strncmp(input->arg_env[i], "PATH=", 5))
		i++;
	if (!input->arg_env[i])
	{
		access(cmd, F_OK);
        print_error(input, errno, cmd, NULL); ///?
	}
	forward_slash = ft_strjoin("/", cmd);
	if (!forward_slash)
		print_error(input, 12, "malloc", NULL);
	path = cmd_path(forward_slash, cmd, i, input);
	return (path);
}

static void	open_dup(int i, t_cmd *cmd, t_arg *arg)
{
	int		file[2];

	file[0] = -2;
	if (cmd->in)
		file[0] = open(cmd->in->name, O_RDONLY);
	if (file[0] == -1)
		ft_exit(errno, cmd->in->name, arg);
	file[1] = -2;
	if (cmd->out && cmd->out->two)
		file[1] = open(cmd->out->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (cmd->out && !cmd->out->two)
		file[1] = open(cmd->out->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file[1] == -1)
		ft_exit(errno, cmd->out->name, arg);
	ft_dup2(i, file, cmd, arg);
}

void	child(int i, t_arg *arg)
{
	int		num;
	t_cmd	*cmd;
	char	*path;

	signal(SIGINT, sig_child);
	signal(SIGQUIT, sig_child);
	cmd = arg->cmd;
	num = 0;
	while (num++ < i)
		cmd = cmd->next;
	if (!cmd->cmd || !cmd->cmd[0])
		exit(0);
	open_dup(i, cmd, arg);
	if (cmd->built)
		ft_exit(run_built(cmd, arg), NULL, arg);
	path = ft_path(cmd->cmd[0], arg);
	if (execve(path, cmd->cmd, arg->env) == -1)
		ft_exit(errno, cmd->cmd[0], arg);
}
