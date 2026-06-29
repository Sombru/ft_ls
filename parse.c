#include "ft_ls.h"

static int	print_invalid_option(char option)
{
	ft_putstr_fd("ft_ls: illegal option -- ", 2);
	ft_putchar_fd(option, 2);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("usage: ft_ls [-adflRrtuUg] [file ...]\n", 2);
	return (0);
}

static void	set_flag(t_flags *flags, char option)
{
	switch (option)
	{
		case 'a':
			flags->a_all = true;
			break;
		case 'd':
			flags->d_directories = true;
			break;
		case 'f':
			flags->f_list = true;
			flags->a_all = true;
			flags->U_sort = true;
			break;
		case 'l':
			flags->l_list = true;
			break;
		case 'R':
			flags->R_recursive = true;
			break;
		case 'r':
			flags->r_reverse = true;
			break;
		case 't':
			flags->t_time = true;
			break;
		case 'u':
			flags->u_sort = true;
			break;
		case 'U':
			flags->U_sort = true;
			break;
		case 'g':
			flags->g_list = true;
			break;
		default:
			print_invalid_option(option);
			break;
	}
}

char	**parse_ls(t_flags *flags, int argc, char *argv[])
{
	char **args = ft_calloc(argc, sizeof(char *) * argc);
	ft_memset(flags, 0, sizeof(flags));
	int k = 0;
	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
			set_flag(flags, argv[i][1]);
		else
			args[k++] = argv[i]; 
	}
	if (args[0] == 0)
		args[0] = ".";
	return (args);
}
