from ._bindings import Board, GameState, Location, Piece
from typing import Generator


def get_open_positions(board: Board) -> Generator[Location]:
    for i, row in enumerate(board.board()):
        for j, piece in enumerate(row):
            if piece is None:
                yield Location(i, j)


def minmax(board: Board, maximising: bool) -> int:
    if (state := board.state()) != GameState.e_InPlay:
        match state:
            case GameState.e_OWon:
                return 1
            case GameState.e_XWon:
                return -1
            case GameState.e_Draw:
                return 0

    if maximising:
        m = -99999999
        for location in get_open_positions(board):
            board.moveO(location)
            m = max(m, minmax(board, False))
            board.clear(location)
        return m
    else:
        m = 99999999
        for location in get_open_positions(board):
            board.moveX(location)
            m = min(m, minmax(board, True))
            board.clear(location)
        return m


def get_best_move(board: Board) -> Location:
    move_and_score: list[tuple[int, Location]] = []
    for location in get_open_positions(board):
        board.moveO(location)
        move_and_score.append((minmax(board, False), location))
        board.clear(location)

    print(move_and_score)
    return sorted(move_and_score, key=lambda x: x[0])[-1][1]


def main() -> None:
    board = Board()
    turn = Piece.e_O
    for row in board.board():
        print("".join(map(lambda x: "-" if x is None else x.name[2:], row)))
    while board.state() == GameState.e_InPlay:
        match turn:
            case Piece.e_O:
                print("Thinking")
                best_move = get_best_move(board)
                board.moveO(best_move)
            case Piece.e_X:
                move = tuple(map(int, input("Turn: ").split(" ")))
                board.moveX(Location(*move))
        for row in board.board():
            print("".join(map(lambda x: "-" if x is None else x.name[2:], row)))
        turn = Piece.e_O if turn == Piece.e_X else Piece.e_X

    state = board.state()
    match state:
        case GameState.e_OWon:
            print("Winner: O")
        case GameState.e_XWon:
            print("Winner: X")
        case GameState.e_Draw:
            print("Draw!")


if __name__ == "__main__":
    main()
