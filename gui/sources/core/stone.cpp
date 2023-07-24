#include "core/stone.hpp"

std::ostream &operator<<(std::ostream &os, const Stone &stone)
{
    switch (stone)
    {
    case Empty:
        os << '.';
        break;
    case Black:
        os << 'x';
        break;
    case White:
        os << 'o';
        break;
    }
    return os;
}