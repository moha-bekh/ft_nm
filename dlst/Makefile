NAME	=	libdlst.a

CC		=	cc
INC		=	-I inc
ERR_FLG	=	-Wall -Wextra -Werror -g3
DEP_FLG	=	-MMD -MP
AR		=	ar rcs

SRC_DIR	:=	src/
OBJ_DIR	:=	obj/

SRC_FLS	:=	\
			dlst_clear\
			dlst_del\
			dlst_foreach\
			dlst_foreach_reverse\
			dlst_getters\
			dlst_insert_next\
			dlst_insert_prev\
			dlst_new\
			dlst_node_new\
			dlst_pop_back\
			dlst_pop_front\
			dlst_push_back\
			dlst_push_front\
			dlst_sort\

SRC		:=	$(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FLS)))
OBJ		:=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FLS)))
DEP		:=	$(patsubst %.o, %.d, $(OBJ))


all		:	$(NAME)

$(NAME)	:	$(OBJ)
			$(AR) $@ $^

$(OBJ_DIR)%.o	:	$(SRC_DIR)%.c | obj_dir
			$(CC) $(ERR_FLG) $(INC) $(DEP_FLG) -c $< -o $@

obj_dir	:
			mkdir -p $(OBJ_DIR)

clean	:
			rm -rf $(OBJ_DIR)

fclean	: 	clean
			rm -f $(NAME)

re		:	fclean all

test	:	all
			$(MAKE) -C tests run

.PHONY	:	all clean fclean re test

-include $(DEP)
