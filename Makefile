# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/24 12:09:35 by ngardzie          #+#    #+#              #
#    Updated: 2022/01/24 12:09:40 by ngardzie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pong
SDL_TAR = SDL2-2.0.22.tar.gz


SRC_DIR = ./src/
INC_DIR = ./includes/

SDL_DIR = ./SDL2-2.0.22/
D_SDLINC = $(SDL_DIR)include
SDLLIB_DIR = $(SDL_DIR)lib
OBJ_DIR = ./obj/

L_SDL = `$(SDL_DIR)sdl2-config --cflags --libs`

HEADER_FILES = $(INC_DIR)pong.h 


C_FILES =	main.c

SRC = $(addprefix $(SRC_DIR), $(C_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(C_FILES:.c=.o))

CC = gcc
CFLAGS = -Werror
INC = -I $(INC_DIR) -I $(D_SDLINC)

C_RED = \033[31m
C_GREEN = \033[32m
C_NONE = \033[0m

all: $(NAME)

$(NAME): $(SDL_DIR) $(SDLLIB_DIR) $(OBJ_DIR) $(OBJ)
	@$(CC) $(OBJ) $(L_SDL) -o $(NAME)
	@printf "pong:  %-25s$(C_GREEN)[done]$(C_NONE)\n" $@

$(SDL_DIR):
	@tar -xf $(SDL_TAR)
	@printf "pong:  %-25s$(C_GREEN)[extracted]$(C_NONE)\n" $@

$(SDLLIB_DIR):
	@mkdir $(SDLLIB_DIR)
	@printf "\n$(C_GREEN)[configuring SDL]$(C_NONE)\n"
	@cd $(SDL_DIR); ./configure --prefix=`pwd`/lib
	@printf "$(C_GREEN)[compiling SDL]$(C_NONE)\n"
	@make -C $(SDL_DIR)
	@make -C $(SDL_DIR) install >/dev/null
	@printf "pong:  %-25s$(C_GREEN)[done]$(C_NONE)\n" $@

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)
	@printf "pong:  %-25s$(C_GREEN)[done]$(C_NONE)\n" $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER_FILES)
	@$(CC) $(CFLAGS) -c $(INC) $< -o $@
	@printf "pong:  %-25s$(C_GREEN)[done]$(C_NONE)\n" $@

clean:
	@rm -rf $(OBJ_DIR)
	@printf "pong:  %-25s$(C_RED)[done]$(C_NONE)\n" $@

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(SDL_DIR)
	@printf "pong:  %-25s$(C_RED)[done]$(C_NONE)\n" $@

re: fclean all

