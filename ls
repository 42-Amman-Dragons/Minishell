NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./include -I./libft
DEBUG_FLAG = -g3 -g
LDFLAGS = -lreadline -lhistory

SRC_DIR = src
OBJ_DIR = obj

SRC =

SIGNALS_DIR = Signals
SIGNALS_SRC = signals.c signal_status.c set_signal.c

TOK_DIR = Tokenizer
TOK_SRC = tokenizer.c tokenizer_utils.c tokenizer_factory.c \
	token_word.c token_redirect.c token_operators.c

PARSER_DIR = Parser
PARSER_SRC = parser.c parse_command.c parse_simple_cmd.c node_factory.c node_free.c

EXPAND_DIR = Expander
EXPAND_SRC = expander.c expand_word.c expand_args.c
EXPAND_BONUS_DIR = Expander_bonus
EXPAND_SRC_BONUS = expander_bonus.c expand_args_bonus.c expand_word_bonus.c \
	expand_asterisk_bonus.c generate_expanded_list_bonus.c \
	is_matching_bonus.c add_to_args_bonus.c \
	handle_wild_redirect_bonus.c generate_expanded_list_asterisk_bonus.c
EXPAND_SHARED_DIR = Expander_shared
EXPAND_SHARED_SRC = expand_dollar.c expand_args_utils.c
HEREDOC_DIR = Heredoc
HEREDOC_SRC = heredoc.c heredoc_io.c heredoc_nonint.c heredoc_utils.c

BUILTIN_DIR = Builtins
BUILTIN_SRC = builtin_dispatch.c echo.c cd.c pwd.c env.c export.c export_sort.c \
	unset.c mutable_env.c env_crud.c exit.c exit_utils.c

MAIN_DIR = Main
MAIN_SRC = main.c initializers.c history.c shell_cleanup.c parse_and_execute.c prompt.c prompt_utils.c non_interactive.c

EXEC_DIR = Execution
EXEC_SRC = execution.c execute_cmd.c exec_cmd_utils.c exec_cmd_utils_2.c exec_cmd_err.c \
	execute_oper.c execute_pipe.c exec_pipe_utils.c exec_pipe_utils_2.c \
	execute_subshell.c process_utils.c handle_redirect.c handle_redirect_utils.c
EXEC_SRC_BONUS = execution.c execute_cmd.c exec_cmd_utils.c exec_cmd_utils_2.c exec_cmd_err.c \
	execute_oper_bonus.c execute_pipe.c exec_pipe_utils.c exec_pipe_utils_2.c \
	execute_subshell_bonus.c process_utils.c handle_redirect.c handle_redirect_utils.c

UTILS_DIR = utils
UTILS_SRC = string_utils.c fd_utils.c string_utils_2.c

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(SIGNALS_DIR)/, $(SIGNALS_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(TOK_DIR)/, $(TOK_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(PARSER_DIR)/, $(PARSER_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(EXPAND_DIR)/, $(EXPAND_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(EXPAND_SHARED_DIR)/, $(EXPAND_SHARED_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(HEREDOC_DIR)/, $(HEREDOC_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(BUILTIN_DIR)/, $(BUILTIN_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(MAIN_DIR)/, $(MAIN_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(EXEC_DIR)/, $(EXEC_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(UTILS_DIR)/, $(UTILS_SRC:.c=.o))

BONUS_OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(SIGNALS_DIR)/, $(SIGNALS_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(TOK_DIR)/, $(TOK_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(PARSER_DIR)/, $(PARSER_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(EXPAND_BONUS_DIR)/, $(EXPAND_SRC_BONUS:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(EXPAND_SHARED_DIR)/, $(EXPAND_SHARED_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(HEREDOC_DIR)/, $(HEREDOC_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(BUILTIN_DIR)/, $(BUILTIN_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(MAIN_DIR)/, $(MAIN_SRC:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(EXEC_DIR)/, $(EXEC_SRC_BONUS:.c=.o)) \
	$(addprefix $(OBJ_DIR)/$(UTILS_DIR)/, $(UTILS_SRC:.c=.o))

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

bonus: .bonus

.bonus: $(LIBFT) $(BONUS_OBJ)
	$(CC) $(CFLAGS) $(BONUS_OBJ) -L. $(LIBFT)  $(LDFLAGS) -o $(NAME)
	@touch .bonus

debug: CFLAGS += $(DEBUG_FLAG)
debug: re

debug_bonus: CFLAGS += $(DEBUG_FLAG)
debug_bonus: re
	rm .bonus
debug_bonus: bonus

$(LIBFT): $(LIBFT_OBJ)
	make bonus -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(SIGNALS_DIR)/%.o: $(SRC_DIR)/$(SIGNALS_DIR)/%.c
	mkdir -p $(OBJ_DIR)/$(SIGNALS_DIR)
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

$(OBJ_DIR)/$(EXPAND_BONUS_DIR)/%.o: $(SRC_DIR)/$(EXPAND_BONUS_DIR)/%.c
	mkdir -p $(OBJ_DIR)/$(EXPAND_BONUS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(EXPAND_SHARED_DIR)/%.o: $(SRC_DIR)/$(EXPAND_SHARED_DIR)/%.c
	mkdir -p $(OBJ_DIR)/$(EXPAND_SHARED_DIR)
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

$(OBJ_DIR)/$(HEREDOC_DIR)/%.o: $(SRC_DIR)/$(HEREDOC_DIR)/%.c
	mkdir -p $(OBJ_DIR)/$(HEREDOC_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(UTILS_DIR)/%.o: $(SRC_DIR)/$(UTILS_DIR)/%.c
	mkdir -p $(OBJ_DIR)/$(UTILS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) .bonus
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all


leaks_bonus: debug_bonus
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--suppressions=readline.supp ./$(NAME)

leaks: debug
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--suppressions=readline.supp ./$(NAME)

