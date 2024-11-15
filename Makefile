# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ralves-g <ralves-g@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/26 16:26:20 by ralves-g          #+#    #+#              #
#    Updated: 2023/10/11 18:58:03 by ralves-g         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror -g -fsanitize=address
MLX_LINUX	=	-Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz
RM			=	rm -f

NAME		=	flappy

INCLUDE		=	-I ./
SRCS_		=	\
				flappy.c img_utils.c key.c

SRCS		=	$(addprefix $(_SRC), $(SRCS_))

_OBJ		=	./obj/
_SRC		= 	./src/
OBJS		=	$(patsubst $(_SRC)%.c, $(_OBJ)%.o, $(SRCS))

all:		$(NAME)

$(_OBJ)%.o: $(_SRC)%.c
	$(CC) -c $< -o $@

$(NAME): $(_OBJ) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_LINUX) -o $(NAME) $(INCLUDE)

$(_OBJ):
	mkdir $@

clean:
	$(RM) -r $(_OBJ)

fclean:	clean
	$(RM) $(NAME) $(NAME2)

re:	fclean all
