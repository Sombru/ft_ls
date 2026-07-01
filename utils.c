#include "ft_ls.h"

static char	*join_path(char *parent, char *child)
{
	char	*tmp;
	char	*path;

	if (!ft_strcmp(parent, "/"))
		return (ft_strjoin(parent, child));
	tmp = ft_strjoin(parent, "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, child);
	free(tmp);
	return (path);
}

// function counts directoreis in entries list
int count_dirs(t_entries *entries)
{
	int count = 0;
	while (entries)
	{
		if (!ft_strcmp(entries->entry->d_name, ".") || 
			!ft_strcmp(entries->entry->d_name, ".."))
		{
			entries = entries->next;
			continue;
		}
		if (entries->entry->d_type == DT_DIR)
		{
			count++;
		}
		entries = entries->next;
	}
	return count;
}


char **get_dirs(t_entries *entries, int dir_count, char *parent_path)
{
	char **names = ft_calloc(dir_count + 1, sizeof(char *));
	int i = 0;

	if (!names)
		return (NULL);
	while (entries)
	{
		if (!ft_strcmp(entries->entry->d_name, ".") || 
			!ft_strcmp(entries->entry->d_name, ".."))
		{
			entries = entries->next;
			continue;
		}
		if (entries->entry->d_type == DT_DIR)
		{
			names[i] = join_path(parent_path, entries->entry->d_name);
			if (!names[i])
			{
				ft_free_array(names);
				return (NULL);
			}
			++i;
		}
		entries = entries->next;
	}

	return names;
}
