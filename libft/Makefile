PROJECT				=		libft
CC					=		cc
CFLAGS				=		-Wall -Werror -Wextra -I inc -g3
DEPFLAGS			=		-MMD -MP
AR					=		ar -crs

DPRINTF_DIR			:=		ft_dprintf/
DPRINTF				:=		ft_dprintf ft_dprintf_utils

FTIS_DIR			:=		ft_is/
FTIS				:=		ft_isalnum ft_isalpha ft_isascii ft_isdigit ft_isprint ft_isspace ft_ishex_alpha ft_ishexALPHA

FTMEM_DIR			:=		ft_mem/
FTMEM				:=		ft_bzero ft_calloc ft_memchr ft_memcmp ft_memmove ft_memcpy ft_memset ft_memswap	
FTPUT_DIR			:=		ft_put/
FTPUT				:=		ft_putchar_fd ft_putendl_fd ft_putnbr_fd ft_putstr_fd

FTSTR_DIR			:=		ft_str/
FTSTR				:=		ft_split ft_strchr ft_strcmp ft_strdup ft_striteri ft_strjoin ft_strlcat ft_strlcpy ft_strlen ft_strmapi ft_strncmp ft_strnstr ft_strrchr ft_strtrim ft_substr ft_strchr_in

FTTO_DIR			:=		ft_to/
FTTO				:=		ft_atod ft_atoi ft_atol ft_itoa ft_tolower ft_toupper ft_swap ft_atoi_base

GNL_DIR				:=		ak_gnl/
GNL					:=		get_next_line get_next_line_utils

SORT_DIR			:=		sort/
SORT				:=		ft_quick_sort\

SRC_FILES			+=		$(addprefix $(DPRINTF_DIR),$(DPRINTF))
SRC_FILES			+=		$(addprefix $(FTIS_DIR),$(FTIS))
SRC_FILES			+=		$(addprefix $(FTMEM_DIR),$(FTMEM))
SRC_FILES			+=		$(addprefix $(FTPUT_DIR),$(FTPUT))
SRC_FILES			+=		$(addprefix $(FTSTR_DIR),$(FTSTR))
SRC_FILES			+=		$(addprefix $(FTTO_DIR),$(FTTO))
SRC_FILES			+=		$(addprefix $(GNL_DIR),$(GNL))
SRC_FILES			+=		$(addprefix $(SORT_DIR),$(SORT))

SRC_DIR 			=		src/
OBJ_DIR				=		obj/
TREE				=		.cache_exists

SRC 				= 		$(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))
OBJ 				= 		$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))
DEP 				:=		$(patsubst %.o, %.d, $(OBJ))

NAME				=		libft.a

all					:		$(NAME)
							@echo "$(GREEN)$(PROJECT) : All done!$(DEFAULT)"

$(NAME)				:		$(OBJ)
							@$(AR) $@ $^
							@echo "$(GREEN)$(PROJECT) : Archive created!$(DEFAULT)"

$(OBJ_DIR)%.o 		: 		$(SRC_DIR)%.c | $(TREE)
							@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@
# @echo "$(YELLOW)$(PROJECT) : $< $(DEFAULT)"

$(TREE)				:
							@mkdir -p $(OBJ_DIR)
							@mkdir -p $(OBJ_DIR)$(ALLOC_DIR)
							@mkdir -p $(OBJ_DIR)$(BTREE_DIR)
							@mkdir -p $(OBJ_DIR)$(CMD_DIR)
							@mkdir -p $(OBJ_DIR)$(DLST_DIR)
							@mkdir -p $(OBJ_DIR)$(LST_DIR)
							@mkdir -p $(OBJ_DIR)$(DPRINTF_DIR)
							@mkdir -p $(OBJ_DIR)$(FREE_DIR)
							@mkdir -p $(OBJ_DIR)$(FTIS_DIR)
							@mkdir -p $(OBJ_DIR)$(FTMEM_DIR)
							@mkdir -p $(OBJ_DIR)$(FT_PRINTF_DIR)
							@mkdir -p $(OBJ_DIR)$(FTPUT_DIR)
							@mkdir -p $(OBJ_DIR)$(FTSTR_DIR)
							@mkdir -p $(OBJ_DIR)$(FTTO_DIR)
							@mkdir -p $(OBJ_DIR)$(GNL_DIR)
							@mkdir -p $(OBJ_DIR)$(SORT_DIR)

clean				:
							@rm -rf $(OBJ_DIR)
							@echo "$(CYAN)$(PROJECT) : Objs & Deps files cleaned!$(DEFAULT)"

fclean				:		clean
							@rm -rf $(NAME)
							@echo "$(PURPLE)$(PROJECT) : Content cleaned!$(DEFAULT)"

re					:		fclean all
							@echo "$(PURPLE)$(PROJECT) : Clean and rebuilt everything!$(DEFAULT)"

norm:
							@clear
							@if ! norminette | grep "Error"; then echo "\033[32mOK!\033[0m"; fi

.PHONY				:		all clean fclean re norm test

-include $(DEP)

DEFAULT				= 		\033[0;39m
PURPLE				=		\033[0;35m
GRAY				= 		\033[0;90m
RED					=		\033[0;91m
GREEN				=		\033[0;92m
YELLOW				=		\033[0;93m
BLUE				=		\033[0;94m
CYAN				=		\033[0;96m
WHITE				= 		\033[0;97m