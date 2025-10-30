# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/25 20:47:34 by ddecilap          #+#    #+#              #
#    Updated: 2024/11/25 20:52:24 by ddecilap         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re bonus

NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(HEADER)
HEADER = .

BONUS = ft_bonus.lstadd_back.c ft_bonus.lstadd_front.c ft_bonus.lstclear.c \
        ft_bonus.lstdelone.c ft_bonus.lstiter.c ft_bonus.lstlast.c \
        ft_bonus.lstmap.c ft_bonus.lstnew.c ft_bonus.lstsize.c

SRCS = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c \
ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_strncmp.c ft_isnumber.c ft_free_arr.c

OBJ = $(SRCS:.c=.o)
OBJB = $(BONUS:.c=.o)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

all: $(NAME)

bonus: $(OBJB) $(NAME)
	ar rcs -o $(NAME) $(OBJB)

clean:
	rm -f $(OBJ) $(OBJB)

fclean: clean
	rm -f $(NAME)

re: fclean all
