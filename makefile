# Program name
NAME = ft_ls
# Compiler
CC    = cc
FLAGS = -g -Wall -Wextra -Werror -I libft/

#Libft
LIBFT = libft/libft.a

# Sources
SRC = main.c 

# Objects
OBJ_PATH = obj/
OBJ 	 = $(SRC:.c=.o) 
OBJS	 = $(addprefix $(OBJ_PATH), $(OBJ))
BONUS_OBJ = $(BONUS_SRC:.c=.o)

# Targets
all: $(OBJ_PATH) $(LIBFT) $(NAME)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

$(OBJ_PATH)%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) $(LIBFT) -o $(NAME)

bonus: $(OBJ_PATH) $(LIBFT) $(BONUS_NAME)

$(LIBFT):
	make -C libft --no-print-directory

clean:
	@rm -rf $(OBJ_PATH)
	@make clean -C libft --no-print-directory


fclean: clean
	@rm -f $(NAME)
	@make fclean -C libft --no-print-directory


re: fclean all

.PHONY: all bonus clean fclean re
