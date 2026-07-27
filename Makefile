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

dep:
	make -C dlst
	make -C libft

# -Wall -Wextra -Werror -MMD -MP -g3
$(NAME): $(OBJS)
	cc -g3 $^ -L dlst -l dlst -L libft -l ft -o $@

# -Wall -Wextra -Werror -MMD -MP -g3
build/%.o: src/%.c
	mkdir -p $(dir $@)
	cc -g3 -I inc -I dlst/inc -I libft/inc -c $< -o $@

clean:
	rm -f $(OBJS)

clean-all:
	rm -f $(OBJS)
	make -C dlst clean
	make -C libft clean

fclean: clean
	rm -f $(NAME)

fclean-all:
	rm -f $(NAME)
	make -C dlst fclean
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re
