NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./include -I./libft
DEBUG_FLAG = -g3 -g
LDFLAGS = -lreadline -lhistory

SRC_DIR = src
OBJ_DIR = obj

SRC = history.c signals.c free_all.c print_tree.c

TOK_DIR = Tokenizer
TOK_SRC = tokenizer.c tokenizer_utils.c tokenizer_factory.c \
	word_token_utils.c redirection_token_utils.c

PARSER_DIR = Parser
PARSER_SRC = parser.c parse_command.c parse_simple_cmd.c node_factory.c node_free.c

EXPAND_DIR = Expander
EXPAND_SRC = expander.c expander_utils.c expand_word.c expand_utils.c heredoc.c
EXPAND_SRC_BONUS = expander_bonus.c expander_utils_bonus.c expand_word_bonus.c expand_utils.c heredoc.c expand_astersk.c

BUILTIN_DIR = Builtins
BUILTIN_SRC = builtin_dispatch.c echo.c cd.c pwd.c env.c export.c export_sort.c \
	unset.c mutable_env.c env_crud.c

MAIN_DIR = Main
MAIN_SRC = main.c init_minishell.c parse_and_execute.c

EXEC_DIR = Execution
EXEC_SRC = execution.c execute_cmd.c execute_oper.c execute_pipe.c \
	execute_subshell.c execution_utils.c handle_redirections.c

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(TOK_DIR)/, $(TOK_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(PARSER_DIR)/, $(PARSER_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(EXPAND_DIR)/, $(EXPAND_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(BUILTIN_DIR)/, $(BUILTIN_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(MAIN_DIR)/, $(MAIN_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(EXEC_DIR)/, $(EXEC_SRC:.c=.o))

BONUS_OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(TOK_DIR)/, $(TOK_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(PARSER_DIR)/, $(PARSER_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(EXPAND_DIR)/, $(EXPAND_SRC_BONUS:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(BUILTIN_DIR)/, $(BUILTIN_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(MAIN_DIR)/, $(MAIN_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(EXEC_DIR)/, $(EXEC_SRC:.c=.o))

LIBFT_DIR= ./libft
LIBFT= ./libft/libft.a
LIBFT_SRC = \
ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c \
ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_toupper.c ft_tolower.c \
ft_strlcpy.c ft_strlcat.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c \
ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c \
ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
print_string.c print_pointer.c print_number.c print_unsigned.c print_hexa.c ft_printf.c\
ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c \
ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c \

LIBFT_OBJ := $(addprefix $(LIBFT_DIR)/, $(LIBFT_SRC:.c=.o))

all: $(NAME)

$(NAME) :  $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -L. $(LIBFT)  $(LDFLAGS) -o $(NAME)

bonus:  $(LIBFT) $(BONUS_OBJ) 
	$(CC) $(CFLAGS) $(BONUS_OBJ) -L. $(LIBFT)  $(LDFLAGS) -o $(NAME)
	
debug: $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(OBJ) -L. $(LIBFT)  $(LDFLAGS) -o $(NAME)

$(LIBFT): $(LIBFT_OBJ)
	make bonus -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(TOK_DIR)/%.o: $(SRC_DIR)/$(TOK_DIR)/%.c
	mkdir -p $(OBJ_DIR)/$(TOK_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(PARSER_DIR)/%.o: $(SRC_DIR)/$(PARSER_DIR)/%.c
	mkdir -p $(OBJ_DIR)/$(PARSER_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(EXPAND_DIR)/%.o: $(SRC_DIR)/$(EXPAND_DIR)/%.c
	mkdir -p $(OBJ_DIR)/$(EXPAND_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(BUILTIN_DIR)/%.o: $(SRC_DIR)/$(BUILTIN_DIR)/%.c
	mkdir -p $(OBJ_DIR)/$(BUILTIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(MAIN_DIR)/%.o: $(SRC_DIR)/$(MAIN_DIR)/%.c
	mkdir -p $(OBJ_DIR)/$(MAIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(EXEC_DIR)/%.o: $(SRC_DIR)/$(EXEC_DIR)/%.c
	mkdir -p $(OBJ_DIR)/$(EXEC_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

TOK_OBJ = $(addprefix $(OBJ_DIR)/$(TOK_DIR)/, $(TOK_SRC:.c=.o))

