# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    gui.pro                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: coscialp <coscialp@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/26 11:50:32 by coscialp          #+#    #+#              #
#    Updated: 2023/06/30 12:56:47 by coscialp         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    sources/mainwindow.cpp \
    sources/ugmi_protocol.cpp \
    sources/core/bitboard.cpp \
    sources/core/stone.cpp

INCLUDEPATH += includes

HEADERS += \
    includes/utils/logger.hpp \
    includes/ugmi_protocol.hpp \
    includes/core/bitboard_constant.hpp \
    includes/core/bitboard.hpp \
    includes/core/direction.hpp \
    includes/core/stone.hpp \
    includes/core/file.hpp \
    includes/core/rank.hpp \
    includes/core/square.hpp \
    includes/core/board.hpp

FORMS += \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
