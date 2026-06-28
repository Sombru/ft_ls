#include "ft_ls.h"

void	free_entries(t_entries *entries)
{
	t_entries	*next;

	while (entries)
	{
		next = entries->next;
		free(entries->entry);
		free(entries);
		entries = next;
	}
}

t_entries	*new_entry(const struct dirent *dir_entry)
{
	t_entries	*new;

	new = ft_calloc(1, sizeof(t_entries));
	if (!new)
		return (NULL);
	new->entry = ft_calloc(1, sizeof(struct dirent));
	if (!new->entry)
	{
		free(new);
		return (NULL);
	}
	ft_memcpy(new->entry, dir_entry, sizeof(struct dirent));
	return (new);
}

int	append_entry(t_entries **entries, t_entries *new)
{
	t_entries	*current;

	if (!entries || !new)
		return (0);
	if (!*entries)
	{
		*entries = new;
		return (1);
	}
	current = *entries;
	while (current->next)
		current = current->next;
	current->next = new;
	return (1);
}

t_entries	*get_entries(const char *path)
{
	DIR				*dir;
	struct dirent	*dir_entry;
	t_entries			*entries;
	t_entries			*new;

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
