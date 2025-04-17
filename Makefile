SRCS= $(addprefix srcs/, main.c)
OBJS= $(SRCS:.c=.o)

NAME= ft_ping
FLAGS=
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
