#ifndef FT_FPRINTF_H
# define FT_FPRINTF_H

# include <stdarg.h>
# include <stdint.h>
# include "libft.h"

int	ft_fprintf(int fd, const char *format, ...);
int	print_string(char *str, int fd);

#endif