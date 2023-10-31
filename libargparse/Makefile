CC = gcc

NAME = libftargparse.a

CFLAGS = -Wall -Wextra -Werror

SRCS = 

INCLUDES = 

OBJS = ${SRCS:.c=.o}

RM  = rm -f 

all : $(NAME)

$(NAME) : $(OBJS)
	@ar rcs $(NAME) $(OBJS)

%.o : %.c $(INCLUDES)
	@$(CC) -c $< $(CFLAGS)

clean :
	@$(RM) $(OBJS)

fclean : clean
	@$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re