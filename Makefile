# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: coscialp <coscialp@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/26 11:49:45 by coscialp          #+#    #+#              #
#    Updated: 2023/06/30 14:03:44 by coscialp         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

ENGINE_DIRECTORY = engine
BUILD_DIRECTORY = build
APP_DIRECTORY = gui

all:
	mkdir -p ${BUILD_DIRECTORY}
	cd ${ENGINE_DIRECTORY} && cargo build
	cd ${BUILD_DIRECTORY} && qmake ../${APP_DIRECTORY} && make

clean:
	cd ${ENGINE_DIRECTORY} && cargo clean
	cd ${BUILD_DIRECTORY} && make clean

test:
	cd ${ENGINE_DIRECTORY} && cargo test

distclean:
	cd ${ENGINE_DIRECTORY} && cargo clean
	cd ${BUILD_DIRECTORY} && make distclean

fclean: distclean
	rm -rf ${BUILD_DIRECTORY}

re: distclean all