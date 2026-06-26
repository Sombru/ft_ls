#include "ft_ls.h"

int main(int argc, char* argv[])
{
	t_flags	*flags;

	flags = parse_ls_flags(argc, argv);
	if (!flags)
		return (1);
	free(flags);
	list_entries(get_entries("."));
	return (0);
	
}
