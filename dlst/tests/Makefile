NAME	=	test_dlst

CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror -g3
INC		=	-I ../inc
LIB		=	../libdlst.a
SRC		=	test_dlst.c

all		:	$(NAME)

$(LIB)	:
			$(MAKE) -C ..

$(NAME)	:	$(SRC) $(LIB)
			$(CC) $(CFLAGS) $(INC) $(SRC) $(LIB) -o $(NAME)

run		:	$(NAME)
			./$(NAME)

clean	:
			rm -f $(NAME)
			rm -rf $(NAME).dSYM

re		:	clean all

.PHONY	:	all run clean re
