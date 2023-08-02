/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coscialp <coscialp@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 13:45:54 by coscialp          #+#    #+#             */
/*   Updated: 2023/06/30 15:10:17 by coscialp         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ctime>
#include <iostream>
#include <ostream>
#include <string>

namespace Color {
enum Code {
    FG_RED = 31,
    FG_GREEN = 32,
    FG_BLUE = 34,
    FG_GREY = 37,
    FG_DARK_RED = 91,
    FG_YELLOW = 93,
    FG_CYAN = 96,
    FG_DEFAULT = 39,
};

class Modifier {
    Code code;

 public:
    explicit Modifier(Code &pCode) : code(pCode) {}
    explicit constexpr Modifier(const Code &pCode) : code(pCode) {}

    friend std::ostream &operator<<(std::ostream &os, const Modifier &mod) {
        return os << "\033[" << mod.code << "m";
    }
};
}  // namespace Color

enum LoggerLevel { INFO, DEBUG, WARNING, ERROR, FATAL };

///  @brief Logger class to log message with a level
///  @tparam T Type of the message
///  @tparam out (std::ostream) Output stream. Can be std::cout or std::cerr.
///  Default is std::cout
template <class T, std::ostream &out = std::cout> class Logger {
 private:
    static constexpr Color::Modifier def = Color::Modifier(Color::FG_DEFAULT);
    static constexpr Color::Modifier red = Color::Modifier(Color::FG_RED);
    static constexpr Color::Modifier dark_red =
        Color::Modifier(Color::FG_DARK_RED);
    static constexpr Color::Modifier yellow = Color::Modifier(Color::FG_YELLOW);
    static constexpr Color::Modifier cyan = Color::Modifier(Color::FG_CYAN);
    static constexpr Color::Modifier green = Color::Modifier(Color::FG_GREEN);
    static constexpr Color::Modifier blue = Color::Modifier(Color::FG_BLUE);
    static constexpr Color::Modifier grey = Color::Modifier(Color::FG_GREY);

    Logger() {}
    ~Logger() {}

 public:
    ///  \brief Log a message with a level and the current time in UTC
    ///  \param level Level of the message can be INFO, DEBUG, WARNING, ERROR,
    ///  FATAL \param message Message to log
    static void log(LoggerLevel level, T message, bool multiline = false) {
        out << Logger<T, out>::get_utc_time() << " "
            << Logger<T, out>::level_to_color(level);

        out << Logger<T, out>::level_to_string(level) << " "
            << Logger<T, out>::def << "- ";

        if (multiline)
            out << std::endl;

        out << message << std::endl;
    }

    ///  \brief Log a message with the INFO level and the current time in UTC
    ///  \param message Message to log
    static void info(T message, bool multiline = false) {
        Logger<T, out>::log(LoggerLevel::INFO, message, multiline);
    }

    ///  \brief Log a message with the DEBUG level and the current time in UTC
    ///  \param message Message to log
    static void debug(T message, bool multiline = false) {
        Logger<T, out>::log(LoggerLevel::DEBUG, message, multiline);
    }

    ///  \brief Log a message with the WARNING level and the current time in UTC
    ///  \param message Message to log
    static void warning(T message, bool multiline = false) {
        Logger<T, out>::log(LoggerLevel::WARNING, message, multiline);
    }

    ///  \brief Log a message with the ERROR level and the current time in UTC
    ///  \param message Message to log
    static void error(T message, bool multiline = false) {
        Logger<T, out>::log(LoggerLevel::ERROR, message, multiline);
    }

    ///  \brief Log a message with the FATAL level and the current time in UTC
    ///  \param message Message to log
    static void fatal(T message, bool multiline = false) {
        Logger<T, out>::log(LoggerLevel::FATAL, message, multiline);
    }

 private:
    ///  \brief Get the current time in UTC
    ///  \return std::string Current time in UTC
    static std::string get_utc_time() {
        std::time_t now = std::time(0);
        std::tm *now_tm = std::gmtime(&now);
        char buf[42];

        std::strftime(buf, 42, "[%Y-%m-%d %X GMT+00]", now_tm);
        return std::string(buf);
    }

    static std::string level_to_string(LoggerLevel level) {
        switch (level) {
        case LoggerLevel::INFO:
            return "INFO";
        case LoggerLevel::DEBUG:
            return "DEBUG";
        case LoggerLevel::WARNING:
            return "WARNING";
        case LoggerLevel::ERROR:
            return "ERROR";
        case LoggerLevel::FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
        }
    }

    static Color::Modifier level_to_color(LoggerLevel level) {
        switch (level) {
        case LoggerLevel::INFO:
            return grey;
        case LoggerLevel::DEBUG:
            return cyan;
        case LoggerLevel::WARNING:
            return yellow;
        case LoggerLevel::ERROR:
            return red;
        case LoggerLevel::FATAL:
            return dark_red;
        default:
            return def;
        }
    }
};
