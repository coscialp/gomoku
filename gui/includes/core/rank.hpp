/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rank.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coscialp <coscialp@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:50:51 by coscialp          #+#    #+#             */
/*   Updated: 2023/06/29 13:54:08 by coscialp         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdint>
#include <exception>
#include <ostream>
#include <string>

struct Rank {
    uint8_t value;

    explicit constexpr Rank(uint8_t _value) : value(_value) {}

    inline static Rank from_str(std::string s) {
        if (s.length() != 2)
            throw RankTooLongError();

        return Rank(std::stoi(s));
    }

    friend std::ostream &operator<<(std::ostream &os, const Rank &r) {
        return os << ('0' + (r.value + 1) / 10) << ('0' + (r.value + 1) % 10);
    }

    class RankTooLongError : public std::exception {
     public:
        virtual const char *what() const throw() {
            return "Error: Rank length must be equal to 2";
        }
    };

    constexpr Rank operator+(uint8_t value) {
        return Rank(this->value + value);
    }

    constexpr Rank operator-(uint8_t value) {
        return Rank(this->value - value);
    }

    constexpr Rank &operator+=(uint8_t value) {
        this->value += value;
        return *this;
    }

    constexpr Rank &operator-=(uint8_t value) {
        this->value -= value;
        return *this;
    }

    constexpr Rank operator++(int) {
        Rank tmp(*this);
        operator++();
        return tmp;
    }

    constexpr Rank &operator++() {
        value++;
        return *this;
    }

    constexpr Rank operator--(int) {
        Rank tmp(*this);
        operator--();
        return tmp;
    }

    constexpr Rank &operator--() {
        value--;
        return *this;
    }

    constexpr bool operator==(const Rank &rhs) const {
        return value == rhs.value;
    }
    constexpr bool operator!=(const Rank &rhs) const {
        return value != rhs.value;
    }
    constexpr bool operator<(const Rank &rhs) const {
        return value < rhs.value;
    }
    constexpr bool operator>(const Rank &rhs) const {
        return value > rhs.value;
    }
    constexpr bool operator<=(const Rank &rhs) const {
        return value <= rhs.value;
    }
    constexpr bool operator>=(const Rank &rhs) const {
        return value >= rhs.value;
    }
};
