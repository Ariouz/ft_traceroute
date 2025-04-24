SRCS= $(addprefix srcs/, \
	main.c \
	utils/ip_utils.c \
	utils/exit_utils.c \
	utils/error.c \
	cli/cli_options.c \
	cli/init_options.c \
	cli/cli_options_parser.c \
	)
OBJS= $(SRCS:.c=.o)

NAME= ft_ping
FLAGS= -Wall -Werror -Wextra
INCLUDES= -Iincludes/

all: $(NAME)

$(NAME): $(OBJS)
	@gcc $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	@gcc -c $< -o $@ $(FLAGS) $(INCLUDES)

clean:
	@rm $(OBJS)

fclean: clean
	@rm $(NAME)

re: fclean all
