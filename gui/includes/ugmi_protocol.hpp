/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ugmi_protocol.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coscialp <coscialp@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 14:52:16 by coscialp          #+#    #+#             */
/*   Updated: 2023/06/30 16:32:13 by coscialp         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <array>
#include <iostream>
#include <map>
#include <vector>

#include <QProcess>

/// @brief The specification is independent of the operating system. For
/// Windows, the engine is a normal exe file, either a console or "real" Windows
/// application.
///
/// All communication is done via standard input and output with text commands.
///
/// The engine should boot and wait for input from the GUI. The engine should
/// wait for the `isready` or `setoption` commands to set up its internal
/// parameters as the boot process should be as quick as possible.
///
/// The engine must always be able to process input from stdin, even while
/// thinking.
///
/// All command strings the engine receives must end with a newline, all command
///  strings the GUI receives must end with a newline. It can be either `\r\n`
///  (for example on Windows) or `\n`.
///
/// All time durations sent by the GUI or the engine are in milliseconds.
///
/// Arbitrary whitespace counts before and after tokens are allowed, limited to
/// spaces and tabulations.
class UgmiProtocol {
 private:
    QString _program;
    QStringList _arguments;
    QProcess _process = QProcess();

 public:
    UgmiProtocol(const QString &program, const QStringList &arguments = {})
        : _program(program), _arguments(arguments) {}
    ~UgmiProtocol() {}

    void start();
    std::string readLine();
    void write(const QByteArray &content);

    class StartException : public std::exception {
     public:
        virtual const char *what() const throw() {
            return ("Error: Cannot start engine");
        }
    };

    class ReadException : public std::exception {
     public:
        virtual const char *what() const throw() {
            return ("Error: Cannot read from engine");
        }
    };

    class WriteException : public std::exception {
     public:
        virtual const char *what() const throw() {
            return ("Error: Cannot write to engine");
        }
    };

    enum ContextAttribute {
        Move,
    };

    class Send {
     public:
        /// @brief Tells the engine to use the UGMI protocol. This will be sent
        /// once as the first command after program boot to tell the engine to
        /// switch to UGMI mode. After receiving the `ugmi` command, the engine
        /// must identify itself with the `id` command and send the `option`
        /// commands to tell the GUI which engine settings the engine supports,
        /// if any. After that the engine must send `ugmiok` to acknowledge the
        /// UGMI mode. If no `ugmiok` is sent within a certain time period, the
        /// engine task should be killed by the GUI.
        inline static const QByteArray ugmi() { return "ugmi\n"; }

        /// @brief Switches the debug mode of the engine on and off. In debug
        /// mode the engine should send additional information to the GUI, for
        /// example with the `info string` command, to help debugging (for
        /// example an engine could log the commands it received and how it
        /// interpreted them). The debug mode must be switched off by default by
        /// the engine. The `debug` command can be sent at any time, including
        /// when the engine is thinking.
        /// @param mode Activate debug if true and deactivate debug if false
        inline static const QByteArray debug(bool mode = true) {
            return mode ? "debug on\n" : "debug off\n";
        }

        /// @brief This command is used to synchronize the engine with the GUI.
        /// When the GUI has sent a command or multiple commands that can take
        /// some time to complete (for example initializing a new hashtable when
        /// the `Hash` option is set), this command can be used to wait for the
        /// engine to be ready again or to ping the engine to find out if it is
        /// still alive. This command must be sent at least once before the
        /// engine is asked to do any search and after the position information
        /// has been sent, to wait for the engine to finish initializing its
        /// internal board. This command must always be answered with `readyok`
        /// and can be sent at any time, including when the engine is thinking.
        /// This command should not stop the search.
        inline static const QByteArray isready() { return "isready\n"; }

        /// @brief This command is sent to the engine when the user wants to
        /// change the internal parameters of the engine. For the button type
        /// the `value` token is not needed. One command will be sent for each
        /// parameter. This command must be sent only when the engine is
        /// waiting. The name and value of the option are case-sensitive, and
        /// can both include spaces. The substrings `name` and `value` must not
        /// be used for the `option_name` and
        ///`option_value` fields to disambiguate parsing. This is also the case
        /// for the `option` commands sent by the engine.
        /// @param name The name of the option to set
        /// @param value The value of the option to set (optional). Default is
        /// ""
        inline static const QByteArray setoption(std::string name,
                                                 std::string value = "") {
            return value.empty()
                       ? ("setoption name " + name + "\n").c_str()
                       : ("setoption name " + name + " value " + value + "\n")
                             .c_str();
        }

        /// @brief This command is sent to the engine when the next requested
        /// search will be from a different game. (This can be a new game the
        /// engine should play/analyze, or the next position from a testsuite.)
        /// As the engine's reaction to `ugminewgame` can take some time the GUI
        /// must always send `isready` after `ugminewgame` to finish its
        /// operation.
        inline static const QByteArray ugminewgame() { return "ugminewgame\n"; }

        /// @brief This command asks the engine to set up the position described
        /// on its internal board and play the moves listed from the given
        /// position. If the game was played from an empty board, the string
        /// `startpos` will be sent in place of `board <boardstring>`.
        /// @param board The board to set up. Default is `startpos`
        /// @param moves The list of moves all previous moves of the game.
        static const QByteArray
        position(std::string board = "startpos",
                 std::vector<std::string> moves = std::vector<std::string>());

        /// @brief This command asks the engine to start calculating on the
        /// current position set up with the `position` command. There are a
        /// number of search parameters that can follow this command. If no
        /// search parameters affecting the search limits (that is, any
        /// parameter except for
        ///`searchmoves`) are specified the engine must run in infinite mode,
        /// which
        /// means it must not send a `bestmove` command until a `stop` command
        /// is issued.
        /// @param searchmoves `searchmoves <move1> [<move2...moveN>]` restricts
        /// the search to the given moves only. For example, after `position
        /// startpos` and `go moves h08 j10` the engine should only search
        /// infinitely these two moves in the initial position.
        /// @param options A map of options to set for the search. The
        /// authorized options are:
        /// - `[wtime <time>] [btime <time>] [winc <time>] [binc <time>]`
        ///     configures the remaining time and increment for white/black. If
        ///     this parameter is sent by the GUI, it must contain the remaining
        ///     time for the side to move.
        /// - `depth <x>` limits the search to `x` plies of depth.
        /// - `nodes <x>` limits the search to `x` nodes.
        /// - `mate <x>` asks the engine to search for a mate in `x` moves (that
        /// is
        /// `(x * 2 - 1)` plies).
        /// - `movetime <x>` limits the search to `x` milliseconds.
        static const QByteArray
        go(std::vector<std::string> searchmoves = std::vector<std::string>(),
           std::map<std::string, int> options = std::map<std::string, int>());

        /// @brief Asks the engine to stop calculating as soon as possible and
        /// return a bestmove.
        inline static const QByteArray stop() { return "stop\n"; }

        /// @brief Asks the engine to stop its execution as soon as possible.
        inline static const QByteArray quit() { return "quit\n"; }
    };

    class Receive {
     public:
        /* Make parsing with regex */

     private:
        /* Create map of regex */
    };
};
