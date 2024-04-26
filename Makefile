CC = gcc

NAME := ft_nm

CFLAGS := -Wall -Wextra -Werror

SRCS := 32_bits.c 64_bits.c errors.c utils.c ft_nm.c main.c

TESTS := main.cpp tests.cpp

TESTS := $(addprefix tests/, ${TESTS})

OBJS := $(addprefix obj/, ${SRCS:.c=.o})

INCLUDE := include/ft_nm.h

LIBARGPARSE_VERSION = 2.0.0

LIBARGPARSE_URL = https://github.com/Tlafay1/libargparse/releases/download/v$(LIBARGPARSE_VERSION)/libargparse-$(LIBARGPARSE_VERSION).tar.gz

LIBARGPARSE_NAME = libargparse-$(LIBARGPARSE_VERSION)

all : $(NAME)

libs: libft libargparse

libft:
	$(MAKE) -C ./libft

libargparse: $(LIBARGPARSE_NAME) $(LIBARGPARSE_NAME)/configure
	$(MAKE) -C ./$(LIBARGPARSE_NAME)

$(LIBARGPARSE_NAME)/configure:
	cd $(LIBARGPARSE_NAME) && ./configure

$(LIBARGPARSE_NAME):
	[ -d "./$(LIBARGPARSE_NAME)" ] || \
		(curl $(LIBARGPARSE_URL) -L -o $(LIBARGPARSE_NAME).tar.gz; \
		tar -xf $(LIBARGPARSE_NAME).tar.gz; \
		$(RM) $(LIBARGPARSE_NAME).tar.gz)

$(NAME) : libs $(OBJS)
	echo "[Compiling $(NAME)]"
	$(CC) $(CFLAGS) \
		$(OBJS) \
		-o $(NAME) \
		-Llibft \
		-L $(LIBARGPARSE_NAME)/lib \
		-lft \
		-largparse \
		-Wl,-R./libft
	echo "[Done]"

obj/%.o : src/%.c  $(INCLUDE) Makefile
	mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@ -c -I./include -I./libft -I./$(LIBARGPARSE_NAME)/include

clean :
	$(MAKE) -C ./libft $@
	$(MAKE) -C ./$(LIBARGPARSE_NAME) clean
	$(RM) $(OBJS)

fclean : clean
	$(MAKE) -C ./libft $@
	$(RM) $(NAME)

distclean: fclean
	$(RM) -r $(LIBARGPARSE_NAME)
	$(RM) config.log config.status

test: tests/test

tests/test: libs $(OBJS)
	g++ $(TESTS) \
		$(filter-out obj/main.o, $(OBJS)) \
		-o tests/test \
		-Llibft \
		-L $(LIBARGPARSE_NAME)/lib \
		-Wl,-R./libft \
		-I./include \
		-I./libft \
		-I./$(LIBARGPARSE_NAME)/include \
		-pthread \
		-lgtest \
		-largparse \
		-lft
	./tests/test
	$(RM) tests/test

re : fclean all

.PHONY : all \
	re \
	libs \
	libft \
	clean \
	fclean \
	libargparse \
	$(LIBARGPARSE_NAME)/configure

