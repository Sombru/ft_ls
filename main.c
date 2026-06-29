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
	t_flags	flags;
	char 	**args;
	args =  parse_ls(&flags, argc, argv);
	print_array(args);
	for (int i = 0; args[i]; ++i)
		argc = i;
	ft_ls(&flags, args, argc);
	return (0);
	
}
