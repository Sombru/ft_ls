#include "ft_ls.h"

// function counts directoreis in entries list
int count_dirs(t_entries *entries)
{
	int count = 0;
	while (entries)
	{
		entries = entries->next;
		if (entries->entry->d_type == DT_DIR)
		{
			count++;
		}
	}
	return count;
}


char **get_dirs(t_entries *entries, int dir_count)
{
	char **names = malloc(sizeof(char *) * dir_count);
	int i = 0;
	while (entries)
	{
		entries = entries->next;
		if (entries->entry->d_type == DT_DIR)
		{
			names[i] = ft_strdup(entries->entry->d_name);
			++i;
		}
	}

	return names;
}