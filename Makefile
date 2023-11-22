CC = gcc

NAME := ft_nm

CFLAGS := -Wall -Werror -Wextra -g

SRCS := 32_bits.c 64_bits.c errors.c ft_nm.c utils.c

OBJS := ${SRCS:.c=.o}

OBJDIR := $(addprefix obj/, $(OBJS))

INCLUDES := ft_nm.h

INCDIR := $(addprefix includes/, $(INCLUDES))

all : $(NAME)

$(NAME) : $(OBJDIR)
	echo "[Compiling libft]"
	$(MAKE) -C ./libft
	echo "[Compiling argparse]"
	$(MAKE) -C ./libargparse
	echo "[Compiling $(NAME)]"
	$(CC) $(OBJDIR) ./libargparse/libargparse.a -o $(NAME) $(CFLAGS) -Llibft -lft -Wl,-R./libft
	echo "[Done]"

obj/%.o : src/%.c $(INCDIR) Makefile
	mkdir -p obj
	$(CC) -c $< -I includes -I libft -I libargparse $(CFLAGS) -o $@

clean :
	$(MAKE) -C ./libft $@
	$(RM) $(OBJDIR)

fclean : clean
	$(MAKE) -C ./libft $@
	$(RM) $(NAME) 

re : fclean all

.PHONY : all clean fclean re
.SILENT:
