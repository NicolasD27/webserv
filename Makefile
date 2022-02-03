# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clorin <clorin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/17 16:20:01 by clorin            #+#    #+#              #
#    Updated: 2022/01/20 14:51:47 by clorin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		:= webserv 	

CC		:= clang++

CFLAGS	+= -Wall -Wextra -Werror -std=c++98

SRCS_DIR = srcs/

OBJS_DIR = objs/

SRCS	= main.cpp Webserv.cpp Server.cpp Client.cpp Request.cpp Response.cpp file_utils.cpp \
		  ErrorPages.cpp MimesType.cpp StatusCode.cpp ParserConfig.cpp Location.cpp utility.cpp \
		  CGIHandler.cpp Base64.cpp

OBJS	= $(SRCS:.cpp=.o) 

OBJS	:= ${addprefix ${OBJS_DIR}, ${OBJS}}

${OBJS_DIR}%.o: ${SRCS_DIR}%.cpp
		mkdir -p ${@D}
		$(CC) $(FLAGS) -c $< -o $@

all:	${NAME}

${NAME}:	${OBJS}
			${CC} ${CFLAGS} -o ${NAME} ${OBJS}

clean:		
			@echo "Cleaning: ${OBJS}"
			@rm -rf ${OBJS_DIR}

fclean:		clean
			@rm -rf ${NAME}
			@echo "\033[0mdelete "${NAME}"... \033[32mok\033[0m"

re:			fclean all

.phony: all clean fclean re