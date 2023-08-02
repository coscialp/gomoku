/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stone.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coscialp <coscialp@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 11:39:36 by coscialp          #+#    #+#             */
/*   Updated: 2023/06/30 13:14:14 by coscialp         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ostream>

/// @brief This enum is used to represent a stone on the board
/// @param Black is used to represent a black stone
/// @param White is used to represent a white stone
/// @param Empty is used to represent an empty square
enum Stone {
    Black,
    White,
    Empty,
};

inline Stone flip(Stone stone) {
    switch (stone) {
    case Stone::Black:
        return Stone::White;
    case Stone::White:
        return Stone::Black;
    default:
        return Stone::Empty;
    }
}

std::ostream &operator<<(std::ostream &os, const Stone &stone);
