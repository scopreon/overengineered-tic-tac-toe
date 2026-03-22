#ifndef MY_BOARD
#define MY_BOARD

#include <bitset>
#include <cstdint>
#include <pybind11/pybind11.h>
#include <string>
namespace py = pybind11;

enum class Piece {
    e_O,
    e_X
};

enum class GameState {
    e_InPlay,
    e_OWon,
    e_XWon,
    e_Draw
};

inline std::string to_string(Piece p) {
    switch (p) {
    case Piece::e_O:
        return "O";
    case Piece::e_X:
        return "X";
    }
}

struct Location {
    int row;
    int col;
};

inline void check_location(const Location &location) {
    if (location.col < 0 || location.col > 2 || location.row < 0 ||
        location.row > 2) {
        throw std::out_of_range("Invalid board location");
    }
}

typedef std::array<std::array<std::optional<Piece>, 3>, 3> BoardType;

class Board {
  private:
    uint16_t d_bitset_x{};
    uint16_t d_bitset_o{};

  public:
    Board() {};
    template <Piece P>
    void move(const Location &location);
    GameState state() const;
    BoardType board() const;
    void clear(const Location &location);
};

template <Piece P>
inline void Board::move(const Location &location) {
    check_location(location);
    const int bit = 1 << (location.col + location.row * 3);

    if constexpr (P == Piece::e_O) {
        d_bitset_o |= bit;
    } else if constexpr (P == Piece::e_X) {
        d_bitset_x |= bit;
    }
}

inline void Board::clear(const Location &location) {
    check_location(location);
    const int bit = 1 << (location.col + location.row * 3);
    d_bitset_o &= ~bit;
    d_bitset_x &= ~bit;
}

#endif
