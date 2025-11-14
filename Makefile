NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = minishell.c utils/free.c utils/signal.c utils/utils_2.c utils/utils_3.c expander/question.c expander/dollar_fix.c expander/repair_expand.c \
lexer/lex_clean.c expander/more_space.c lexer/split_env.c lexer/lexer.c expander/quote_stufs.c expander/mini_expand.c lexer/get_len.c expander/expander.c utils/utils_1.c	\
parser/parser.c parser/handle_1.c parser/handle_2.c parser/count.c parser/create.c parser/utils.c parser/redirect_permissions.c expander/handle.c lexer/skip.c parser/new_process.c\
utils/list_to_arr.c builtins/builtin_punctuation.c builtins/builtin_exit.c builtins/builtin_echo.c builtins/builtin_cd.c builtins/builtin_unset.c builtins/builtin_export_1.c \
builtins/builtin_export_2.c execute/heredoc.c execute/heredoc_1.c  execute/redir.c execute/redir_1.c execute/pipe.c execute/heredoc_3.c execute/heredoc_4.c execute/pipe_1.c\
execute/heredoc_5.c execute/cmd.c execute/path.c execute/builtins.c execute/heredoc_2.c execute/execute.c utils/signal_2.c execute/extra.c execute/redir_2.c execute/pipe_2.c \
execute/heredoc_6.c execute/heredoc_7.c execute/cmd_1.c
OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

PRINTF_DIR = ft_printf
PRINTF = $(PRINTF_DIR)/libftprintf.a


READLINE_FLAGS = -lreadline -lncurses

all: $(NAME)

$(NAME): $(LIBFT) $(PRINTF) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft -L$(PRINTF_DIR) -lftprintf $(READLINE_FLAGS)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(PRINTF):
	make -C $(PRINTF_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) clean
	make -C $(PRINTF_DIR) clean
	rm -f $(OBJS)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	make -C $(PRINTF_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re