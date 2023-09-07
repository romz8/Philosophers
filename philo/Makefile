# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rjobert <rjobert@student.42barcelo>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/31 20:47:02 by rjobert           #+#    #+#              #
#    Updated: 2023/08/31 20:47:05 by rjobert          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC = cc
FLAGS = --debug -Wall -Wextra -Werror -MMD -I/. -pthread #-fsanitize=thread -g 

SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)
DEPS = $(SRC:.c=.d)
NAME = philo

all: $(NAME)

-include $(DEPS)
$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)


%.o : %.c Makefile
	$(CC) $(FLAGS) -c $< -o $@ 

clean:
	rm -rf $(OBJS)

fclean: clean
	rm $(NAME) $(DEPS)

re: fclean all

.PHONY: re all clean fclean