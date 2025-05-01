SRCS= $(addprefix srcs/, \
	main.c \
	utils/ip_utils.c \
	utils/exit_utils.c \
	utils/error.c \
	utils/math.c \
	utils/stats/time_utils.c \
	utils/stats/end_stats.c \
	utils/stats/ping_stats.c \
	utils/stats/stats_process.c \
	utils/stats/timeout_check.c \
	cli/cli_options.c \
	cli/init_options.c \
	cli/cli_options_parser.c \
	socket/socket_send.c \
	socket/socket_receive.c \
	socket/socket_routines.c \
	signals/sig_handler.c \
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
