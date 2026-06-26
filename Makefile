# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/26 09:10:30 by kkoujan           #+#    #+#              #
#    Updated: 2026/06/26 11:12:40 by kkoujan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_irc
OBJ = ./server/Client.o ./server/Server.o main.o
HEADERS = ./server/Client.hpp ./server/Server.hpp
FLAGS = -Wall -Wextra -Werror -std=c++98
CC = c++


all : $(NAME)
	
$(NAME) : $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.c $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all
