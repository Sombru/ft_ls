#include "ft_ls.h"

// typedef int (*t_cmp_entries)(t_entries *a, t_entries *b);

static char	*get_basename(char *name)
{
	char	*base;

	base = ft_strrchr(name, '/');
	if (base)
		return (base + 1);
	return (name);
}

static char	*get_sort_name(t_entries *entry)
{
	char	*name;

	if (entry->is_argument)
		name = get_basename(entry->path);
	else
		name = entry->entry->d_name;
	if (name[0] == '.' && name[1] && !(name[1] == '.' && !name[2]))
		return (name + 1);
	return (name);
}

static int	cmp_alpha(t_entries *a, t_entries *b)
{
	int	result;

	result = ft_strcmp(get_sort_name(a), get_sort_name(b));
	if (result)
		return (result);
	return (ft_strcmp(get_basename(a->path), get_basename(b->path)));
}

static int	cmp_time(t_entries *a, t_entries *b)
{
	if (a->st.st_mtime > b->st.st_mtime)
		return (-1);
	if (a->st.st_mtime < b->st.st_mtime)
		return (1);
	return (cmp_alpha(a, b));
}

static int	cmp_access_time(t_entries *a, t_entries *b)
{
	if (a->st.st_atime > b->st.st_atime)
		return (-1);
	if (a->st.st_atime < b->st.st_atime)
		return (1);
	return (cmp_alpha(a, b));
}

static void	split_list(t_entries *src, t_entries **left, t_entries **right)
{
	t_entries	*slow;
	t_entries	*fast;

	slow = src;
	fast = src->next;
	while (fast)
	{
		fast = fast->next;
		if (fast)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*left = src;
	*right = slow->next;
	slow->next = NULL;
}

static t_entries	*merge_lists(t_entries *a, t_entries *b, int (*cmp)(t_entries *a, t_entries *b))
{
	t_entries	dummy;
	t_entries	*tail;

	tail = &dummy;
	dummy.next = NULL;
	while (a && b)
	{
		if (cmp(a, b) <= 0)
		{
			tail->next = a;
			a = a->next;
		}
		else
		{
			tail->next = b;
			b = b->next;
		}
		tail = tail->next;
	}
	if (a)
		tail->next = a;
	else
		tail->next = b;
	return (dummy.next);
}

// merge sort
static t_entries	*sort_entries(t_entries *list, int (*cmp)(t_entries *a, t_entries *b))
{
	t_entries	*left;
	t_entries	*right;

	if (!list || !list->next || !cmp)
		return (list);
	split_list(list, &left, &right);
	left = sort_entries(left, cmp);
	right = sort_entries(right, cmp);
	return (merge_lists(left, right, cmp));
}

static t_entries	*reverse_entries(t_entries *entries)
{
	t_entries	*prev;
	t_entries	*next;

	prev = NULL;
	while (entries)
	{
		next = entries->next;
		entries->next = prev;
		prev = entries;
		entries = next;
	}
	return (prev);
}

// default ls sorts alphabetically
t_entries *sort(t_entries *entries, t_flags *flags)
{
	int (*cmp)(t_entries *, t_entries *) = cmp_alpha; // comparator function

	if (flags->U_sort)
		cmp = NULL;
	else if (flags->u_sort)
		cmp = cmp_access_time;
	else if (flags->t_time)
		cmp = cmp_time;
		
	entries = sort_entries(entries, cmp);
	if (flags->r_reverse && !flags->U_sort)
		entries = reverse_entries(entries);
	return entries;
}
