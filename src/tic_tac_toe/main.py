from ._bindings import Board, Location


def main() -> None:
    board = Board()
    board.moveX(Location(0, 0))
    board.moveO(Location(1, 0))
    board.moveX(Location(2, 0))
    print(board.winner())


if __name__ == "__main__":
    main()
