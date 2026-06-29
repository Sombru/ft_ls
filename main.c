#include "ft_ls.h"

// single entrypoint for simple recursion
void ft_ls(t_flags *flags, char **args, int argc)
{
	// array of entries linked list 
	t_entries **entries = malloc(sizeof(t_entries *) * argc);

	for (int i = 0; i < argc; ++i)
	{
		entries[i] = get_entries(args[i]);
		list_entries(entries[i], flags);
	}
	if (flags->R_recursive)
	{
		for (int i = 0; i < argc; ++i)
		{
			int dir_count = count_dirs(entries[i]);
				ft_ls(flags, get_dirs(entries[i], dir_count), dir_count); 
			
		}
	}
}

int main(int argc, char* argv[])
{
	t_flags	*flags;

	flags = parse_ls_flags(argc, argv);
	if (!flags)
		return (1);
	if (argc == 1)
	{
		char *args[4] = {".", NULL};
		ft_ls(flags, args, argc);
	}
	else
	{
		ft_ls(flags, &argv[1], argc - 1);
	}
	free(flags);
	return (0);
	
}
