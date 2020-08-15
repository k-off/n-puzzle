# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: pacovali <pacovali@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/08/06 18:29:29 by pacovali      #+#    #+#                  #
#    Updated: 2020/08/09 15:49:04 by pacovali      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = npuzzle

CXX = @clang++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -O3
SRC = main.cpp Board.class.cpp Npuzzle.class.cpp
OBJS = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $@ $^

clean:
	@$(RM) *.o .DS_Store

fclean: clean
	@$(RM) $(NAME) ../.DS_Store

re: fclean all