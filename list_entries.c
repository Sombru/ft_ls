#include "ft_ls.h"

static int	is_visible(t_entries *entry, t_flags *flags)
{
	return (flags->a_all || entry->entry->d_name[0] != '.');
}

static int	get_terminal_width(void)
{
	struct winsize	ws;

	if (ioctl(1, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
		return (80);
	return (ws.ws_col);
}

static int	count_visible_entries(t_entries *entries, t_flags *flags)
{
	int	count;

	count = 0;
	while (entries)
	{
		if (is_visible(entries, flags))
			count++;
		entries = entries->next;
	}
	return (count);
}

static int	get_max_name_len(t_entries *entries, t_flags *flags)
{
	int	len;
	int	max;

	max = 0;
	while (entries)
	{
		if (is_visible(entries, flags))
		{
			len = ft_strlen(entries->entry->d_name);
			if (len > max)
				max = len;
		}
		entries = entries->next;
	}
	return (max);
}

static t_entries	**entries_to_array(t_entries *entries, t_flags *flags,
		int count)
{
	t_entries	**array;
	int			i;

	array = malloc(sizeof(t_entries *) * count);
	if (!array)
		return (NULL);
	i = 0;
	while (entries)
	{
		if (is_visible(entries, flags))
			array[i++] = entries;
		entries = entries->next;
	}
	return (array);
}

static char	*get_column_format(int col_width)
{
	char	*width;
	char	*tmp;
	char	*format;

	width = ft_itoa(col_width);
	if (!width)
		return (NULL);
	tmp = ft_strjoin("%-", width);
	free(width);
	if (!tmp)
		return (NULL);
	format = ft_strjoin(tmp, "s");
	free(tmp);
	return (format);
}

static char	*get_right_format(int width)
{
	char	*width_str;
	char	*tmp;
	char	*format;

	width_str = ft_itoa(width);
	if (!width_str)
		return (NULL);
	tmp = ft_strjoin("%", width_str);
	free(width_str);
	if (!tmp)
		return (NULL);
	format = ft_strjoin(tmp, "s");
	free(tmp);
	return (format);
}

static int	num_len(long long n)
{
	int	len;

	len = 1;
	if (n < 0)
	{
		len++;
		n = -n;
	}
	while (n >= 10)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static char	*ft_lltoa(long long n)
{
	char	*str;
	int		len;

	len = num_len(n);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (n == 0)
		str[0] = '0';
	if (n < 0)
	{
		str[0] = '-';
		n = -n;
	}
	while (n > 0)
	{
		str[--len] = (n % 10) + '0';
		n /= 10;
	}
	return (str);
}

static void	print_right_field(long long n, int width)
{
	char	*format;
	char	*value;

	format = get_right_format(width);
	value = ft_lltoa(n);
	if (!format || !value)
	{
		free(format);
		free(value);
		return ;
	}
	ft_printf(format, value);
	free(format);
	free(value);
}

static void	print_ll(long long n)
{
	char	*value;

	value = ft_lltoa(n);
	if (!value)
		return ;
	ft_putstr_fd(value, 1);
	free(value);
}

static void	print_left_field(char *str, int width)
{
	char	*format;

	format = get_column_format(width);
	if (!format)
		return ;
	ft_printf(format, str);
	free(format);
}

static void	list_one_per_line(t_entries *entries, t_flags *flags)
{
	while (entries)
	{
		if (is_visible(entries, flags))
			ft_putendl_fd(entries->entry->d_name, 1);
		entries = entries->next;
	}
}

static void	get_mode_string(mode_t mode, char mode_str[11])
{
	if (S_ISDIR(mode))
		mode_str[0] = 'd';
	else if (S_ISLNK(mode))
		mode_str[0] = 'l';
	else if (S_ISCHR(mode))
		mode_str[0] = 'c';
	else if (S_ISBLK(mode))
		mode_str[0] = 'b';
	else if (S_ISFIFO(mode))
		mode_str[0] = 'p';
	else if (S_ISSOCK(mode))
		mode_str[0] = 's';
	else
		mode_str[0] = '-';
	mode_str[1] = (mode & S_IRUSR) ? 'r' : '-';
	mode_str[2] = (mode & S_IWUSR) ? 'w' : '-';
	mode_str[3] = (mode & S_IXUSR) ? 'x' : '-';
	mode_str[4] = (mode & S_IRGRP) ? 'r' : '-';
	mode_str[5] = (mode & S_IWGRP) ? 'w' : '-';
	mode_str[6] = (mode & S_IXGRP) ? 'x' : '-';
	mode_str[7] = (mode & S_IROTH) ? 'r' : '-';
	mode_str[8] = (mode & S_IWOTH) ? 'w' : '-';
	mode_str[9] = (mode & S_IXOTH) ? 'x' : '-';
	mode_str[10] = '\0';
}

static int	get_owner_len(t_entries *entry)
{
	struct passwd	*pw;

	pw = getpwuid(entry->st.st_uid);
	if (pw)
		return (ft_strlen(pw->pw_name));
	return (num_len(entry->st.st_uid));
}

static int	get_group_len(t_entries *entry)
{
	struct group	*gr;

	gr = getgrgid(entry->st.st_gid);
	if (gr)
		return (ft_strlen(gr->gr_name));
	return (num_len(entry->st.st_gid));
}

static void	get_long_widths(t_entries *entries, t_flags *flags, int widths[4])
{
	widths[0] = 0;
	widths[1] = 0;
	widths[2] = 0;
	widths[3] = 0;
	while (entries)
	{
		if (is_visible(entries, flags))
		{
			if (num_len(entries->st.st_nlink) > widths[0])
				widths[0] = num_len(entries->st.st_nlink);
			if (get_owner_len(entries) > widths[1])
				widths[1] = get_owner_len(entries);
			if (get_group_len(entries) > widths[2])
				widths[2] = get_group_len(entries);
			if (num_len(entries->st.st_size) > widths[3])
				widths[3] = num_len(entries->st.st_size);
		}
		entries = entries->next;
	}
}

static long long	get_total_blocks(t_entries *entries, t_flags *flags)
{
	long long	total;

	total = 0;
	while (entries)
	{
		if (is_visible(entries, flags))
			total += entries->st.st_blocks;
		entries = entries->next;
	}
	return (total / 2);
}

static void	print_owner_group(t_entries *entry, t_flags *flags, int widths[4])
{
	struct passwd	*pw;
	struct group	*gr;
	char			*uid;
	char			*gid;

	pw = getpwuid(entry->st.st_uid);
	gr = getgrgid(entry->st.st_gid);
	if (!flags->g_list)
	{
		if (pw)
			print_left_field(pw->pw_name, widths[1]);
		else
		{
			uid = ft_lltoa(entry->st.st_uid);
			print_left_field(uid, widths[1]);
			free(uid);
		}
		ft_putchar_fd(' ', 1);
	}
	if (gr)
		print_left_field(gr->gr_name, widths[2]);
	else
	{
		gid = ft_lltoa(entry->st.st_gid);
		print_left_field(gid, widths[2]);
		free(gid);
	}
	ft_putchar_fd(' ', 1);
}

static void	print_time(time_t timestamp)
{
	char	*time_str;

	time_str = ctime(&timestamp);
	if (!time_str)
		return ;
	write(1, time_str + 4, 12);
	ft_putchar_fd(' ', 1);
}

static void	print_symlink_target(t_entries *entry)
{
	char	buffer[4096];
	ssize_t	len;

	if (!S_ISLNK(entry->st.st_mode))
		return ;
	len = readlink(entry->path, buffer, sizeof(buffer) - 1);
	if (len == -1)
		return ;
	buffer[len] = '\0';
	ft_putstr_fd(" -> ", 1);
	ft_putstr_fd(buffer, 1);
}

static void	print_long_entry(t_entries *entry, t_flags *flags, int widths[4])
{
	char	mode_str[11];

	get_mode_string(entry->st.st_mode, mode_str);
	ft_putstr_fd(mode_str, 1);
	ft_putchar_fd(' ', 1);
	print_right_field(entry->st.st_nlink, widths[0]);
	ft_putchar_fd(' ', 1);
	print_owner_group(entry, flags, widths);
	print_right_field(entry->st.st_size, widths[3]);
	ft_putchar_fd(' ', 1);
	print_time(entry->st.st_mtime);
	ft_putstr_fd(entry->entry->d_name, 1);
	print_symlink_target(entry);
	ft_putchar_fd('\n', 1);
}

static void	list_long(t_entries *entries, t_flags *flags)
{
	int	widths[4];

	get_long_widths(entries, flags, widths);
	ft_putstr_fd("total ", 1);
	print_ll(get_total_blocks(entries, flags));
	ft_putchar_fd('\n', 1);
	while (entries)
	{
		if (is_visible(entries, flags))
			print_long_entry(entries, flags, widths);
		entries = entries->next;
	}
}

static void list_rows(t_entries *entries, t_flags *flags)
{
	t_entries	**array;
	char		*format;
	int			count;
	int			col_width;
	int			cols;
	int			rows;
	int			i;
	int			row;
	int			col;

	count = count_visible_entries(entries, flags);
	if (count == 0)
		return ;
	col_width = get_max_name_len(entries, flags) + 2;
	cols = get_terminal_width() / col_width;
	if (cols < 1)
		cols = 1;
	rows = (count + cols - 1) / cols;
	array = entries_to_array(entries, flags, count);
	if (!array)
		return (list_one_per_line(entries, flags));
	format = get_column_format(col_width);
	if (!format)
	{
		free(array);
		return (list_one_per_line(entries, flags));
	}
	row = 0;
	while (row < rows)
	{
		col = 0;
		while (col < cols)
		{
			i = row + (col * rows);
			if (i < count)
			{
				if (col + 1 < cols && row + ((col + 1) * rows) < count)
					ft_printf(format, array[i]->entry->d_name);
				else
					ft_putstr_fd(array[i]->entry->d_name, 1);
			}
			col++;
		}
		ft_putchar_fd('\n', 1);
		row++;
	}
	free(format);
	free(array);
}

void	list_entries(t_entries *entries, t_flags *flags)
{
	if (flags->l_list || flags->g_list)
		list_long(entries, flags);
	else if (!isatty(1) || flags->f_1)
		list_one_per_line(entries, flags);
	else
		list_rows(entries, flags);
}
