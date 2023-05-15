from srcs.models.grid import Grid


class Board:
    grid: Grid

    def __init__(self) -> None:
        self.grid = Grid()
