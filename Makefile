SRCS := \
			 src/main.c

OBJS := $(SRCS:src/%.c=build/%.o)

NAME := ft_nm

all: $(NAME)

# -Wall -Wextra -Werror -MMD -MP -g3
$(NAME): $(OBJS)
	make -C dlst
	make -C libft
	cc -g3 $^ -L dlst -l dlst -L libft -l ft -o $@

# -Wall -Wextra -Werror -MMD -MP -g3
build/%.o: src/%.c
	mkdir -p $(dir $@)
	cc -g3 -I inc -I dlst/inc -I libft/inc -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C dlst clean
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C dlst fclean
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re
