SRCS :=	\
				src/main.c \
				src/cli.c \
				src/parse.c \
				src/elf64.c \
				src/elf32.c \
				src/filter.c \
				src/sort.c \
				src/print.c

OBJS := $(SRCS:src/%.c=build/%.o)

NAME := ft_nm

all: $(NAME)

$(NAME): $(OBJS)
	make -C dlst
	make -C libft
	cc -Wall -Wextra -Werror -MMD -MP -g3 $^ -L dlst -l dlst -L libft -l ft -o $@

build/%.o: src/%.c
	mkdir -p $(dir $@)
	cc -Wall -Wextra -Werror -MMD -MP -g3 -I inc -I dlst/inc -I libft/inc -c $< -o $@

clean:
	make -C dlst clean
	make -C libft clean
	rm -f $(OBJS)

fclean: clean
	make -C dlst fclean
	make -C libft fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
