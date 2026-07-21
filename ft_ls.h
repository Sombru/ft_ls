#ifndef FT_LS_H
#define FT_LS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/xattr.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "libft.h"

typedef struct s_flags
{
	bool a_all; // list hidden 
	bool C_list; // force listing by columns
	bool f_1; // force one_per_line
	bool d_directories; // list directories themselves, not their contents (ls -d *)
	bool f_list; // enable -aU
	bool l_list; // long list 
	bool R_recursive; // list subdirs recursively
	bool r_reverse; // reverse order when sorting
	bool t_time; // sort by time newset -> oldest
	bool u_sort; // sort by access time
	bool U_sort; // do not sort, list in directory order
	bool g_list; // like -l but without the owner
}	t_flags;

typedef struct s_entries
{
	struct dirent		*entry;
	char				*path;
	struct stat			st;
	bool				is_argument;
	struct s_entries	*next;
	
}	t_entries;

typedef struct s_test
{
	char *str
} t_test;

char	**get_dirs(t_entries *entries, int dir_count, char *parent_path,
			t_flags *flags);
char	**parse_ls(t_flags *flags, int argc, char *argv[]);
int		count_dirs(t_entries *entries, t_flags *flags);
int		append_entry(t_entries **entries, t_entries *new);
void	list_entries(t_entries *entries, t_flags *flags);
void	free_entries(t_entries *entries);
void	ft_ls(t_flags *flags, char **args, int argc);
void	print_flags(const t_flags *flags);
void	print_array(char **array);

t_entries *new_entry(const struct dirent *dir_entry, char *path, struct stat *st);
unsigned char	get_dtype(mode_t mode);
t_entries	*get_entries(const char *path, t_flags *flags);
t_entries *sort(t_entries *entries, t_flags *flags);

#endif
