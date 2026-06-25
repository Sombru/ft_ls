#include "ft_ls.h"

int main()
{
	DIR* dir = opendir(".");
	struct dirent* entry;

	if (!dir)
		return 1;	
	while ((entry = readdir(dir)))
	{
		ft_printf("%s\n",entry->d_name);
	}
	
	
}