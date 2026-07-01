#include "ft_ls.h"

// single entrypoint for simple recursion
void ft_ls(t_flags *flags, char **args, int argc)
{
	// array of entries linked list 
	t_entries **entries_arr = malloc(sizeof(t_entries *) * argc);
	if (!entries_arr)
	{
		ft_free_array(args);
		return ;
	}
	// print_array(args);
	for (int i = 0; i < argc; ++i)
	{
		entries_arr[i] = get_entries(args[i]);
		entries_arr[i] = sort(entries_arr[i], flags);
		if (argc > 1 || flags->R_recursive)
			ft_printf("%s:\n", args[i]);
		list_entries(entries_arr[i], flags);
	}
	if (flags->R_recursive)
	{
		for (int i = 0; i < argc; ++i)
		{
			int dir_count = count_dirs(entries_arr[i]);
			char **new_args = get_dirs(entries_arr[i], dir_count, args[i]);
			// print_array(new_args);
			if (new_args)
				ft_ls(flags, new_args, dir_count);
		}
	}
	for (int i = 0; i < argc; ++i)
		free_entries(entries_arr[i]);
	free(entries_arr);
	ft_free_array(args);
}

int main(int argc, char* argv[])
{
	t_flags	flags;
	char 	**args;
	args =  parse_ls(&flags, argc, argv);
	if (!args)
		return (1);
	int count = 0;
	
	while (args[count])
		count ++;
	ft_ls(&flags, args, count);
	return (0);
	
}
