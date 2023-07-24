/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitboard.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coscialp <coscialp@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 09:40:41 by coscialp          #+#    #+#             */
/*   Updated: 2023/06/30 13:04:48 by coscialp         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core/square.hpp"
#include <cstdint>
#include <array>
#include <initializer_list>

class BitBoard
{
private:
    std::array<uint64_t, 6> _board = {0, 0, 0, 0, 0, 0};

public:
    class iterator
    {
    public:
        iterator(const BitBoard &bb, uint8_t index)
        {
            _bb = bb.raw_buffer();
            _index = index;
        }

        iterator &operator++()
        {
            for (uint8_t i = _index; i < 6; i++)
            {
                if (_bb[i])
                {
                    _index = i;
                    _bb[i] &= _bb[i] - 1;
                    return *this;
                }
            }
            return *this;
        }

        iterator operator++(int)
        {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        Square operator*() const
        {
            for (uint8_t i = _index; i < 6; i++)
                if (_bb[i])
                    return Square(i * 64 + __builtin_ctzll(_bb[i]));
            return SquareConstants::OUT_OF_BOARD;
        }

        bool operator==(const iterator &rhs) const { return **this == *rhs; }
        bool operator!=(const iterator &rhs) const { return **this != *rhs; }

    private:
        std::array<uint64_t, 6> _bb;
        uint8_t _index = 0;
    };

    constexpr BitBoard() {}
    constexpr BitBoard(std::array<uint64_t, 6> arr) : _board(arr) {}
    constexpr BitBoard(const BitBoard &bb) : _board(bb._board) {}

    BitBoard &operator=(const BitBoard &bb)
    {
        _board = bb._board;
        return *this;
    }

    inline std::array<uint64_t, 6> raw_buffer() const
    {
        return this->_board;
    }
    inline bool get_square(Square sq) const { return this->_board[sq.value / 64] >> (sq.value % 64) & 1; }
    inline void set_square(Square sq) { this->_board[sq.value / 64] |= uint64_t(1) << (sq.value % 64); }
    inline void rst_square(Square sq) { this->_board[sq.value / 64] &= ~(uint64_t(1) << (sq.value % 64)); }
    inline void swp_square(Square sq) { this->_board[sq.value / 64] ^= uint64_t(1) << (sq.value % 64); }

    friend std::ostream &operator<<(std::ostream &os, const BitBoard &bb)
    {
        for (Rank r = SquareConstants::FIRST_RANK; r <= SquareConstants::LAST_RANK; r++)
        {
            for (File f = SquareConstants::FIRST_FILE; f <= SquareConstants::LAST_FILE; f++)
            {
                Square sq = Square::from(f, r);
                os << (bb.get_square(sq) ? "x" : ".");
            }
            os << std::endl;
        }
        return os;
    }

    BitBoard shift_up();
    BitBoard shift_down();
    BitBoard shift_left();
    BitBoard shift_right();

    BitBoard &operator&=(const BitBoard &rhs);
    BitBoard &operator|=(const BitBoard &rhs);
    BitBoard &operator^=(const BitBoard &rhs);
    BitBoard &operator<<=(uint8_t value);
    BitBoard &operator>>=(uint8_t value);
    BitBoard operator&(const BitBoard &rhs) const;
    BitBoard operator|(const BitBoard &rhs) const;
    BitBoard operator^(const BitBoard &rhs) const;
    BitBoard operator~() const;
    BitBoard operator<<(uint8_t value) const;
    BitBoard operator>>(uint8_t value) const;

    BitBoard::iterator begin() const { return iterator(*this, 0); }
    BitBoard::iterator end() const { return iterator(*this, 6); }
};
