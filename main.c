#include "ft_ls.h"

// single entrypoint for simple recursion
// void ft_ls(t_flags *flags, char* args[])
// {
// }

int main(int argc, char* argv[])
{
	t_flags	*flags;

	flags = parse_ls_flags(argc, argv);
	if (!flags)
		return (1);
	free(flags);
	list_entries(get_entries("."), flags);
	return (0);
	
}
