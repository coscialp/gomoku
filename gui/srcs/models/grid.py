from pydantic import BaseModel, Field, validator


WIDTH: int = 19
HEIGHT: int = 19


class Grid(BaseModel):
    rows: list[list[int]] = Field(
        default=[[0 for _ in range(WIDTH)] for _ in range(HEIGHT)]
    )

    def __init__(self) -> None:
        super().__init__()
