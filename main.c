#include "ft_ls.h"

// static void	ft_ls_impl(t_flags *flags, char **args, int argc, int *printed_list);

// static void	print_list_separator(int *printed_list)
// {
// 	if (*printed_list)
// 		ft_putchar_fd('\n', 1);
// 	*printed_list = 1;
// }

// static int	count_entries(t_entries *entries)
// {
// 	int	count;

// 	count = 0;
// 	while (entries)
// 	{
// 		count++;
// 		entries = entries->next;
// 	}
// 	return (count);
// }

// static t_entries	*get_operand_entries(t_flags *flags, char **args, int argc)
// {
// 	t_entries	*entries;
// 	t_entries	*entry;
// 	t_flags		arg_flags;

// 	arg_flags = *flags;
// 	arg_flags.d_directories = true;
// 	entries = NULL;
// 	for (int i = 0; i < argc; ++i)
// 	{
// 		entry = get_entries(args[i], &arg_flags);
// 		if (entry && !append_entry(&entries, entry))
// 			free_entries(entry);
// 	}
// 	return (sort(entries, flags));
// }

// static void	split_operands(t_entries *operands, t_entries **files,
// 		t_entries **dirs)
// {
// 	t_entries	*next;

// 	*files = NULL;
// 	*dirs = NULL;
// 	while (operands)
// 	{
// 		next = operands->next;
// 		operands->next = NULL;
// 		if (S_ISDIR(operands->st.st_mode))
// 			append_entry(dirs, operands);
// 		else
// 			append_entry(files, operands);
// 		operands = next;
// 	}
// }

// static void	list_recursive_dirs(t_entries *entries, t_flags *flags,
// 		char *parent_path, int *printed_list)
// {
// 	char	**new_args;
// 	int		dir_count;

// 	if (!flags->R_recursive)
// 		return ;
// 	dir_count = count_dirs(entries);
// 	new_args = get_dirs(entries, dir_count, parent_path);
// 	if (new_args)
// 		ft_ls_impl(flags, new_args, dir_count, printed_list);
// }

// static void	list_directory_operand(t_entries *dir, t_flags *flags,
// 		int print_header, int *printed_list)
// {
// 	t_entries	*entries;

// 	print_list_separator(printed_list);
// 	if (print_header)
// 		ft_printf("%s:\n", dir->path);
// 	entries = get_entries(dir->path, flags);
// 	entries = sort(entries, flags);
// 	list_entries(entries, flags);
// 	list_recursive_dirs(entries, flags, dir->path, printed_list);
// 	free_entries(entries);
// }

// static void	ft_ls_impl(t_flags *flags, char **args, int argc, int *printed_list)
// {
// 	t_entries	*operands;
// 	t_entries	*files;
// 	t_entries	*dirs;
// 	t_entries	*current;
// 	int			print_headers;

// 	operands = get_operand_entries(flags, args, argc);
// 	if (flags->d_directories)
// 	{
// 		if (operands)
// 		{
// 			print_list_separator(printed_list);
// 			list_entries(operands, flags);
// 		}
// 		free_entries(operands);
// 		ft_free_array(args);
// 		return ;
// 	}
// 	split_operands(operands, &files, &dirs);
// 	if (files)
// 	{
// 		print_list_separator(printed_list);
// 		list_entries(files, flags);
// 	}
// 	print_headers = (files || count_entries(dirs) > 1 || flags->R_recursive);
// 	current = dirs;
// 	while (current)
// 	{
// 		list_directory_operand(current, flags, print_headers, printed_list);
// 		current = current->next;
// 	}
// 	free_entries(files);
// 	free_entries(dirs);
// 	ft_free_array(args);
// }

// t_entries *get_files(char **args, int argc, t_flags *flags)
// {
// 	t_entries *head = NULL;
// 	t_entries *new = NULL;
// 	struct stat st;
// 	struct dirent	dir_entry;
// 	for (int i = 0; i < argc; ++i)
// 	{
// 		if (lstat(args[i], &st) == -1)
// 			perror(args[i]);
// 		if (S_ISDIR(st.st_mode))
// 			continue;
// 		ft_bzero(&dir_entry, sizeof(dir_entry));
// 		dir_entry.d_type = get_dtype(st.st_mode);
// 		ft_strlcpy(dir_entry.d_name, args[i], sizeof(dir_entry.d_name));
// 		new = new_entry(&dir_entry, args[i], &st);
// 	}
// }

// single entrypoint for simple recursion
void ft_ls(t_flags *flags, char **args, int argc)
{
	// array of entries linked list 
	t_linked_list *files = get_files(flags, args, argc);
	t_linked_list *dirs = get_dirs(flags, args, argc);
	if (!entries_arr)
	{
		ft_free_array(args);
		return ;
	}
	// print_array(args);
	for (int i = 0; i < argc; ++i)
	{
		entries_arr[i] = get_entries(args[i], flags);
		entries_arr[i] = sort(entries_arr[i], flags);
		if (argc > 1 || flags->R_recursive)
			ft_printf("%s:\n", args[i]);
		list_entries(entries_arr[i], flags);
	}
	// if (flags->R_recursive)
	// {
	// 	for (int i = 0; i < argc; ++i)
	// 	{
	// 		int dir_count = count_dirs(entries_arr[i]);
	// 		char **new_args = get_dirs(entries_arr[i], dir_count, args[i]);
	// 		// print_array(new_args);
	// 		if (new_args)
	// 			ft_ls(flags, new_args, dir_count);
	// 	}
	// }
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
