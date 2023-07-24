/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitboard.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coscialp <coscialp@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 10:20:03 by coscialp          #+#    #+#             */
/*   Updated: 2023/07/04 16:35:18 by coscialp         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "core/bitboard.hpp"
#include "core/bitboard_constant.hpp"

BitBoard BitBoard::shift_up()
{
    return *this >> ROW_SIZE;
}
BitBoard BitBoard::shift_down()
{
    return (*this & ~BitBoardConstant::BOTTOM_EDGE) << ROW_SIZE;
}
BitBoard BitBoard::shift_left()
{
    return (*this & ~BitBoardConstant::LEFT_EDGE) >> 1;
}
BitBoard BitBoard::shift_right()
{
    return (*this & ~BitBoardConstant::RIGHT_EDGE) << 1;
}
BitBoard &BitBoard::operator&=(const BitBoard &rhs)
{
    for (int i = 0; i < 6; i++)
        this->_board[i] &= rhs._board[i];
    return *this;
}

BitBoard &BitBoard::operator|=(const BitBoard &rhs)
{
    for (int i = 0; i < 6; i++)
        this->_board[i] |= rhs._board[i];
    return *this;
}

BitBoard &BitBoard::operator^=(const BitBoard &rhs)
{
    for (int i = 0; i < 6; i++)
        this->_board[i] ^= rhs._board[i];
    return *this;
}

BitBoard &BitBoard::operator<<=(uint8_t value)
{
    uint64_t carry = 0;

    for (int i = 0; i < 6; i++)
    {
        uint64_t new_carry = this->_board[i] >> (64 - value);
        this->_board[i] = (this->_board[i] << value) | carry;
        carry = new_carry;
    }
    return *this;
}

BitBoard &BitBoard::operator>>=(uint8_t value)
{
    uint64_t carry = 0;

    for (int i = 5; i >= 0; i--)
    {
        uint64_t new_carry = this->_board[i] << (64 - value);
        this->_board[i] = (this->_board[i] >> value) | carry;
        carry = new_carry;
    }
    return *this;
}

BitBoard BitBoard::operator&(const BitBoard &rhs) const
{
    BitBoard tmp(*this);
    tmp &= rhs;
    return tmp;
}

BitBoard BitBoard::operator|(const BitBoard &rhs) const
{
    BitBoard tmp(*this);
    tmp |= rhs;
    return tmp;
}

BitBoard BitBoard::operator^(const BitBoard &rhs) const
{
    BitBoard tmp(*this);
    tmp ^= rhs;
    return tmp;
}

BitBoard BitBoard::operator~() const
{
    BitBoard tmp(*this);

    for (int i = 0; i < 6; i++)
        tmp._board[i] = ~tmp._board[i];
    return tmp;
}

BitBoard BitBoard::operator<<(uint8_t value) const
{
    BitBoard tmp(*this);
    tmp <<= value;
    return tmp;
}

BitBoard BitBoard::operator>>(uint8_t value) const
{
    BitBoard tmp(*this);
    tmp >>= value;
    return tmp;
}
