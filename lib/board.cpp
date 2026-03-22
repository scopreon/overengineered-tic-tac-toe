#include "board.h"
#include "arm_neon.h"
#include <cstdint>
#include <iostream>
#include <optional>
#include <pybind11/native_enum.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
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

    py::class_<Location>(m, "Location").def(py::init<int, int>());

    py::class_<Board>(m, "Board")
        .def("moveX", &Board::move<Piece::e_X>)
        .def("moveO", &Board::move<Piece::e_O>)
        .def("winner", &Board::winner)
        .def(py::init<>());
}

// This can be made faster :)
std::optional<Piece> Board::winner() {
    {
        const uint16x8_t board_bitset_vec = vdupq_n_u16(d_bitset_o);
        const uint16x8_t and_them = vandq_u16(success, board_bitset_vec);
        const uint16x8_t compared_equal = vceqq_s16(success, and_them);
        const uint16_t max = vmaxvq_u16(compared_equal);
        if (max > 0) {
            return Piece::e_O;
        }
    }
    {
        const uint16x8_t board_bitset_vec = vdupq_n_u16(d_bitset_x);
        const uint16x8_t and_them = vandq_u16(success, board_bitset_vec);
        const uint16x8_t compared_equal = vceqq_s16(success, and_them);
        const uint16_t max = vmaxvq_u16(compared_equal);
        if (max > 0) {
            return Piece::e_X;
        }
    }
    return {};
}
