NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./include -I./libft
DEBUG_FLAG = -g3
LDFLAGS = -lreadline -lhistory

SRC_DIR = src
OBJ_DIR = obj

SRC = main.c history.c signals.c free_all.c tokenization.c
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

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

debug: $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(OBJ) -L. $(LIBFT)  $(LDFLAGS) -o $(NAME)

$(LIBFT): $(LIBFT_OBJ)
	make bonus -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(LIBFT_OBJ) $(OBJ) $(LIB)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all