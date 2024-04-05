CC = gcc

NAME := ft_nm

CFLAGS := -Wall -Wextra -g

SRCS := 32_bits.c 64_bits.c errors.c ft_nm.c utils.c

OBJS := ${SRCS:.c=.o}

OBJDIR := $(addprefix obj/, $(OBJS))

INCLUDE := include/ft_nm.h

all : $(NAME)

libs:
	$(MAKE) -C ./libft
	$(MAKE) -C ./libargparse

$(NAME) : libs $(OBJDIR)
	echo "[Compiling $(NAME)]"
	$(CC) $(CFLAGS) \
		$(OBJDIR) \
		-o $(NAME) \
		-Llibft \
		-L libargparse/lib \
		-lft \
		-largparse \
		-Wl,-R./libft
	echo "[Done]"

obj/%.o : src/%.c  $(INCLUDE) Makefile
	mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@ -c -I./include -I./libft -I./libargparse/include

clean :
	$(MAKE) -C ./libft $@
	$(RM) $(OBJDIR)

fclean : clean
	$(MAKE) -C ./libft $@
	$(RM) $(NAME) 

re : fclean all

.PHONY : all clean fclean re
.SILENT: