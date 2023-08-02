/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ugmi_protocol.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coscialp <coscialp@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:14:24 by coscialp          #+#    #+#             */
/*   Updated: 2023/06/28 16:50:03 by coscialp         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ugmi_protocol.hpp"

const QByteArray UgmiProtocol::Send::position(std::string board,
                                              std::vector<std::string> moves) {
    if (board == "startpos")
        return "position startpos\n";

    std::string command = "position board " + board;

    if (!moves.empty())
        command += " moves";

    for (std::string &move : moves)
        command += " " + move;

    command += "\n";
    return command.c_str();
}

const QByteArray UgmiProtocol::Send::go(std::vector<std::string> searchmoves,
                                        std::map<std::string, int> options) {
    std::string command = "go";

    if (!searchmoves.empty())
        command += " searchmoves";

    for (const std::string &searchmove : searchmoves)
        command += " " + searchmove;

    for (const auto &option : options) {
        // TODO(coscialp): Check if the option is valid and logger a warning if
        // option are not valid
        command += " " + option.first + " " + std::to_string(option.second);
    }

    command += "\n";
    return command.c_str();
}

void UgmiProtocol::start() {
    this->_process.start(this->_program, this->_arguments);
    if (!this->_process.waitForStarted())
        throw UgmiProtocol::StartException();
}
std::string UgmiProtocol::readLine() {
    if (!this->_process.waitForReadyRead() && !this->_process.canReadLine())
        throw UgmiProtocol::ReadException();
    return this->_process.readLine().toStdString();
}

void UgmiProtocol::write(const QByteArray &content) {
    if (!this->_process.write(content.toStdString().c_str()))
        throw UgmiProtocol::WriteException();
    this->_process.write(content);
}
