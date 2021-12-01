SRCS = ./srcs/main.cpp $(wildcard ./srcs/Request/*.cpp) $(wildcard ./srcs/Response/*.cpp) \
	$(wildcard ./srcs/Configs/*.cpp) $(wildcard ./srcs/Server/*.cpp) $(wildcard ./srcs/Cgi/*.cpp)
OBJS	= $(SRCS:.cpp=.o)
CFLAGS = -Iincludes/ -g
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
