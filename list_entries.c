#include "ft_ls.h"


void	list_entries(t_entries *entries, t_flags *flags)
{
	while (entries)
	{
		if (!flags->a_all && entries->entry->d_name[0] == '.')
		{
			entries = entries->next;
			continue;
		}
		ft_putstr_fd(entries->entry->d_name, 1);
		if (entries->next)
			ft_putstr_fd("  ", 1);
		entries = entries->next;
	}
	ft_putchar_fd('\n', 1);
}
