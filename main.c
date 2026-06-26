#include "ft_ls.h"

int main(int argc, char* argv[])
{
	t_flags	*flags;

	flags = parse_ls_flags(argc, argv);
	if (!flags)
		return (1);
	// DIR* dir = opendir(".");
	// struct dirent* entry;

	// if (!dir)
	// 	return 1;	
	// while ((entry = readdir(dir)))
	// {
	// 	ft_printf("%s  ",entry->d_name);
	// }
	// ft_printf("\n");
	free(flags);
	return (0);
	
}
