#include "ft_ls.h"

// void print_flags(const t_flags* flags)
// {
// 	ft_printf("a: %b")
// }

void print_array(char **array)
{
	for (int i = 0; array[i]; ++i)
	{
		ft_printf("arr[%d]: %s\n", i, array[i]);
	}
}