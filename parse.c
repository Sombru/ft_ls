#include "ft_ls.h"

static int	print_invalid_option(char option)
{
	ft_putstr_fd("ft_ls: illegal option -- ", 2);
	ft_putchar_fd(option, 2);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("usage: ft_ls [-adflRrtuUg] [file ...]\n", 2);
	return (0);
}

static int	set_flag(t_flags *flags, char option)
{
	if (option == 'a')
		flags->a_all = true;
	else if (option == 'd')
		flags->d_directories = true;
	else if (option == 'f')
	{
		flags->f_list = true;
		flags->a_all = true;
		flags->U_sort = true;
	}
	else if (option == 'l')
		flags->l_list = true;
	else if (option == 'R')
		flags->R_recursive = true;
	else if (option == 'r')
		flags->r_reverse = true;
	else if (option == 't')
		flags->t_time = true;
	else if (option == 'u')
		flags->u_sort = true;
	else if (option == 'U')
		flags->U_sort = true;
	else if (option == 'g')
	{
		flags->g_list = true;
		flags->l_list = true;
	}
	else
		return (print_invalid_option(option));
	return (1);
}

t_flags	*parse_ls_flags(int argc, char *argv[])
{
	t_flags	*flags;
	int		i;
	int		j;

	flags = ft_calloc(1, sizeof(t_flags));
	if (!flags)
		return (NULL);
	i = 1;
	while (i < argc && argv[i][0] == '-' && argv[i][1] != '\0')
	{
		if (ft_strcmp(argv[i], "--") == 0)
			return (flags);
		j = 1;
		while (argv[i][j] != '\0')
		{
			if (!set_flag(flags, argv[i][j]))
			{
				free(flags);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	return (flags);
}
