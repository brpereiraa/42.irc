CXX = c++
NAME = ircserv
CFLAGS = -Wall -Wextra -Werror -std=c++98 -I./includes
RM = rm -rf

SRCS = main.cpp src/Client.cpp src/Server.cpp src/server/Server.cpp src/Parser.cpp src/Channel.cpp src/cmds/Join.cpp\
	src/cmds/Kick.cpp src/cmds/Mode.cpp src/cmds/Invite.cpp src/cmds/Topic.cpp src/cmds/Commands.cpp \
	src/cmds/Pong.cpp src/cmds/User.cpp src/cmds/Pass.cpp src/cmds/Nick.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)
	clear

$(NAME) : $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

run: all clean
	./$(NAME)

.PHONY: all clean fclean re run