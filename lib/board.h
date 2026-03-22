#ifndef MY_BOARD
#define MY_BOARD

#include <bitset>
#include <cstdint>
#include <optional>
#include <pybind11/pybind11.h>
namespace py = pybind11;

enum class Piece { e_O, e_X };

struct Location {
    int row;
    int col;
};

class Board {
  private:
    uint16_t d_bitset_x{};
    uint16_t d_bitset_o{};

  public:
    Board() {};
    template <Piece P>
    int move(const Location &location);
    std::optional<Piece> winner();
};

template <Piece P>
inline int Board::move(const Location &location) {
    const int bit = 1 << (location.col + location.row * 3);

    if constexpr (P == Piece::e_O) {
        d_bitset_o |= bit;
    } else if constexpr (P == Piece::e_X) {
        d_bitset_x |= bit;
    }

    return 0;
}

#endif
