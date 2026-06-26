#ifndef FT_LS_H
#define FT_LS_H

#include <sys/types.h>
#include <dirent.h>
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

t_flags	*parse_ls_flags(int argc, char *argv[]);
void	print_flags(const t_flags *flags);

#endif
