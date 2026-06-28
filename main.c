#include "ft_ls.h"

// single entrypoint for simple recursion
void ft_ls(t_flags *flags, char **args)
{
	t_entries *entries = get_entries(args[0]);
	list_entries(entries, flags);
	// for (int i = 0; args[i]; ++i)
	// {
		
	// }	
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
		ft_ls(flags, args);
	}
	else
	{
		ft_ls(flags, &argv[1]);
	}
	free(flags);
	return (0);
	
}
