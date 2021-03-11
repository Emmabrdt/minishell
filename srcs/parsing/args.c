#include "../../includes/minishell.h"

int	init_malloc_args(char *whole_cmd, t_copy *copy, size_t i)
{
	copy->args[i] = NULL;
	g_error = 0;
	copy->args[i] = malloc(sizeof(char) * (strlen(whole_cmd) + 1));
	if (!(copy->args[i]))
		return (-1);
	copy->args[i][0] = 0;
	while (whole_cmd[copy->i] && whole_cmd[copy->i] == ' ')
		copy->i++;
	copy->i--;
	return (0);
}

char	*args(char *whole_cmd, t_copy *copy, size_t i, t_redir *redir)
{
	int		j;

	copy->j = -1;
	if (init_malloc_args(whole_cmd, copy, i) == -1)
		return (NULL);
	while (whole_cmd[copy->i] && (whole_cmd[++copy->i] != ' ' || (whole_cmd[copy->i] == ' ' && whole_cmd[copy->i - 1] == '\\')))
	{
		j = 0;
		if ((whole_cmd[copy->i] == '1' || whole_cmd[copy->i] == '2') && whole_cmd[copy->i + 1] == '>' && whole_cmd[copy->i - 1] == ' ')
			copy->i++;
		while (whole_cmd[copy->i] == '\'' || whole_cmd[copy->i] == '"')
		{
			while (whole_cmd[copy->i] == '"')
				if ((j = double_quote_arg(whole_cmd, copy, i)) == -1)
					return (NULL);
			while (whole_cmd[copy->i] == '\'')
				if ((simple_quote_arg(whole_cmd, copy, i)) == -1)
					return (NULL);
		}
		if (whole_cmd[copy->i] == '\\')
			copy->i++;
		if (whole_cmd[copy->i] && whole_cmd[copy->i] == '$' && whole_cmd[copy->i - 1] != '\\')
		{
			if (whole_cmd[copy->i + 1] == '\\')
				copy->args[i][++copy->j] = whole_cmd[copy->i];
			else
				j = environnement(whole_cmd, copy, 1, i);
		}
		if ((whole_cmd[copy->i] == '>' || whole_cmd[copy->i] == '<') && whole_cmd[copy->i - 1] != '\\')
		{
			j = redirection(whole_cmd, copy, redir);
			if (j == -1)
				return (NULL);
			if (j == -3)
				return (copy->args[i]);
		}
		if ((whole_cmd[copy->i] == ' ' && whole_cmd[copy->i - 1] != '\\') && (copy->args[i][0] || (!copy->args[i][0] 
			&& (whole_cmd[copy->i - 1] == '"' || whole_cmd[copy->i - 1] == '\'') 
			&& (whole_cmd[copy->i - 2] == '"' || whole_cmd[copy->i - 2] == '\'' || j == 1))))
				break;
		if (whole_cmd[copy->i] && (whole_cmd[copy->i] != ' ' || (whole_cmd[copy->i] == ' ' && whole_cmd[copy->i - 1] == '\\')) && j != 1 && j!= 4 && ((whole_cmd[copy->i] == '$' && whole_cmd[copy->i - 1] == '\\') || (whole_cmd[copy->i] != '$')))
			copy->args[i][++copy->j] = whole_cmd[copy->i];	
	}
	copy->args[i][copy->j + 1] = 0;
	return (copy->args[i]);
}