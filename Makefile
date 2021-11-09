SRCS = ./srcs/main.cpp
OBJS	= $(SRCS:.cpp=.o)
CFLAGS = -Iincludes/ -g -MMD
CC = clang++
NAME = webserver

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm $(NAME)

re: fclean all
