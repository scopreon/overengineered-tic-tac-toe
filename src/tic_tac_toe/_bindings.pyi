import enum
from typing import Optional

class Piece(enum.Enum):
    e_O: int
    e_X: int

class Location:
    def __init__(self, row: int, col: int) -> None: ...

class Board:
    def moveX(self, location: Location) -> int: ...
    def moveO(self, location: Location) -> int: ...
    def winner(self) -> Optional[Piece]: ...
