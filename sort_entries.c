#include "ft_ls.h"

typedef int (*t_cmp_entries)(t_entries *a, t_entries *b);

static int	cmp_alpha(t_entries *a, t_entries *b)
{
	return (ft_strcmp(a->entry->d_name, b->entry->d_name));
}

// static int	cmp_time(t_entries *a, t_entries *b)
// {
// 	if (a->st.st_mtime > b->st.st_mtime)
// 		return (-1);
// 	if (a->st.st_mtime < b->st.st_mtime)
// 		return (1);
// 	return (cmp_alpha(a, b));
// }

static int	cmp_reverse(t_entries *a, t_entries *b, t_cmp_entries cmp)
{
	return (-cmp(a, b));
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

static t_entries	*merge_lists(t_entries *a, t_entries *b, t_cmp_entries cmp)
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

// default ls sorts alphabetically
t_entries *sort(t_entries **entries, t_flags *flags)
{
	entries = sort_entries(entries, cmp_alpha);
}
t_entries	*sort_entries(t_entries *list, t_flags *flags)
{
	t_entries	*left;
	t_entries	*right;
	t_cmp_entries cmp;

	cmp = cmp_alpha;

	if (!list || !list->next)
		return (list);
	split_list(list, &left, &right);
	left = sort_entries(left, cmp);
	right = sort_entries(right, cmp);
	return (merge_lists(left, right, cmp));
}

