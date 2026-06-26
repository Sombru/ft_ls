#include "ft_ls.h"

t_entry	*new_entry(const struct dirent *dir_entry)
{
	t_entry	*new;

	new = ft_calloc(1, sizeof(t_entry));
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

int	append_entry(t_entry **entries, t_entry *new)
{
	t_entry	*current;

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

void	list_entries(t_entry *entries)
{
	while (entries)
	{
		ft_putstr_fd(entries->entry->d_name, 1);
		if (entries->next)
			ft_putstr_fd("  ", 1);
		entries = entries->next;
	}
	ft_putchar_fd('\n', 1);
}

void	free_entries(t_entry *entries)
{
	t_entry	*next;

	while (entries)
	{
		next = entries->next;
		free(entries->entry);
		free(entries);
		entries = next;
	}
}
