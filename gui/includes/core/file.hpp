/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coscialp <coscialp@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:49:51 by coscialp          #+#    #+#             */
/*   Updated: 2023/06/29 13:54:14 by coscialp         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdint>
#include <exception>
#include <ostream>
#include <string>

struct File {
    uint8_t value;

    explicit constexpr File(uint8_t _value) : value(_value) {}

    inline static File from_str(std::string s) {
        if (s.length() != 1)
            throw FileTooLongError();
        return File(s[0] - 97);
    }

    friend std::ostream &operator<<(std::ostream &os, const File &f) {
        return os << ('a' + f.value);
    }

    class FileTooLongError : public std::exception {
     public:
        virtual const char *what() const throw() {
            return "Error: The file length must be equal to 1";
        }
    };

    constexpr File operator+(uint8_t value) {
        return File(this->value + value);
    }

    constexpr File operator-(uint8_t value) {
        return File(this->value - value);
    }

    constexpr File &operator+=(uint8_t value) {
        this->value += value;
        return *this;
    }

    constexpr File &operator-=(uint8_t value) {
        this->value -= value;
        return *this;
    }

    constexpr File operator++(int) {
        File tmp(*this);
        operator++();
        return tmp;
    }

    constexpr File &operator++() {
        value++;
        return *this;
    }

    constexpr File operator--(int) {
        File tmp(*this);
        operator--();
        return tmp;
    }

    constexpr File &operator--() {
        value--;
        return *this;
    }

    constexpr bool operator==(const File &rhs) const {
        return value == rhs.value;
    }
    constexpr bool operator!=(const File &rhs) const {
        return value != rhs.value;
    }
    constexpr bool operator<(const File &rhs) const {
        return value < rhs.value;
    }
    constexpr bool operator>(const File &rhs) const {
        return value > rhs.value;
    }
    constexpr bool operator<=(const File &rhs) const {
        return value <= rhs.value;
    }
    constexpr bool operator>=(const File &rhs) const {
        return value >= rhs.value;
    }
};