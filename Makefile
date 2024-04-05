CC = gcc

NAME := ft_nm

CFLAGS := -Wall -Wextra -g

SRCS := 32_bits.c 64_bits.c errors.c utils.c ft_nm.c main.c

TESTS := main.cpp tests.cpp

TESTS := $(addprefix tests/, ${TESTS})

OBJS := $(addprefix obj/, ${SRCS:.c=.o})

INCLUDE := include/ft_nm.h

all : $(NAME)

libs:
	$(MAKE) -C ./libft
	$(MAKE) -C ./libargparse

$(NAME) : libs $(OBJS)
	echo "[Compiling $(NAME)]"
	$(CC) $(CFLAGS) \
		$(OBJS) \
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
	$(RM) $(OBJS)

fclean : clean
	$(MAKE) -C ./libft $@
	$(RM) $(NAME)

test: libs $(OBJS)
	clang++ $(CFLAGS) \
		$(TESTS) \
		$(filter-out obj/main.o, $(OBJS)) \
		-o tests/test \
		-Llibft \
		-Llibargparse/lib \
		-Wl,-R./libft \
		-I./include \
		-I./libft \
		-I./libargparse/include \
		-pthread \
		-lgtest \
		-largparse \
		-lft
	./tests/test
	$(RM) tests/test

re : fclean all

.PHONY : all clean fclean re
.SILENT: