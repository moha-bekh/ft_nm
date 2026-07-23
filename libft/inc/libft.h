/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbekheir <mbekheir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/20 09:41:33 by ajordan-          #+#    #+#             */
/*   Updated: 2025/04/15 18:03:28 by mbekheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_ft_dprintf	t_ft_dprintf;

/* FT_PRINTF */
int							ft_dprintf(int fd, const char *format, ...);
void						ft_putchar_buf(t_ft_dprintf *d, char c);
void						ft_putstr_buf(t_ft_dprintf *d, char *str);

/* FT_IS */
int							ft_isalnum(int c);
int							ft_isalpha(int c);
int							ft_isascii(int c);
int							ft_isdigit(int c);
int							ft_isprint(int c);
bool						ft_isspace(int c);
int							ft_ishex_alpha(int c);
int							ft_ishex_alpha_(int c);

/* FT_MEM */
void						ft_bzero(void *s, size_t n);
void						*ft_calloc(size_t count, size_t size);
void						*ft_memchr(const void *s, int c, size_t n);
int							ft_memcmp(const void *s1, const void *s2, size_t n);
void						*ft_memcpy(void *dst, const void *src, size_t n);
void						*ft_memmove(void *dst, const void *src, size_t len);
void						*ft_memset(void *b, int c, size_t len);
int							ft_memswap(void *const a, void *const b,
								size_t size);

/* FT_PUT */
void						ft_putchar_fd(char c, int fd);
void						ft_putendl_fd(char *s, int fd);
void						ft_putnbr_fd(int n, int fd);
void						ft_putstr_fd(char *s, int fd);

/* FT_STR */
char						**ft_split(char const *str, char c);
char						*ft_strchr(const char *s, int c);
int							ft_strcmp(const char *s1, const char *s2);
char						*ft_strdup(const char *s);
void						ft_striteri(char *s, void (*f)(unsigned int,
									char *));
char						*ft_strjoin(char const *s1, char const *s2);
size_t						ft_strlcat(char *dst, const char *src,
								size_t dstsize);
size_t						ft_strlcpy(char *dst, const char *src,
								size_t dstsize);
size_t						ft_strlen(const char *s);
char						*ft_strmapi(char const *s, char (*f)(unsigned int,
									char));
int							ft_strncmp(const char *s1, const char *s2,
								size_t n);
char						*ft_strnstr(const char *haystack,
								const char *needle, size_t len);
char						*ft_strrchr(const char *s, int c);
char						*ft_strtrim(char const *s1, char const *set);
char						*ft_substr(char const *s, unsigned int start,
								size_t len);
char						*ft_strchr_in(char *str, char *to_ad, char target);

/* FT_TO */
double						ft_atod(const char *str);
int							ft_atoi(const char *str);
int							ft_atol(const char *nptr);
char						*ft_itoa(int n);
int							ft_tolower(int c);
int							ft_toupper(int c);
void						ft_swap(int *a, int *b);
int							ft_atoi_base(const char *str, int base);

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef struct s_list
{
	char					*content;
	struct s_list			*next;
}							t_list;

// get_next_line
// char						*get_next_line(int fd);
// void						read_and_stock(int fd, t_list **line_list);
// void						add_to_list(t_list **line_list, char *buf,
// 								int readed);
// void						extract_line(t_list *line_list, char **line);
// void						clean_list(t_list **line_list);
// get_next_line_utils
// int							is_new_line(t_list *line_list);
// t_list						*get_last_node(t_list *line_list);
// void						generate_line(t_list *line_list, char **line);
// void						free_list(t_list *line_list);

/* SORT */
void						quick_sort(int *tab, int start, int end);

/* FT_DPRINTF */
# define FT_DPRINTF_BUFF_SIZE 4096

struct						s_ft_dprintf
{
	int						fd;
	const char				*format;
	va_list					args;
	char					buf[FT_DPRINTF_BUFF_SIZE];
	int						len;
	int						i;
	int						j;
};

int							ak_strlen(char *str);
int							ak_strchr(char *str, char c);
char						*ak_strjoin(char *s1, char *s2);

char						*read_to_newline(int fd, char *buffer);
void						clean_static(char *buffer);
char						*extract_line(char *tmp);
char						*get_next_line(int fd);

#endif /* LIBFT_H */
