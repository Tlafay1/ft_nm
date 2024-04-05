CC = gcc

NAME := ft_nm

CFLAGS := -Wall -Werror -Wextra -g

SRCS := 32_bits.c 64_bits.c errors.c ft_nm.c utils.c

OBJS := ${SRCS:.c=.o}

OBJDIR := $(addprefix obj/, $(OBJS))

INCLUDE := includes/ft_nm.h

INCDIR := $(addprefix includes/, $(INCLUDES))

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

obj/%.o : src/%.c $(INCDIR) Makefile
	mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@ -c -I./includes -I./libft -I./libargparse/include

clean :
	$(MAKE) -C ./libft $@
	$(RM) $(OBJDIR)

fclean : clean
	$(MAKE) -C ./libft $@
	$(RM) $(NAME) 

re : fclean all

.PHONY : all clean fclean re
.SILENT: