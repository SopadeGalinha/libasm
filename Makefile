NAME		= libasm.a
SHARED		= libasm.so
NASM		= nasm
NASMFLAGS	= -f elf64
AR		= ar rcs
RM		= rm -f

# Assembly sources (mandatory) and objects
SRCS		= ft_strlen.s ft_strcpy.s ft_strcmp.s ft_write.s ft_read.s ft_strdup.s
OBJS		= $(SRCS:.s=.o)

# Available mandatory sources/objects (for partial test runs)
SRCS_AVAIL	= $(wildcard $(SRCS))
OBJS_AVAIL	= $(SRCS_AVAIL:.s=.o)

# Bonus sources and objects (add files when implemented)
SRCS_BONUS	= ft_atoi_base_bonus.s ft_list_push_front_bonus.s ft_list_size_bonus.s ft_list_sort_bonus.s ft_list_remove_if_bonus.s
OBJS_BONUS	= $(SRCS_BONUS:.s=.o)

# Available bonus
SRCS_BONUS_AVAIL = $(wildcard $(SRCS_BONUS))
OBJS_BONUS_AVAIL = $(SRCS_BONUS_AVAIL:.s=.o)

# Tests
TEST_DIR	= tests
TEST_BIN	= $(TEST_DIR)/run_tests
TEST_SRCS	= $(wildcard $(TEST_DIR)/ft_*_test.c) $(TEST_DIR)/main.c
TEST_OBJS	= $(TEST_SRCS:.c=.o)
CC		= gcc
CFLAGS		= -Wall -Wextra -Werror -g
LDFLAGS	= -L. -lasm -ldl

# Colors
CLR_RESET	= \033[0m
CLR_RED	= \033[0;31m
CLR_GREEN	= \033[0;32m
CLR_YEL	= \033[0;33m

.PHONY: all clean fclean re test bonus check shared help

all: $(NAME)
	@echo "$(CLR_GREEN)[OK]$(CLR_RESET) Built $(NAME)"

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

$(SHARED): $(OBJS_AVAIL) $(OBJS_BONUS_AVAIL)
	$(CC) -shared -o $(SHARED) $(OBJS_AVAIL) $(OBJS_BONUS_AVAIL)

%.o: %.s
	$(NASM) $(NASMFLAGS) $< -o $@

test: $(SHARED) $(TEST_BIN)
	@echo "$(CLR_YEL)[*]$(CLR_RESET) Running tests"
	@LD_LIBRARY_PATH=. $(TEST_BIN) || true

$(TEST_BIN): $(TEST_OBJS) $(SHARED)
	$(CC) $(CFLAGS) -I. $(TEST_OBJS) $(LDFLAGS) -o $@

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

bonus: $(OBJS) $(OBJS_BONUS)
	$(AR) $(NAME) $(OBJS) $(OBJS_BONUS)

clean:
	$(RM) $(OBJS)
	$(RM) $(TEST_OBJS)
	$(RM) $(OBJS_BONUS)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(SHARED)
	$(RM) $(TEST_BIN)

re: fclean all

# Optional: warn if mandatory sources are missing
check:
	@missing=0; \
	for f in $(SRCS); do \
		[ -f $$f ] || { echo "Missing $$f"; missing=1; }; \
	done; exit $$missing

help:
	@echo "libasm Makefile targets:";
	@echo "  make / make all      - build $(NAME) (mandatory only)";
	@echo "  make bonus           - build $(NAME) with bonus objects";
	@echo "  make shared          - build $(SHARED) (with bonus if present)";
	@echo "  make test            - build shared + tests and run them";
	@echo "  make clean           - remove object files (mand/bonus/tests)";
	@echo "  make fclean          - clean + remove $(NAME), $(SHARED), test bin";
	@echo "  make re              - fclean then all";
	@echo "  make check           - report missing mandatory .s files";
