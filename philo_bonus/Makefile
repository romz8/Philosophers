# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rjobert <rjobert@student.42barcelo>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/31 20:47:02 by rjobert           #+#    #+#              #
#    Updated: 2023/09/13 21:30:59 by rjobert          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC = cc
FLAGS = -Wall -Wextra -Werror -MMD -I/. -pthread 

SRC =  philo_bonus.c utils_bonus.c simulation_bonus.c philosophers_bonus.c \
parsing_bonus.c sem_philo.c utils_bonus2.c

OBJS = $(SRC:.c=.o)
DEPS = $(SRC:.c=.d)
NAME = philo_bonus

all: $(NAME)

-include $(DEPS)
$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o : %.c Makefile
	$(CC) $(FLAGS) -c $< -o $@ 

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME) $(DEPS)

re: fclean all

.PHONY: re all clean fclean
