/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   square.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coscialp <coscialp@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 11:51:29 by coscialp          #+#    #+#             */
/*   Updated: 2023/07/04 16:38:06 by coscialp         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core/direction.hpp"
#include "core/file.hpp"
#include "core/rank.hpp"

#include <cstdint>
#include <ostream>

constexpr uint16_t ROW_SIZE = 19;
constexpr uint16_t BOARD_SIZE = ROW_SIZE * ROW_SIZE;

struct Square {
    uint16_t value;

    explicit constexpr Square(uint16_t _value) : value(_value) {}

    static constexpr Square from(File file, Rank rank) {
        return Square(file.value + uint16_t(rank.value) * ROW_SIZE);
    }

    File file() const { return File(this->value % ROW_SIZE); }
    Rank rank() const { return Rank(this->value / ROW_SIZE); }

    constexpr Square shift(Direction direction) const {
        switch (direction) {
        case Direction::North:
            return Square(this->value - ROW_SIZE);
        case Direction::NorthEast:
            return Square(this->value - ROW_SIZE + 1);
        case Direction::East:
            return Square(this->value + 1);
        case Direction::SouthEast:
            return Square(this->value + ROW_SIZE + 1);
        case Direction::South:
            return Square(this->value + ROW_SIZE);
        case Direction::SouthWest:
            return Square(this->value + ROW_SIZE - 1);
        case Direction::West:
            return Square(this->value - 1);
        case Direction::NorthWest:
            return Square(this->value - ROW_SIZE - 1);
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const Square &s) {
        return os << s.file() << s.rank();
    }

    constexpr Square operator+(uint16_t value) {
        return Square(this->value + value);
    }
    constexpr Square operator-(uint16_t value) {
        return Square(this->value - value);
    }
    constexpr Square &operator+=(uint16_t value) {
        this->value += value;
        return *this;
    }
    constexpr Square &operator-=(uint16_t value) {
        this->value -= value;
        return *this;
    }

    constexpr bool operator==(const Square &other) const {
        return this->value == other.value;
    }
    constexpr bool operator!=(const Square &other) const {
        return this->value != other.value;
    }
};

namespace SquareConstants {
constexpr File FIRST_FILE = File(0);
constexpr File LAST_FILE = File(18);
constexpr Rank FIRST_RANK = Rank(0);
constexpr Rank LAST_RANK = Rank(18);
constexpr Square OUT_OF_BOARD = Square(BOARD_SIZE);
}  // namespace SquareConstants
