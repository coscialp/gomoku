from pydantic import BaseModel, Field, validator


class Coordinate(BaseModel):
    x: int
    y: int

    @validator("x", "y")
    def check_coordinate(cls, value: int) -> int:
        if value < 0:
            raise ValueError("Coordinate must be positive")
        return value

    def to_tuple(self) -> tuple[int, int]:
        return (self.x, self.y)
