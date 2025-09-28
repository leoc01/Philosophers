# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/24 17:31:50 by lbuscaro          #+#    #+#              #
#    Updated: 2025/09/24 12:37:05 by lbuscaro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -pthread -fsanitize=thread -Wall -Werror -Wextra
GDB= -g
RM = rm -rf

INCLUDES_DIR = ./includes
SRC_DIR = ./src
BUILD_DIR = ./build

M_MAIN = $(SRC_DIR)/main.c
M_MAIN_OBJ = $(BUILD_DIR)/main.o

M_SRC = args_check.c
M_SRC += init_philos.c
M_SRC := $(addprefix $(SRC_DIR)/,$(M_SRC))
M_OBJ = $(M_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

all: $(NAME)

$(NAME): $(LFT) $(M_OBJ) $(M_MAIN_OBJ)
	$(CC) $(GDB) $(CFLAGS) $(M_OBJ) $(M_MAIN_OBJ) -o $(NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c 
	@mkdir -p $(BUILD_DIR)
	$(CC) $(GDB) $(CFLAGS) -I$(INCLUDES_DIR) -c $< -o $@

clean:
	$(RM) $(BUILD_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all
