CC = gcc

NAME := ft_nm

CFLAGS := -Wall -Werror -Wextra

SRCS := ft_nm.c 32_bits.c 64_bits.c utils.c errors.c

OBJS := ${SRCS:.c=.o}

OBJDIR := $(addprefix obj/, $(OBJS))

INCLUDES := ft_nm.h

INCDIR := $(addprefix includes/, $(INCLUDES))

all : $(NAME)

$(NAME) : $(OBJDIR)
	echo "\x1b[34m $1[Compiling libft]\x1b[0m"
	$(MAKE) -C ./libft
	echo "\x1b[34m $1[Compiling $(NAME)]\x1b[0m"
	$(CC) $(OBJDIR) ./libft/libft.a -o $(NAME) $(CFLAGS)
	echo "\x1b[34m $1[Done]\x1b[0m"

obj/%.o : src/%.c $(INCDIR) Makefile
	mkdir -p obj
	$(CC) -c $< -I includes -I libft $(CFLAGS) -o $@

clean :
	$(MAKE) -C ./libft $@
	$(RM) $(OBJDIR)

fclean : clean
	$(MAKE) -C ./libft $@
	$(RM) $(NAME) 

re : fclean all

.PHONY : all clean fclean re
.SILENT:
