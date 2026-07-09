#include "ft_ls.h"

#define C_RESET     "\033[0m"
#define C_BLUE      "\033[34m"
#define C_CYAN      "\033[36m"
#define C_GREEN     "\033[32m"
#define C_YELLOW    "\033[33m"
#define C_MAGENTA   "\033[35m"
#define C_DEV       "\033[33;40m"

// ACL is MacOS thingy
// static char	get_acl_indicator(const char *path)
// {
// 	acl_t	acl;

// 	acl = acl_get_file(path, ACL_TYPE_ACCESS);
// 	if (!acl)
// 		return (' ');

// 	if (acl_equiv_mode(acl, NULL) != 0)
// 	{
// 		acl_free(acl);
// 		return ('+');
// 	}
// 	acl_free(acl);
// 	return (' ');
// }

// setfattr -n user.comment -v "Usefull comment" <file> # give a file extended attribute
static int	has_xattr(const char *path)
{
	ssize_t	size;

	size = listxattr(path, NULL, 0);
	return (size > 0);
}

static char	get_attr_indicator(const char *path)
{
	if (has_xattr(path))
		return ('@');
	// if (has_acl(path))
		// return ('+');
	return (0);
}

const char	*get_color(t_entries *entry)
{
	mode_t	mode;

	mode = entry->st.st_mode;
	if (S_ISDIR(mode))
		return (C_BLUE);
	if (S_ISLNK(mode))
		return (C_CYAN);
	if (S_ISFIFO(mode))
		return (C_YELLOW);
	if (S_ISSOCK(mode))
		return (C_MAGENTA);
	if (S_ISBLK(mode) || S_ISCHR(mode))
		return (C_DEV);
	if (S_ISREG(mode) && (mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
		return (C_GREEN);
	return ("");
}

static char	*get_display_name(t_entries *entry)
{
	if (entry->is_argument)
		return (entry->path);
	return (entry->entry->d_name);
}

static void	print_name(t_entries *entry)
{
	const char	*color;

	color = get_color(entry);
	if (*color)
		ft_printf("%s%s%s",
			color,
			get_display_name(entry),
			C_RESET);
	else
		ft_printf("%s", get_display_name(entry));
}

static int	is_visible(t_entries *entry, t_flags *flags)
{
	return (entry->is_argument || flags->a_all
		|| entry->entry->d_name[0] != '.');
}

static int	get_terminal_width(void)
{
	struct winsize	ws;

	if (ioctl(1, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
		return (80);
	return (ws.ws_col);
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

static void	print_padding(int count)
{
	while (count > 0)
	{
		ft_putchar_fd(' ', 1);
		count--;
	}
}

static void	print_padded_name(t_entries *entry, int width)
{
	int	name_len;

	name_len = ft_strlen(get_display_name(entry));
	print_name(entry);
	print_padding(width - name_len);
}

static void	list_one_per_line(t_entries *entries, t_flags *flags)
{
	while (entries)
	{
		if (is_visible(entries, flags))
		{
			print_name(entries);
			ft_putchar_fd('\n', 1);
		}
		entries = entries->next;
	}
}

static void	get_mode_string(mode_t mode, char mode_str[12], char *path)
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
	mode_str[10] = get_attr_indicator(path);
	mode_str[11] = 0;
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
	char	mode_str[12];

	get_mode_string(entry->st.st_mode, mode_str, entry->path);
	ft_putstr_fd(mode_str, 1);
	ft_putchar_fd(' ', 1);
	print_right_field(entry->st.st_nlink, widths[0]);
	ft_putchar_fd(' ', 1);
	print_owner_group(entry, flags, widths);
	print_right_field(entry->st.st_size, widths[3]);
	ft_putchar_fd(' ', 1);
	print_time(entry->st.st_mtime);
	print_name(entry);
	print_symlink_target(entry);
	ft_putchar_fd('\n', 1);
}

static void	list_long(t_entries *entries, t_flags *flags)
{
	int	widths[4];

	get_long_widths(entries, flags, widths);
	if (!entries || !entries->is_argument)
	{
		ft_putstr_fd("total ", 1);
		print_ll(get_total_blocks(entries, flags));
		ft_putchar_fd('\n', 1);
	}
	while (entries)
	{
		if (is_visible(entries, flags))
			print_long_entry(entries, flags, widths);
		entries = entries->next;
	}
}

static int	count_visible(t_entries *entries, t_flags *flags)
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

static t_entries	*get_visible_at(t_entries *entries, t_flags *flags, int index)
{
	while (entries)
	{
		if (is_visible(entries, flags))
		{
			if (index == 0)
				return (entries);
			index--;
		}
		entries = entries->next;
	}
	return (NULL);
}

static int	get_compact_width(t_entries *entries, t_flags *flags)
{
	int	width;

	width = 0;
	while (entries)
	{
		if (is_visible(entries, flags))
		{
			if (width > 0)
				width += 2;
			width += ft_strlen(get_display_name(entries));
		}
		entries = entries->next;
	}
	return (width);
}

static void	print_compact_row(t_entries *entries, t_flags *flags)
{
	int	first;

	first = 1;
	while (entries)
	{
		if (is_visible(entries, flags))
		{
			if (!first)
				ft_putstr_fd("  ", 1);
			print_name(entries);
			first = 0;
		}
		entries = entries->next;
	}
	ft_putchar_fd('\n', 1);
}

static int	get_column_width(t_entries *entries, t_flags *flags, int layout[3],
		int col)
{
	t_entries	*entry;
	int			width;
	int			len;
	int			row;

	width = 0;
	row = 0;
	while (row < layout[0])
	{
		entry = get_visible_at(entries, flags, col * layout[0] + row);
		if (entry)
		{
			len = ft_strlen(get_display_name(entry));
			if (len > width)
				width = len;
		}
		row++;
	}
	return (width);
}

static int	layout_width(t_entries *entries, t_flags *flags, int layout[3])
{
	int	width;
	int	col;

	width = 0;
	col = 0;
	while (col < layout[1])
	{
		if (col > 0)
			width += 2;
		width += get_column_width(entries, flags, layout, col);
		col++;
	}
	return (width);
}

static void	set_column_layout(t_entries *entries, t_flags *flags, int layout[3])
{
	int	terminal_width;

	terminal_width = get_terminal_width();
	layout[1] = layout[2];
	while (layout[1] > 1)
	{
		layout[0] = (layout[2] + layout[1] - 1) / layout[1];
		if (layout_width(entries, flags, layout) <= terminal_width)
			return ;
		layout[1]--;
	}
	layout[0] = layout[2];
}

static void	print_column_row(t_entries *entries, t_flags *flags, int row,
		int layout[3])
{
	t_entries	*entry;
	int			col;
	int			index;
	int			width;

	col = 0;
	while (col < layout[1])
	{
		index = col * layout[0] + row;
		entry = get_visible_at(entries, flags, index);
		if (entry)
		{
			if (col + 1 < layout[1]
				&& (col + 1) * layout[0] + row < layout[2])
			{
				width = get_column_width(entries, flags, layout, col);
				print_padded_name(entry, width);
				ft_putstr_fd("  ", 1);
			}
			else
				print_name(entry);
		}
		col++;
	}
	ft_putchar_fd('\n', 1);
}

static void	list_columns(t_entries *entries, t_flags *flags)
{
	int	layout[3];
	int	row;

	layout[2] = count_visible(entries, flags);
	if (layout[2] == 0)
		return ;
	if (get_compact_width(entries, flags) <= get_terminal_width())
	{
		print_compact_row(entries, flags);
		return ;
	}
	set_column_layout(entries, flags, layout);
	row = 0;
	while (row < layout[0])
	{
		print_column_row(entries, flags, row, layout);
		row++;
	}
}

void	list_entries(t_entries *entries, t_flags *flags)
{
	if (flags->C_list)
		list_columns(entries, flags);
	if (flags->l_list || flags->g_list)
		list_long(entries, flags);
	else if (!isatty(1) || flags->f_1)
		list_one_per_line(entries, flags);
	else
		list_columns(entries, flags);
}