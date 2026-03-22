#include "board.h"
#include "arm_neon.h"
#include <cstdint>
#include <iostream>
#include <pybind11/native_enum.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>
namespace py = pybind11;

// 0 | 1 | 2
// 3 | 4 | 5
// 6 | 7 | 8
// 0b000000000
//   876543210
// Valid wins are:
//  - 012, 345, 678
//  - 036, 147, 258
//  - 048, 246

constexpr uint16x8_t success = {
    0b000000111, 0b000111000, 0b111000000, 0b001001001,
    0b010010010, 0b100100100, 0b100010001, 0b001010100,
};

PYBIND11_MODULE(_bindings, m, py::mod_gil_not_used()) {
    py::native_enum<Piece>(m, "Piece", "enum.Enum")
        .value("e_O", Piece::e_O)
        .value("e_X", Piece::e_X)
        .finalize();

    py::native_enum<GameState>(m, "GameState", "enum.Enum")
        .value("e_InPlay", GameState::e_InPlay)
        .value("e_OWon", GameState::e_OWon)
        .value("e_XWon", GameState::e_XWon)
        .value("e_Draw", GameState::e_Draw)
        .finalize();

    py::class_<Location>(m, "Location")
        .def(py::init<int, int>())
        .def("__repr__", [](const Location &l) {
            return "Location(row=" + std::to_string(l.row) +
                   ", col=" + std::to_string(l.col) + ")";
        });

    py::class_<Board>(m, "Board")
        .def("moveX", &Board::move<Piece::e_X>)
        .def("moveO", &Board::move<Piece::e_O>)
        .def("state", &Board::state)
        .def("board", &Board::board)
        .def("clear", &Board::clear)
        .def(py::init<>());
}

GameState Board::state() const {
    // Struggle to make faster due to 128 bit max register size on ARMv8s
    {
        const uint16x8_t board_bitset_vec = vdupq_n_u16(d_bitset_o);
        const uint16x8_t and_them = vandq_u16(success, board_bitset_vec);
        const uint16x8_t compared_equal = vceqq_s16(success, and_them);
        if (vmaxvq_u16(compared_equal)) {
            return GameState::e_OWon;
        }
    }
    {
        const uint16x8_t board_bitset_vec = vdupq_n_u16(d_bitset_x);
        const uint16x8_t and_them = vandq_u16(success, board_bitset_vec);
        const uint16x8_t compared_equal = vceqq_s16(success, and_them);
        if (vmaxvq_u16(compared_equal)) {
            return GameState::e_XWon;
        }
    }
    if ((d_bitset_o | d_bitset_x) + 1 == (1 << 9)) {
        return GameState::e_Draw;
    }
    return GameState::e_InPlay;
}

BoardType Board::board() const {
    BoardType b;
    for (size_t i = 0; i < 9; i++) {
        std::optional<Piece> piece;
        const int position = 1 << i;
        if (position & d_bitset_o) {
            piece.emplace(Piece::e_O);
        }
        if (position & d_bitset_x) {
            piece.emplace(Piece::e_X);
        }
        b[i / 3][i % 3] = piece;
    }
    return b;
}
