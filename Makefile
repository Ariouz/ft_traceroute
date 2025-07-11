SRCS= $(addprefix srcs/, \
	main.c \
	utils/ip_utils.c \
	utils/exit_utils.c \
	utils/error.c \
	utils/time_utils.c \
	cli/cli_options.c \
	cli/init_options.c \
	cli/cli_options_parser.c \
	socket/socket_send.c \
	socket/socket_receive.c \
	signals/sig_handler.c \
	traceroute/tracer.c \
	traceroute/trace_printer.c \
	)
OBJS= $(SRCS:.c=.o)

NAME= ft_traceroute
FLAGS= -Wall -Werror -Wextra
INCLUDES= -Iincludes/

all: $(NAME)

$(NAME): $(OBJS)
	@gcc $(FLAGS) $(OBJS) -o $(NAME)

bonus: FLAGS += -DBONUS
bonus: $(NAME)

%.o: %.c
	@gcc -c $< -o $@ $(FLAGS) $(INCLUDES)

clean:
	@rm $(OBJS)

fclean: clean
	@rm $(NAME)

re: fclean all
