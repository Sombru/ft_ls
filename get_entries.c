#include "ft_ls.h"

t_entry	*get_entries(const char *path)
{
	DIR				*dir;
	struct dirent	*dir_entry;
	t_entry			*entries;
	t_entry			*new;

	dir = opendir(path);
	if (!dir)
	{
		perror(path);
		return (NULL);
	}
	entries = NULL;
	dir_entry = readdir(dir);
	while (dir_entry)
	{
		new = new_entry(dir_entry);
		if (!new || !append_entry(&entries, new))
		{
			free_entries(entries);
			closedir(dir);
			return (NULL);
		}
		dir_entry = readdir(dir);
	}
	closedir(dir);
	return (entries);
}
