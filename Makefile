ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so

L_NAME = libft_malloc.so

CC = gcc # -g -fsanitize=address,undefined,integer

CCFLAGS = -fPIC -fvisibility=hidden

SRC_PATH = ./src

LIBFT_PATH = ./libft

LIBFT = $(LIBFT_PATH)/libft.a

OBJ_PATH = ./obj

LIBFT_INCLUDE = $(LIBFT_PATH)

SRC_INCLUDE = $(SRC_PATH)

INC = $(addprefix -I, $(LIBFT_INCLUDE) $(SRC_INCLUDE))

SRC = $(addprefix $(SRC_PATH)/, malloc.c free.c realloc.c init.c show_alloc_mem.c calloc.c dbg_env_handler.c reallocf.c)

INCLUDE_FILES = $(wildcard $(SRC_INCLUDE)/include/*.h $(LIBFT_INCLUDE)/*.h)

OBJ = $(SRC:$(SRC_PATH)/%.c=$(OBJ_PATH)/%.o)

all : creat_obj_dir $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) -shared -o $@ $(OBJ) $(LIBFT)
	@ln -sf $@ $(L_NAME)

$(LIBFT):
	@make -C $(LIBFT_PATH)

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c $(INCLUDE_FILES)
	$(CC) $(CCFLAGS) $(INC) -c $< -o $@


creat_obj_dir:
	@if [ ! -d $(OBJ_PATH) ]; then mkdir $(OBJ_PATH); fi

clean:
	@rm -rf $(OBJ)
	@make clean -C $(LIBFT_PATH)

fclean: clean
	@rm -f $(NAME)
	@rm -f $(L_NAME)
	@make fclean -C $(LIBFT_PATH)

re : fclean all

.PHONY: all clean fclean re
