#ifndef FT_LS_H
#define FT_LS_H

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdbool.h>
#include "libft.h"

typedef struct s_flags
{
	bool a_all; // list hidden 
	bool d_directories; // list directories themselves, not their contents (ls -d *)
	bool f_list; // enable -aU
	bool l_list; // long list 
	bool R_recursive; // list subdirs recursively
	bool r_reverse; // reverse order when sorting
	bool t_time; // sort by time newset -> oldest
	bool u_sort; // sort by access time
	bool U_sort; // do not sort, list in directory order
	bool g_list; // like -l bit without the owner
	
} t_flags ;

typedef struct s_entries
{
	struct dirent		*entry;
	struct s_entries	*next;
	
} t_entry;


t_flags	*parse_ls_flags(int argc, char *argv[]);
t_entry	*get_entries(const char *path);
t_entry	*new_entry(const struct dirent *dir_entry);
int		append_entry(t_entry **entries, t_entry *new);
void	list_entries(t_entry *entries, t_flags *flags);
void	free_entries(t_entry *entries);
void	print_flags(const t_flags *flags);

#endif
