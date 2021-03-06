/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmather <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/10 16:15:32 by dmather           #+#    #+#             */
/*   Updated: 2016/09/11 11:50:51 by dmather          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh.h"

void	get_full_paths(t_env *e)
{
	int				i;
	t_split_string	args;
	char			*envi;

	i = 0;
	envi = ft_getenv("PATH", e->environ);
	args = ft_nstrsplit(envi, ':');
	e->n_paths = args.words;
	e->paths = ft_tabdup(args.strings, args.words);
	ft_free_tab(&args.strings, args.words);
	e->full_paths = (char **)ft_memalloc(sizeof(char *) * (e->n_paths + 2));
	while (i < e->n_paths)
	{
		e->full_paths[i] = ft_nstrjoin(e->paths[i], "/", e->input[0]);
		i++;
	}
}

int		accessible(t_env *e)
{
	int	i;

	i = 0;
	if (access(e->input[0], F_OK) == 0)
		return (CONT);
	while (i < e->n_paths)
	{
		if (access(e->full_paths[i], F_OK) == 0)
		{
			e->execute = ft_strdup(e->full_paths[i]);
			return (CONT);
		}
		i++;
	}
	return (0);
}

char	*ft_getenv(char *name, char **envp)
{
	int	i;
	int	j;

	if (name)
	{
		i = 0;
		while (envp[i] != NULL)
		{
			j = 0;
			while (envp[i][j] == name[j])
			{
				if (name[j + 1] == '\0' && envp[i][j + 1] == '=')
					return (ft_strchr(envp[i], '=') + 1);
				++j;
			}
			++i;
		}
	}
	return (name);
}

int		ft_setenv(t_env *e)
{
	char	*tmp;
	char	**tmp2;

	if (e->input[1] && e->input[2] && e->n_input == 3)
	{
		tmp2 = ft_tabdup(e->environ, ++e->ie);
		ft_free_tab(&e->environ, e->ie);
		e->environ = tmp2;
		tmp = e->input[2];
		e->input[2] = ft_trim_qu(tmp);
		ft_strdel(&tmp);
		e->environ[e->ie - 1] = ft_nstrjoin(e->input[1], "=", e->input[2]);
	}
	else if (e->n_input == 2 && e->input[1])
		more_setenv(e);
	else
		ft_putstr(C_RED
					"Please be specific or use the correct syntax.\n"C_RESET);
	return (CONT);
}

int		ft_unsetenv(t_env *e)
{
	int	i;

	i = 0;
	if (e->input[1])
	{
		e->name = ft_strjoin(e->input[1], "=");
		while (i < e->ie && UNSETENV_FIND)
			i++;
		if (!e->environ[i])
			return (more_unsetenv(e));
		ft_strdel(&e->environ[i]);
		while (i < (e->ie - 1))
		{
			e->environ[i] = e->environ[i + 1];
			i++;
		}
		e->environ[i] = NULL;
		e->ie -= 1;
	}
	else
		ft_putstr(C_RED"Please tell me what you want to unset.\n"C_RESET);
	ft_strdel(&e->name);
	return (CONT);
}
