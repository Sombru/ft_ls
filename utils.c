#include "ft_ls.h"

unsigned char	get_dtype(mode_t mode)
{
	if (S_ISDIR(mode))
		return (DT_DIR);
	if (S_ISLNK(mode))
		return (DT_LNK);
	if (S_ISREG(mode))
		return (DT_REG);
	if (S_ISCHR(mode))
		return (DT_CHR);
	if (S_ISBLK(mode))
		return (DT_BLK);
	if (S_ISFIFO(mode))
		return (DT_FIFO);
	if (S_ISSOCK(mode))
		return (DT_SOCK);
	return (DT_UNKNOWN);
}


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

static int	is_recursive_dir(t_entries *entry, t_flags *flags)
{
	if (entry->is_argument)
		return (0);
	if (!flags->a_all && entry->entry->d_name[0] == '.')
		return (0);
	if (!ft_strcmp(entry->entry->d_name, ".")
		|| !ft_strcmp(entry->entry->d_name, ".."))
		return (0);
	return (entry->entry->d_type == DT_DIR);
}

// function counts directoreis in entries list
int count_dirs(t_entries *entries, t_flags *flags)
{
	int count = 0;
	while (entries)
	{
		if (is_recursive_dir(entries, flags))
			count++;
		entries = entries->next;
	}
	return count;
}


char **get_dirs(t_entries *entries, int dir_count, char *parent_path,
		t_flags *flags)
{
	char **names = ft_calloc(dir_count + 1, sizeof(char *));
	int i = 0;

	if (!names)
		return (NULL);
	while (entries)
	{
		if (is_recursive_dir(entries, flags))
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

