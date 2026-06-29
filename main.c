#include "ft_ls.h"

// single entrypoint for simple recursion
void ft_ls(t_flags *flags, char **args, int argc)
{
	// array of entries linked list 
	t_entries **entries_arr = malloc(sizeof(t_entries *) * argc);
	// print_array(args);
	for (int i = 0; i < argc; ++i)
	{
		entries_arr[i] = get_entries(args[i]);
		ft_printf("%s:\n", args[i]);
		list_entries(entries_arr[i], flags);
		ft_printf("\n");
	}
	if (flags->R_recursive)
	{
		for (int i = 0; i < argc; ++i)
		{
			int dir_count = count_dirs(entries_arr[i]);
			char **new_args = get_dirs(entries_arr[i], dir_count, args[i]);
			// print_array(new_args);
			ft_ls(flags, new_args, dir_count); 
			
		}
	}
}

int main(int argc, char* argv[])
{
	t_flags	flags;
	char 	**args;
	args =  parse_ls(&flags, argc, argv);
	int count = 0;
	
	while (args[count])
		count ++;
	ft_ls(&flags, args, count);
	return (0);
	
}
