#include "ft_ls.h"
#include <errno.h>

void free_entries(t_entries *entries)
{
	t_entries *next;

	while (entries)
	{
		next = entries->next;
		free(entries->path);
		free(entries->entry);
		free(entries);
		entries = next;
	}
}

static char	*join_path(const char *parent, const char *child)
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

t_entries *new_entry(const struct dirent *dir_entry, char *path,
		struct stat *st)
{
	t_entries *new;

	new = ft_calloc(1, sizeof(t_entries));
	if (!new)
		return (NULL);
	new->entry = ft_calloc(1, sizeof(struct dirent));
	new->path = ft_strdup(path);
	new->st = *st;
	if (!new->entry || !new->path)
	{
		free(new->path);
		free(new->entry);
		free(new);
		return (NULL);
	}
	ft_memcpy(new->entry, dir_entry, sizeof(struct dirent));
	return (new);
}

static t_entries	*get_argument_entry(const char *path, int opendir_errno,
		int allow_dir)
{
	struct dirent	dir_entry;
	struct stat		st;
	t_entries		*entry;

	if (lstat(path, &st) == -1)
	{
		perror(path);
		return (NULL);
	}
	if (!allow_dir && S_ISDIR(st.st_mode))
	{
		errno = opendir_errno;
		perror(path);
		return (NULL);
	}
	ft_bzero(&dir_entry, sizeof(dir_entry));
	dir_entry.d_type = get_dtype(st.st_mode);
	ft_strlcpy(dir_entry.d_name, path, sizeof(dir_entry.d_name));
	entry = new_entry(&dir_entry, (char *)path, &st);
	if (entry)
		entry->is_argument = true;
	return (entry);
}

int append_entry(t_entries **entries, t_entries *new)
{
	t_entries *current;

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

t_entries *get_entries(const char *path, t_flags *flags)
{
	struct dirent *dir_entry;
	struct stat st;
	t_entries *entries;
	t_entries *new;
	char *entry_path;
	DIR *dir;

	if (flags->d_directories)
		return (get_argument_entry(path, 0, 1));
	dir = opendir(path);
	if (!dir)
		return (get_argument_entry(path, errno, 0));

	entries = NULL;
	dir_entry = readdir(dir);
	while (dir_entry)
	{
		entry_path = join_path(path, dir_entry->d_name);
		if (!entry_path || lstat(entry_path, &st))
		{
			free(entry_path);
			free_entries(entries);
			closedir(dir);
			return (NULL);
		}
		new = new_entry(dir_entry, entry_path, &st);
		free(entry_path);
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


t_linked_list *get_files(t_flags *flags, char **args, int argc)
{
	t_linked_list 	*list = linked_list_new();
	struct stat		st;
	for (int i = 0; i < argc; ++i)
	{
		if ((lstat(args[i]), &st) == -1)
		{
			perror(args[i]);
			continue;
		}
		if (S_ISDIR(st.st_mode))
			continue;
		t_entries *entry;
		struct dirent dir_entry;
		ft_bzero(&dir_entry, sizeof(dir_entry));
		dir_entry.d_type = get_dtype(st.st_mode);
		ft_strlcpy(dir_entry.d_name, args[i], sizeof(dir_entry.d_name));
		entry = new_entry(&dir_entry, args[i], &st);
		linkedlist_add(list, entry);
	}
	
}
t_linked_list *get_dirs(t_flags *flags, char **args, int argc)
{
	t_linked_list *list = linked_list_new()
	DIR *dir;

}