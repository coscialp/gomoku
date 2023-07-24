/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coscialp <coscialp@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 11:50:01 by coscialp          #+#    #+#             */
/*   Updated: 2023/06/30 15:39:23 by coscialp         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mainwindow.h"
#include "utils/logger.hpp"
#include "core/square.hpp"
#include "core/bitboard_constant.hpp"
#include "core/bitboard.hpp"
#include "core/board.hpp"

#include <QApplication>

int main(int ac, char **av)
{
    QApplication a(ac, av);
    // MainWindow w;

    Logger<std::string>::info("Starting GUI");
    Logger<std::string>::debug("Connecting to server");
    Logger<std::string>::warning("Server is not responding");
    Logger<std::string, std::cerr>::error("Server is not responding");
    Logger<std::string, std::cerr>::fatal("Server is not responding");

    Logger<BitBoard>::info(BitBoardConstant::LEFT_EDGE, true);
    Logger<BitBoard>::info(BitBoardConstant::RIGHT_EDGE, true);
    Logger<BitBoard>::info(BitBoardConstant::BOTTOM_EDGE, true);

    // w.show();
    return a.exec();
}
