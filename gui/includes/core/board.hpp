/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coscialp <coscialp@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:54:59 by coscialp          #+#    #+#             */
/*   Updated: 2023/06/30 13:19:36 by coscialp         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core/bitboard.hpp"
#include "core/square.hpp"
#include "core/stone.hpp"

#include <array>
#include <ostream>

class Board {
 private:
    std::array<BitBoard, 2> _bitsets;
    std::array<Stone, BOARD_SIZE> _table;
    Stone _turn;

 public:
    Board() {
        _bitsets[0] = BitBoard();
        _bitsets[1] = BitBoard();
        _table.fill(Stone::Empty);
        _turn = Stone::Black;
    }

    static Board from_raw_parts(BitBoard black, BitBoard white, Stone turn) {
        Board b;
        b._bitsets[0] = black;
        b._bitsets[1] = white;
        b._turn = turn;

        for (Square s : black)
            b._table[s.value] = Stone::Black;

        for (Square s : white)
            b._table[s.value] = Stone::White;

        return b;
    }

    BitBoard bitboard(Stone side) {
        if (side == Stone::Empty)
            return ~(_bitsets[Stone::Black] | _bitsets[Stone::White]);
        return _bitsets[side];
    }

    void push(Square sq) {
        _table[sq.value] = _turn;
        _bitsets[_turn].set_square(sq);
        _turn = flip(_turn);
    }

    void pop(Square sq) {
        _turn = flip(_turn);
        _bitsets[_turn].rst_square(sq);
        _table[sq.value] = Stone::Empty;
    }

    friend std::ostream &operator<<(std::ostream &os, const Board &b) {
        os << "black:\n"
           << b._bitsets[0] << "\nwhite:\n"
           << b._bitsets[1] << "\nturn: " << b._turn;
        return os;
    }
};
